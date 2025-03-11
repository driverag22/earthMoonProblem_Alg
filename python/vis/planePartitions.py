import sys
from utils import read_partitions, draw_partitions


if __name__ == "__main__":
    if len(sys.argv) > 1:
        file_path = sys.argv[1]
    else:
        file_path = "data/partitions.txt"

    partition1, partition2 = read_partitions(file_path)

    if partition1 and partition2:
        draw_partitions(partition1, partition2, True)
