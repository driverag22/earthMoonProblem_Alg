import networkx as nx
import matplotlib.pyplot as plt


def draw_graph(edges, plane=False):
    """Draws a graph."""
    G = nx.Graph()
    G.add_edges_from(edges)
    if plane:
        if nx.check_planarity(G)[0]:
            pos = nx.planar_layout(G)
        else:
            print("graph is not planar")
            pos = nx.spring_layout(G)
    else:
        pos = nx.spring_layout(G)
    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True,
            edge_color="blue", node_color="lightgray")
    plt.show()


def draw_partitions(partition1, partition2, side_by_side=False):
    """Draws partitions either overlaid or side-by-side."""
    if side_by_side:
        fig, axes = plt.subplots(1, 2, figsize=(12, 6))
        for i, (edges, ax) in enumerate(zip([partition1, partition2], axes)):
            G = nx.Graph()
            G.add_edges_from(edges)
            pos = nx.planar_layout(G) if nx.check_planarity(G)[0] else nx.spring_layout(G)
            nx.draw(G, pos, ax=ax, with_labels=True,
                    node_color="lightblue", edge_color="black")
        plt.show()
    else:
        G = nx.Graph()
        G.add_edges_from(partition1)
        G.add_edges_from(partition2)
        pos = nx.spring_layout(G)
        plt.figure(figsize=(8, 6))
        nx.draw(G, pos, edgelist=partition1,
                edge_color="blue", with_labels=True, node_color="lightgray")
        nx.draw(G, pos, edgelist=partition2,
                edge_color="red", with_labels=True, node_color="lightgray")
        plt.show()
