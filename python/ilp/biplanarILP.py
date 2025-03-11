import networkx as nx
import gurobipy as gp
import sys
from gurobipy import GRB
from utils import read_graph, extract_vertices, output_graph, draw_partitions, isPlanar


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
        is_planar0, edges0 = isPlanar(G0)
        if not is_planar0:
            # For partition 0 (where x[e] == 0), add constraints:
            #   - sum_{e in cert0} x[e] >= 1
            #     (at least one edge must be in partition 1)
            #   - sum_{e in cert0} x[e] <= |cert0| - 1
            #     (at least one edge must be in partition 0)
            model.cbLazy(gp.quicksum(
                model._x[e] for e in edges0) >= 1)
            model.cbLazy(gp.quicksum(
                model._x[e] for e in edges0) <= len(edges0) - 1)

        # Check planarity for partition 1.
        G1 = nx.Graph()
        G1.add_nodes_from(model._nodes)
        G1.add_edges_from(partition1_edges)
        is_planar1, edges1 = isPlanar(G1)
        if not is_planar1:
            model.cbLazy(gp.quicksum(
                model._x[e] for e in edges1) >= 1)
            model.cbLazy(gp.quicksum(
                model._x[e] for e in edges1) <= len(edges1) - 1)



if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "data/test.txt"

    edges = read_graph(file_path)
    nodes = extract_vertices(edges)

    # Create a Gurobi model.
    model = gp.Model("biplanar")

    # Create a binary variable for each edge:
    # x[e] = 1 if edge e is assigned to partition 1;
    #   0 if assigned to partition 0.
    x = {}
    for e in edges:
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
        gp.quicksum(x[e] for e in edges)
        <= 3*n - 6, name="partition1_limit")
    model.addConstr(
        gp.quicksum(1 - x[e] for e in edges)
        <= 3*n - 6, name="partition0_limit")
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
        output_graph("data/ILP_partition.txt", [partition0, partition1])
        draw_partitions(partition0, partition1, False)
    else:
        print("No biplanar partition exists for this graph.")
