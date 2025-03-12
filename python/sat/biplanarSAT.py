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
        solver.add_clause([
            edge_to_var(e, edge_to_var_map) for e in violating_edges
        ])

        # Not all violating_edges to true (partition1)
        # !x_0 \lor !x_1 \lor ... = !(x_0 \land x_1 \land ...)
        solver.add_clause([
            -edge_to_var(e, edge_to_var_map) for e in violating_edges
        ])
    return is_planar


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

    # Fix first edge to one partition to avoid symmetric cases.
    solver.add_clause([edge_to_var(edges[0], edge_to_var_map)])

    n = len(nodes)
    edgesLimit = 3 * n - 6

    edge_vars = [edge_to_var(e, edge_to_var_map) for e in edges]
    # Partition 1:
    #          sum(1{x_e}) <= 3n - 6
    cnf1 = CardEnc.atmost(lits=edge_vars, bound=edgesLimit, encoding=1)
    for clause in cnf1.clauses:
        solver.add_clause(clause)

    # For some reason, having this constraint as well as cnf0 somehow
    # breaks things, so commented out for now.
    # comp_edge_vars = [-v for v in edge_vars]
    # # Partition 0:
    # #          sum(1 - 1{x_e}) <= 3n - 6
    # cnf2 = CardEnc.atmost(lits=comp_edge_vars, bound=edgesLimit, encoding=1)
    # for clause in cnf2.clauses:
    #     solver.add_clause(clause)

    # Planarity constraints using lazy clause generation
    while True:
        if not solver.solve():
            print("No biplanar partition exists for this graph.")
            return None

        # Extract current edge assignments from the SAT solver
        model = solver.get_model()
        partition0, partition1 = [], []
        for v in model:
            if v in var_to_edge_map:
                partition1.append(var_to_edge_map[v])
            elif -v in var_to_edge_map:
                partition0.append(var_to_edge_map[-v])

        # Check if both partitions are planar
        valid0 = add_planarity_clauses(solver, partition0, edge_to_var_map)
        valid1 = add_planarity_clauses(solver, partition1, edge_to_var_map)

        if valid0 and valid1:
            return partition0, partition1  # Found a valid partition


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
