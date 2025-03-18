import networkx as nx
import sys
import concurrent.futures
from pysat.solvers import Solver
from pysat.card import CardEnc
from utils import read_graph, extract_vertices, output_graph, isPlanar, draw_partitions


def edge_to_var(edge, edge_to_var_map):
    """ Maps an edge to a SAT variable index. """
    return edge_to_var_map[edge]


def check_planarity_clauses(partition_edges, edge_to_var_map):
    """
    Adds CNF clauses to enforce planarity constraints.
    If a subset of edges forms a non-planar subgraph,
    at least one of those edges must be reassigned to
    the other partition.
    """
    G = nx.Graph()
    G.add_edges_from(partition_edges)
    is_planar, violating_edges = isPlanar(G)
    clauses = []

    if not is_planar:
        # Not all violating_edges to false(partition0)
        # x_0 \lor x_1 \lor ...
        clause1 = [edge_to_var(e, edge_to_var_map) for e in violating_edges]

        # Not all violating_edges to true (partition1)
        # !x_0 \lor !x_1 \lor ... = !(x_0 \land x_1 \land ...)
        clause2 = [-edge_to_var(e, edge_to_var_map) for e in violating_edges]

        clauses.extend([clause1, clause2])

    return is_planar, clauses


def solve_biplanar(edges, nodes):
    """ Solves the biplanar graph problem using a SAT solver. """
    # map edges to SAT variables
    edge_to_var_map = {e: i+1 for i, e in enumerate(edges)}
    var_to_edge_map = {v: e for e, v in edge_to_var_map.items()}

    solver = Solver(name="cms")  # CryptoMiniSat solver

    # Edge count constraints (each edge belongs to one of two partitions)
    for e in edges:
        solver.add_clause([edge_to_var(e, edge_to_var_map),
                           -edge_to_var(e, edge_to_var_map)])

    n = len(nodes)
    edgesLimit = 3 * n - 6
    edgesUpperBound = n - edgesLimit

    edge_vars = [edge_to_var(e, edge_to_var_map) for e in edges]
    # Partition 1:
    #          sum(1{x_e}) <= 3n - 6
    cnf1 = CardEnc.atmost(lits=edge_vars, bound=edgesLimit, encoding=1)
    for clause in cnf1.clauses:
        solver.add_clause(clause)

    # For some reason, having this constraint as well as cnf0 somehow
    # breaks things, so commented out for now.
    # comp_edge_vars = [-v for v in edge_vars]
    # Partition 0:
    #          sum(1 - 1{x_e}) <= 3n - 6 <=> \sum(1{x_e} => num_edges - 3n + 6
    cnf2 = CardEnc.atleast(lits=edge_vars, bound=edgesUpperBound, encoding=1)
    for clause in cnf2.clauses:
        solver.add_clause(clause)

    # Create a process pool for parallel planarity checking.
    with concurrent.futures.ProcessPoolExecutor(max_workers=2) as executor:
        # Planarity constraints using lazy clause generation
        while True:
            if not solver.solve():
                print("No biplanar partition exists for this graph.")
                return None

            # extract current edge partitions
            model = solver.get_model()
            partition0, partition1 = [], []
            for v in model:
                if v in var_to_edge_map:
                    partition1.append(var_to_edge_map[v])
                elif -v in var_to_edge_map:
                    partition0.append(var_to_edge_map[-v])

            # run planarity checks in parallel.
            future0 = executor.submit(
                check_planarity_clauses, partition0, edge_to_var_map)
            future1 = executor.submit(
                check_planarity_clauses, partition1, edge_to_var_map)
            valid0, clauses0 = future0.result()
            valid1, clauses1 = future1.result()

            # if any partition is non-planar, add the corresponding clauses.
            if not valid0:
                for clause in clauses0:
                    solver.add_clause(clause)
            if not valid1:
                for clause in clauses1:
                    solver.add_clause(clause)

            # if both partitions are planar, we have found a valid partition.
            if valid0 and valid1:
                return partition0, partition1


if __name__ == '__main__':
    # get input and output files
    file_path = sys.argv[1] if len(sys.argv) > 1 else "data/test.txt"
    output_file = sys.argv[2] if len(sys.argv) > 2 else "data/SAT_partition.txt"

    edges = read_graph(file_path)
    nodes = extract_vertices(edges)

    result = solve_biplanar(edges, nodes)

    if result:
        partition0, partition1 = result
        print("Found a biplanar partition:")
        print("Partition 0:", partition0)
        print("Partition 1:", partition1)
        print(edges)
        output_graph(output_file, [partition0, partition1])
        draw_partitions(partition0, partition1, False)
    else:
        print("Found no biplanar partition")
        output_graph(output_file.replace(".txt", "_failed.txt"), [edges])
