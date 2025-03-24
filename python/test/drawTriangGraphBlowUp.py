import networkx as nx
import matplotlib.pyplot as plt

# Define grid size
n = 4
# Create the original grid graph
G = nx.Graph()


# Helper to compute vertex number in column-major order
def node_index(i, j):
    return i * n + j


# Add vertices (0 to n*n - 1)
G.add_nodes_from(range(n * n))


# add edges for the triangular grid:
for i in range(n):
    for j in range(n):
        current = node_index(i, j)
        neighbours = [
            (i, j+1),    # top
            (i, j-1),    # bottom
            (i-1, j),    # left
            (i+1, j),    # right
            (i-1, j+1),  # top left
            (i+1, j-1)   # bottom right
        ]
        for ni, nj in neighbours:
            # make sure to only take valid edges
            if 0 <= ni < n and 0 <= nj < n:
                neighbour_index = node_index(ni, nj)
                G.add_edge(current, neighbour_index)

# positions for the original graph (column-major ordering)
pos = {v: (v // n, v % n) for v in range(n * n)}

# compute the 2-blow-up graph G2:
G2 = nx.Graph()
for v in G.nodes():
    G2.add_node((v, 0))
    G2.add_node((v, 1))

# def. positions for the blown-up vertices:
#   - (v,0) at pos[v]
#   - (v,1) shifted by an offset (-0.3, -0.3)
pos2 = {}
offset = 0.3
for v in G.nodes():
    x, y = pos[v]
    pos2[(v, 0)] = (x, y)
    pos2[(v, 1)] = (x - offset, y - offset)

# Add edges:
# (Note: Since the graph is undirected, iterating over
# each edge once is sufficient.)
for u, v in G.edges():
    G2.add_edge((u, 0), (v, 0))
    G2.add_edge((u, 1), (v, 0))
    G2.add_edge((u, 0), (v, 1))

# Draw the blow-up graph
plt.figure(figsize=(8, 8))
nx.draw(G2, pos2, with_labels=True, node_color='lightblue',
        edge_color='gray', node_size=500)
plt.title("2-Blow-Up of the Triangular Grid Graph")
plt.show()
