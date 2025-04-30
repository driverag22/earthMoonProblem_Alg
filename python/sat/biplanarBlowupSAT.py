import networkx as nx
import sys
import concurrent.futures
from pysat.solvers import Solver
from pysat.card import CardEnc
from utils import read_graph, extract_vertices, output_graph, isPlanar, draw_partitions
from collections import defaultdict


# def compute_dfs_tree(edges):
#     graph = defaultdict(set)
#     for u, v in edges:
#         graph[u].add(v)
#         graph[v].add(u)
# 
#     # Find root: vertex with highest degree
#     root = max(graph.keys(), key=lambda x: len(graph[x]))
# 
#     tree_edges = []
#     visited = set()
#     parent = {}
# 
#     def dfs(u):
#         visited.add(u)
#         for v in graph[u]:
#             if v not in visited:
#                 parent[v] = u
#                 tree_edges.append((u, v))
#                 dfs(v)
# 
#     dfs(root)
#     return root, tree_edges


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
    """
    Solves the biplanar graph problem using a SAT solver
    for blown-up graph.
    """
    print(edges)
    # get original edges (pre-blowup), find spanning rooted tree
    # original_edges = [edges[i] for i in range(0, len(edges), 4)]
    # root, tree_edges = compute_dfs_tree(original_edges)

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
    # Partition 0:
    #          sum(1 - 1{x_e}) <= 3n - 6 <=> \sum(1{x_e}) => num_edges - 3n + 6
    cnf2 = CardEnc.atleast(lits=edge_vars, bound=edgesUpperBound, encoding=1)
    for clause in cnf2.clauses:
        solver.add_clause(clause)

    # Blow-up related constraints
    # Here we use/assume that for blown-up graphs, edges corresponding to the
    # same original edge ab are passed always together, and in the following
    # order:    ab, a'b', a'b, ab'
    for i in range(0, len(edges), 4):
        group = edges[i:i+4]
        vars_group = [edge_to_var(e, edge_to_var_map) for e in group]
        # corresponds to:
        # ab, a'b', a'b, ab'
        v0, v1, v2, v3 = vars_group

        # No monochromatic edges:
        solver.add_clause([-v0, -v1, -v2, -v3])  # not all true
        solver.add_clause([v0, v1, v2, v3])  # not all false

        # No matchings:
        # No ab=v0, a'b'=v1 true and a'b=v3 ab'=v3 false
        # So add clause: ¬v0 ∨ ¬v1 ∨ v2 ∨ v3
        solver.add_clause([-v0, -v1, v2, v3])

        # No ab=v0, a'b'=v1 false and a'b=v3 ab'=v3 true
        # So add clause: v0 ∨ v1 ∨ ¬v2 ∨ ¬v3
        solver.add_clause([v0, v1, -v2, -v3])

    # TODO: enforce this correctly
    # for edge in tree_edges:
    #     a, b = edge
    #     # Find index of this edge's group
    #     try:
    #         i = original_edges.index((a, b))
    #         forward = True
    #     except ValueError:
    #         i = original_edges.index((b, a))
    #         a, b = b, a  # ensure (a, b) is in DFS order
    #         forward = False

    #     e_group = edges[4*i:4*i+4]
    #     v0 = edge_to_var(e_group[0], edge_to_var_map)  # (a, b)
    #     v1 = edge_to_var(e_group[1], edge_to_var_map)  # (a′, b′)
    #     if forward:
    #         v2 = edge_to_var(e_group[2], edge_to_var_map)  # (b, a′)
    #         v3 = edge_to_var(e_group[3], edge_to_var_map)  # (a, b′)
    #     else:
    #         # Need to reverse roles of v2 and v3
    #         v2 = edge_to_var(e_group[3], edge_to_var_map)  # (b, a′)
    #         v3 = edge_to_var(e_group[2], edge_to_var_map)  # (a, b′)

    #     # 3-1 case: singleton edge must be ab=v0 or a'b'=v1
    #     # Forbid singleton is v2
    #     solver.add_clause([v0, v1, -v2, v3])
    #     # Forbid singleton is v3
    #     solver.add_clause([v0, v1, v2, -v3])

    #     # 2-2 case with center at b and b′:
    #     #     enforce ba=v0, ba'=v2 True, b′a=v3, b′a′=v1 False
    #     # Enforce v0 == v2 (ba, ba′ same color)
    #     solver.add_clause([v0, -v2])
    #     solver.add_clause([-v0, v2])
    #     # Enforce v1 == v3 (b′a, b′a′ same color)
    #     solver.add_clause([v1, -v3])
    #     solver.add_clause([-v1, v3])

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
        draw_partitions(partition0, partition1, False, True)
    else:
        print("Found no biplanar partition")
        output_graph(output_file.replace(".txt", "_failed.txt"), [edges])
