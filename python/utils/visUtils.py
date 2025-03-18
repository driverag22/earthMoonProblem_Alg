import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm


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


def draw_partitions(partition1, partition2, side_by_side=False, blow_up=False):
    """Draws partitions either overlaid or side-by-side.
    If blow_up=True, assigns the same color to vertices i and i+n,
    where n = num_vertices/2."""
    all_nodes = set()
    for edge in partition1 + partition2:
        all_nodes.update(edge)

    num_vertices = len(all_nodes)
    node_list = sorted(all_nodes)  # Ensure consistent ordering

    if blow_up:
        n = num_vertices // 2
        colormap = cm.get_cmap("tab10" if n <= 10 else "tab20")
        colors = [colormap(i / max(1, n - 1)) for i in range(n)]
        node_colors = {node: colors[i % n] for i, node in enumerate(node_list)}
    else:
        node_colors = {node: "lightgray" for node in all_nodes}

    if side_by_side:
        fig, axes = plt.subplots(1, 2, figsize=(12, 6))
        for i, (edges, ax) in enumerate(zip([partition1, partition2], axes)):
            G = nx.Graph()
            G.add_edges_from(edges)
            pos = nx.planar_layout(G) if nx.check_planarity(G)[0] else nx.spring_layout(G)
            nx.draw(G, pos, ax=ax, with_labels=True,
                    node_color=[node_colors[node] for node in G.nodes()],
                    edge_color="black")
        plt.show()
    else:
        G = nx.Graph()
        G.add_edges_from(partition1)
        G.add_edges_from(partition2)
        pos = nx.spring_layout(G)
        plt.figure(figsize=(8, 6))
        nx.draw(G, pos, edgelist=partition1,
                edge_color="blue", with_labels=True,
                node_color=[node_colors[node] for node in G.nodes()])
        nx.draw(G, pos, edgelist=partition2,
                edge_color="red", with_labels=True,
                node_color=[node_colors[node] for node in G.nodes()])
        plt.show()
