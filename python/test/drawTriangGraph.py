import networkx as nx
import matplotlib.pyplot as plt

n = 4  # for a 5x5 grid, adjust as needed
G = nx.Graph()

# Suppose you already have a list of edges for the triangular grid.
# For demonstration, we'll create the grid and add edges accordingly.
# Each vertex at (i, j) is connected to:
#   - its top (i, j+1), bottom (i, j-1),
#   - left (i-1, j), right (i+1, j),
#   - top left (i-1, j+1), and bottom right (i+1, j-1).


def node_index(i, j):
    return i * n + j


# Add vertices (0 to n^2-1)
G.add_nodes_from(range(n * n))

# Add edges
for i in range(n):
    for j in range(n):
        current = node_index(i, j)
        # Neighbour coordinates
        neighbours = [
            (i, j+1),    # top
            (i, j-1),    # bottom
            (i-1, j),    # left
            (i+1, j),    # right
            (i-1, j+1),  # top left
            (i+1, j-1)   # bottom right
        ]
        for ni, nj in neighbours:
            if 0 <= ni < n and 0 <= nj < n:
                neighbour_index = node_index(ni, nj)
                G.add_edge(current, neighbour_index)

# Create positions: column-major ordering
pos = {v: (v // n, v % n) for v in range(n * n)}

# Draw the graph
plt.figure(figsize=(6, 6))
nx.draw(G, pos, with_labels=True, node_color='lightblue', edge_color='gray', node_size=500)
plt.show()
