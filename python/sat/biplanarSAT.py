import networkx as nx
import sys
from pysat.solvers import Solver
from pysat.card import CardEnc
from utils import read_graph, extract_vertices, output_graph, isPlanar, draw_partitions


def edge_to_var(edge, edge_to_var_map):
    """ Maps an edge to a SAT variable index. """
    return edge_to_var_map[edge]


def add_planarity_clauses(solver, partition_edges, edge_to_var_map):
    """
    Adds CNF clauses to enforce planarity constraints.
    If a subset of edges forms a non-planar subgraph,
    at least one of those edges must be reassigned to
    the other partition.
    """
    G = nx.Graph()
    G.add_edges_from(partition_edges)
    is_planar, violating_edges = isPlanar(G)

    if not is_planar:
        # Not all violating_edges to false(partition0)
        # x_0 \lor x_1 \lor ...
        clause = [edge_to_var(e, edge_to_var_map) for e in violating_edges]
        solver.add_clause(clause)  # Ensure at least one edge changes partition

        # Not all violating_edges to true (partition1)
        # !x_0 \lor !x_1 \lor ... = !(x_0 \land x_1 \land ...)
        clause2 = [-edge_to_var(e, edge_to_var_map) for e in violating_edges]
        solver.add_clause(clause2)
        return False
    return True


def solve_biplanar(edges, nodes):
    """ Solves the biplanar graph problem using a SAT solver. """
    # map edges to SAT variables
    edge_to_var_map = {e: i+1 for i, e in enumerate(edges)}
    var_to_edge_map = {v: e for e, v in edge_to_var_map.items()}

    solver = Solver(name="cms")  # CryptoMiniSat solver

    # Edge count constraints (each edge belongs to one of two partitions)
    #    at least one
    solver.add_clause([edge_to_var(e, edge_to_var_map) for e in edges])
    #    at most one
    solver.add_clause([-edge_to_var(e, edge_to_var_map) for e in edges])

    n = len(nodes)
    edgesLimit = 3 * n - 6

    edge_vars = [edge_to_var(e, edge_to_var_map) for e in edges]
    # Partition 1:
    #          sum(1{x_e}) <= 3n - 6
    cnf1 = CardEnc.atmost(lits=edge_vars,
                          bound=edgesLimit, encoding=1)
    for clause in cnf1.clauses:
        solver.add_clause(clause)

    comp_edge_vars = [-v for v in edge_vars]
    # Partition 0:
    #          sum(1 - 1{x_e}) <= 3n - 6
    cnf2 = CardEnc.atleast(lits=comp_edge_vars,
                           bound=edgesLimit, encoding=1)
    for clause in cnf2.clauses:
        solver.add_clause(clause)

    # Planarity constraints using lazy clause generation
    while True:
        if not solver.solve():
            print("No biplanar partition exists for this graph.")
            return None

        # Extract current edge assignments from the SAT solver
        model = solver.get_model()
        partition0, partition1 = [], []
        for v in model:
            if v > 0:
                if v in var_to_edge_map:
                    partition1.append(var_to_edge_map[v])
            elif -v in var_to_edge_map:
                partition0.append(var_to_edge_map[-v])

        # Check if both partitions are planar
        valid0 = add_planarity_clauses(solver, partition0, edge_to_var_map)
        valid1 = add_planarity_clauses(solver, partition1, edge_to_var_map)

        if valid0 and valid1:
            return partition0, partition1  # Found a valid partition


if len(sys.argv) > 1:
    file_path = sys.argv[1]
else:
    file_path = "data/test.txt"

edges = read_graph(file_path)
nodes = extract_vertices(edges)

result = solve_biplanar(edges, nodes)

if result:
    partition0, partition1 = result
    print("Found a biplanar partition:")
    print("Partition 0:", partition0)
    print("Partition 1:", partition1)
    output_graph("data/SAT_partition.txt", [partition0, partition1])
    draw_partitions(partition0, partition1, False)
