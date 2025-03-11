import networkx as nx
import gurobipy as gp
from gurobipy import GRB


def read_graph(file_path):
    """Reads graph edges from the given file."""
    with open(file_path, "r") as f:
        lines = f.readlines()

    edges = []

    for line in lines:
        line = line.strip()
        if not line:  # empty line
            break
        u, v = map(int, line.split())
        edges.append(tuple(sorted((u+1, v+1))))

    return edges


def extract_vertices(edges):
    """Extracts and returns
    a sorted list of vertices from the given list of edges."""
    vertices = {u for edge in edges for u in edge}  # collect all endpoints
    return sorted(vertices)


def biplanar_callback(model, where):
    if where == GRB.Callback.MIPSOL:
        # retrieve the current solution values.
        sol = model.cbGetSolution(model._x)
        # extract the edges in partition 0 and partition 1.
        partition0_edges = [e for e in model._edges if sol[e] < 0.5]
        partition1_edges = [e for e in model._edges if sol[e] >= 0.5]

        # Check planarity for partition 0.
        G0 = nx.Graph()
        G0.add_nodes_from(model._nodes)
        G0.add_edges_from(partition0_edges)
        is_planar0, cert0 = nx.check_planarity(G0, counterexample=True)
        if not is_planar0:
            # For partition 0 (where x[e] == 0), add constraints:
            #   - sum_{e in cert0} x[e] >= 1
            #     (at least one edge must be in partition 1)
            #   - sum_{e in cert0} x[e] <= |cert0| - 1
            #     (at least one edge must be in partition 0)
            cert0_edges = [tuple(sorted(e)) for e in cert0.edges()]
            model.cbLazy(gp.quicksum(
                model._x[e] for e in cert0_edges) >= 1)
            model.cbLazy(gp.quicksum(
                model._x[e] for e in cert0_edges) <= len(cert0) - 1)

        # Check planarity for partition 1.
        G1 = nx.Graph()
        G1.add_nodes_from(model._nodes)
        G1.add_edges_from(partition1_edges)
        is_planar1, cert1 = nx.check_planarity(G1, counterexample=True)
        if not is_planar1:
            cert1_edges = [tuple(sorted(e)) for e in cert1.edges()]
            model.cbLazy(gp.quicksum(
                model._x[e] for e in cert1_edges) >= 1)
            model.cbLazy(gp.quicksum(
                model._x[e] for e in cert1_edges) <= len(cert1) - 1)


edges = read_graph('candidateDisprove.txt')
nodes = extract_vertices(edges)
print("Vertices:", nodes)
print("edges:", edges)

# Create a Gurobi model.
model = gp.Model("biplanar")

# Create a binary variable for each edge:
# x[e] = 1 if edge e is assigned to partition 1; 0 if assigned to partition 0.
x = {}
for e in edges:
    print(e)
    x[e] = model.addVar(vtype=GRB.BINARY, name=f"x_{e}")
model.update()

# Attach our data to the model for use in the callback.
model._x = x
model._edges = edges
# model._var_to_edge = {v: e for e, v in x.items()}
model._nodes = nodes

# Add edge count constraints for each partition.
n = len(nodes)
model.addConstr(
    gp.quicksum(x[e] for e in edges) <= 3*n - 6, name="partition1_limit")
model.addConstr(
    gp.quicksum(1 - x[e] for e in edges) <= 3*n - 6, name="partition0_limit")
model.addConstr(
    x[edges[0]] == 0, name="first_edge_constraint")

# Enable lazy constraints.
model.Params.LazyConstraints = 1

# Optimize with the callback.
model.optimize(biplanar_callback)

# After optimization, if the model is feasible,
# the solution gives a valid biplanar partition.
if model.Status == GRB.OPTIMAL:
    partition0, partition1 = [], []
    for e in edges:
        if x[e].X >= 0.5:
            partition0.append(e)
        if x[e].X < 0.5:
            partition1.append(e)
    print("Found a biplanar partition:")
    print(partition0)
    print(partition1)
else:
    print("No biplanar partition exists for this graph.")
