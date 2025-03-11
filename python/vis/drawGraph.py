import sys
from utils import read_graph, draw_graph


if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "data/test.txt"

    graph = read_graph(file_path)
    if graph:
        draw_graph(graph)
