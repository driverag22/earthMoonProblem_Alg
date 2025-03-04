import networkx as nx
import matplotlib.pyplot as plt
import sys

def read_partitions(file_path):
    """Reads edge partitions from the given file."""
    with open(file_path, "r") as f:
        lines = f.readlines()

    partition1, partition2, partition3 = [], [], []
    current_partition = partition1

    for line in lines:
        line = line.strip()
        if not line:  # empty line is switch to second partition
            if current_partition is partition1:
                current_partition = partition2
            elif current_partition is partition2:
                current_partition = partition3
            continue
        u, v = map(int, line.split())
        current_partition.append((u+1, v+1))

    return partition1, partition2, partition3

def draw_graph(partition1, partition2, partition3):
    """Draws the two partitions with red, blue and green."""
    G = nx.Graph()
    G.add_edges_from(partition1)
    G.add_edges_from(partition2)
    G.add_edges_from(partition3)

    pos = nx.spring_layout(G)

    plt.figure(figsize=(8, 6))

    # first partition (blue)
    nx.draw(G, pos, edgelist=partition1, edge_color="blue", with_labels=True, node_color="lightgray")

    # second partition (red)
    nx.draw(G, pos, edgelist=partition2, edge_color="red", with_labels=True, node_color="lightgray")

    # third partition (green)
    nx.draw(G, pos, edgelist=partition3, edge_color="green", with_labels=True, node_color="lightgray")

    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "data/partitions.txt"

    partition1, partition2, partition3 = read_partitions(file_path)

    if partition1 and partition2 and partition3:
        draw_graph(partition1, partition2, partition3)
