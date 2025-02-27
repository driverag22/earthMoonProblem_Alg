import networkx as nx
import matplotlib.pyplot as plt
import sys

def read_partition(file_path):
    """Reads edge partitions from the given file."""
    with open(file_path, "r") as f:
        lines = f.readlines()

    partition = []

    for line in lines:
        line = line.strip()
        if not line:  # empty line
            break
        u, v = map(int, line.split())
        partition.append((u+1, v+1))

    return partition

def draw_graph(partition):
    """Draws a graph."""
    G = nx.Graph()
    G.add_edges_from(partition)

    pos = nx.spring_layout(G)

    plt.figure(figsize=(8, 6))

    nx.draw(G, pos, edgelist=partition, edge_color="blue", with_labels=True, node_color="lightgray")

    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = input("Enter the file name: ")

    partition = read_partition(file_path)
    if partition:
        draw_graph(partition)
