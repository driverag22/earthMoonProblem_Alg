def read_graph(file_path):
    """Reads graph edges from a file."""
    with open(file_path, "r") as f:
        lines = f.readlines()
    edges = [tuple(sorted(map(int, line.strip().split())))
             for line in lines if line.strip()]
    return edges


def read_partitions(file_path):
    """Reads edge partitions from a file."""
    with open(file_path, "r") as f:
        lines = f.readlines()

    partition1, partition2 = [], []
    current_partition = partition1
    for line in lines:
        line = line.strip()
        if not line:
            current_partition = partition2
            continue
        u, v = map(int, line.split())
        current_partition.append((u, v))

    return partition1, partition2


def extract_vertices(edges):
    """Extracts vertices from edges."""
    return sorted(set(u for edge in edges for u in edge))


def output_graph(file_path, partitions):
    """Writes a graph or partitions to a file
    with an empty line separating partitions."""
    with open(file_path, "w") as f:
        for partition in partitions:
            for edge in partition:
                f.write(f"{edge[0]} {edge[1]}\n")
            f.write("\n")  # empty line to separate partitions
