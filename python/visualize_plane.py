import networkx as nx
import matplotlib.pyplot as plt
import sys

def read_partitions(file_path):
    """Reads edge partitions from the given file."""
    with open(file_path, "r") as f:
        lines = f.readlines()

    partition1, partition2 = [], []
    current_partition = partition1

    for line in lines:
        line = line.strip()
        if not line:  # empty line is switch to second partition
            current_partition = partition2
            continue
        u, v = map(int, line.split())
        current_partition.append((u+1, v+1))

    return partition1, partition2

def draw_graphs(partition1, partition2):
    """Draws both partitions side by side."""
    fig, axes = plt.subplots(1, 2, figsize=(12, 6))
    titles = ["Partition 1 - Plane Drawing", "Partition 2 - Plane Drawing"]

    for i, (edges, ax) in enumerate(zip([partition1, partition2], axes)):
        G = nx.Graph()
        G.add_edges_from(edges)

        ax.set_title(titles[i])

        # Use planar layout if the graph is planar, otherwise spring layout
        if nx.check_planarity(G)[0]:
            pos = nx.planar_layout(G)
        else:
            print(f"Warning: {titles[i]} is not planar, using spring layout.")
            pos = nx.spring_layout(G)

        nx.draw(G, pos, ax=ax, with_labels=True, node_color="lightblue", edge_color="black")

    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "data/partitions.txt"

    partition1, partition2 = read_partitions(file_path)

    if partition1 and partition2:
        draw_graphs(partition1, partition2)
        # draw_graph(partition2, "Partition 2 - Plane Drawing")
