import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import numpy as np

n = 5  # grid size
G = nx.Graph()


def node_index(i, j):
    return i * n + j


# add nodes and edges
for i in range(n):
    for j in range(n):
        current = node_index(i, j)
        G.add_node(current)
        neighbours = [
            (i, j+1),    # top
            (i, j-1),    # bottom
            (i-1, j),    # left
            (i+1, j),    # right
            (i-1, j+1),  # top left
            (i+1, j-1)   # bottom right
        ]
        for ni, nj in neighbours:
            # only take valid edges
            if 0 <= ni < n and 0 <= nj < n:
                neighbour_index = node_index(ni, nj)
                G.add_edge(current, neighbour_index)

# Original positions (column-major ordering)
pos = {v: (v // n, v % n) for v in G.nodes()}

# 2-blow-up:
G2 = nx.Graph()
for v in G.nodes():
    G2.add_node((v, 0))
    G2.add_node((v, 1))

for u, v in G.edges():
    # add only one direction (undirected graph)
    G2.add_edge((u, 0), (v, 0))
    G2.add_edge((u, 1), (v, 0))
    G2.add_edge((u, 0), (v, 1))

# positions for the blown-up vertices
# (store original positions for resetting):
offset = 0.3
orig_positions = {}
for v in G.nodes():
    x, y = pos[v]
    orig_positions[(v, 0)] = (x, y)
    orig_positions[(v, 1)] = (x - offset, y - offset)

node_positions = orig_positions.copy()

# color mapping so vertex and its blow up get the same color
# shuffle colors to have similar colors far away from each other
vertices = sorted(G.nodes())
num_orig = len(vertices)
# Create evenly spaced color values and shuffle them.
color_values = np.linspace(0, 1, num_orig)
np.random.shuffle(color_values)
color_mapping = {}
for i, v in enumerate(vertices):
    color_mapping[v] = plt.cm.nipy_spectral(color_values[i])

####################
# Interactive Plot #
####################
fig, ax = plt.subplots(figsize=(20, 20))
plt.subplots_adjust(bottom=0.15)
ax.set_title('''Interactive 2-Blow-Up Graph \n
(click edge to cycle colors; drag nodes to move them)''')
ax.axis('equal')

# nodes as scatter points
nodes = list(G2.nodes())
node_coords = [node_positions[node] for node in nodes]
node_colors = [color_mapping[node[0]] for node in nodes]
base_marker_size = 200

# draw nodes using scatter
scat = ax.scatter([c[0] for c in node_coords],
                  [c[1] for c in node_coords],
                  s=base_marker_size, c=node_colors, zorder=3, picker=5)

# draw edges and store them in a dictionary:
# key: frozenset({u,v}) ensures each undirected edge is stored once.
colors = ["gray", "red", "blue"]
edge_lines = {}  # map frozenset({u, v}) -> [line, state]
for u, v in G2.edges():
    edge_key = frozenset({u, v})
    if edge_key in edge_lines:
        continue
    x0, y0 = node_positions[list(edge_key)[0]]
    x1, y1 = node_positions[list(edge_key)[1]]
    line, = ax.plot([x0, x1], [y0, y1], color=colors[0],
                    lw=2, picker=5, zorder=1)
    edge_lines[edge_key] = [line, 0]  # initially gray

# Interactive Event Handling
dragging = {"node": None, "offset": (0, 0)}


def update_edge_positions():
    """Update the endpoints of all edges based on node_positions."""
    for edge_key, (line, state) in edge_lines.items():
        u, v = list(edge_key)
        x0, y0 = node_positions[u]
        x1, y1 = node_positions[v]
        line.set_data([x0, x1], [y0, y1])
    fig.canvas.draw_idle()


def update_node_positions():
    """Update scatter positions based on node_positions."""
    new_coords = [node_positions[node] for node in nodes]
    scat.set_offsets(new_coords)
    fig.canvas.draw_idle()


def update_marker_sizes():
    """Update marker sizes based on current axis
    limits so nodes grow when zoomed in."""
    # Use the x-axis limits as reference.
    cur_xlim = ax.get_xlim()
    cur_range = cur_xlim[1] - cur_xlim[0]
    # Assume the original x-range is approximately n (from 0 to n)
    scale_factor = n / cur_range
    new_size = base_marker_size * scale_factor
    scat.set_sizes([new_size]*len(nodes))
    fig.canvas.draw_idle()


def on_pick(event):
    global dragging
    # ignore if scroll event
    if event.mouseevent.name == 'scroll_event':
        return
    # if nodes (scatter) is picked, start dragging
    if event.artist == scat:
        if dragging["node"] is not None:
            return  # already dragging a node
        ind = event.ind[0]
        node = nodes[ind]
        dragging["node"] = node
        xclick, yclick = event.mouseevent.xdata, event.mouseevent.ydata
        xnode, ynode = node_positions[node]
        dragging["offset"] = (xnode - xclick, ynode - yclick)
        # return so that overlapping edges don't get processed.
        return
    # Only process line picks if no node is being dragged.
    if dragging["node"] is not None:
        return
    # otherwise, if edge was picked, cycle its color.
    for edge_key, (line, state) in edge_lines.items():
        if line == event.artist:
            new_state = (state + 1) % len(colors)
            edge_lines[edge_key][1] = new_state
            line.set_color(colors[new_state])
            fig.canvas.draw_idle()
            break


def on_release(event):
    global dragging
    if dragging["node"] is not None:
        dragging["node"] = None


def on_motion(event):
    if dragging["node"] is None:
        return
    if event.xdata is None or event.ydata is None:
        return
    node = dragging["node"]
    dx, dy = dragging["offset"]
    new_x = event.xdata + dx
    new_y = event.ydata + dy
    node_positions[node] = (new_x, new_y)
    # update scatter data
    # (find index in nodes list and update)
    idx = nodes.index(node)
    offsets = scat.get_offsets()
    offsets[idx] = [new_x, new_y]
    scat.set_offsets(offsets)
    update_edge_positions()


def on_scroll(event):
    base_scale = 1.1
    # get the current x and y limits
    cur_xlim = ax.get_xlim()
    cur_ylim = ax.get_ylim()
    xdata = event.xdata
    ydata = event.ydata
    if xdata is None or ydata is None:
        return
    if event.button == 'up':
        # zoom in
        scale_factor = 1 / base_scale
    elif event.button == 'down':
        # zoom out
        scale_factor = base_scale
    else:
        # no need to re-draw the canvas if it's not a scroll event.
        scale_factor = 1
    # set new limits
    new_width = (cur_xlim[1] - cur_xlim[0]) * scale_factor
    new_height = (cur_ylim[1] - cur_ylim[0]) * scale_factor
    relx = (cur_xlim[1] - xdata) / (cur_xlim[1] - cur_xlim[0])
    rely = (cur_ylim[1] - ydata) / (cur_ylim[1] - cur_ylim[0])
    ax.set_xlim([xdata - new_width * (1 - relx), xdata + new_width * relx])
    ax.set_ylim([ydata - new_height * (1 - rely), ydata + new_height * rely])
    fig.canvas.draw_idle()


# Connect event handlers
fig.canvas.mpl_connect("pick_event", on_pick)
fig.canvas.mpl_connect("button_release_event", on_release)
fig.canvas.mpl_connect("motion_notify_event", on_motion)
fig.canvas.mpl_connect("scroll_event", on_scroll)


# Reset button:
axreset = plt.axes([0.5, 0.01, 0.05, 0.05])  # [left, bottom, width, height]
button_reset = Button(axreset, "Reset Positions")


def reset_positions(event):
    global node_positions
    node_positions = orig_positions.copy()
    update_node_positions()
    update_edge_positions()


button_reset.on_clicked(reset_positions)


axreset_zoom = plt.axes([0.5, 0.07, 0.05, 0.05])
button_reset_zoom = Button(axreset_zoom, "Reset Zoom")


def reset_zoom(event):
    ax.set_xlim(0, n-1)
    ax.set_ylim(-0.7, n-0.7)
    update_marker_sizes()
    fig.canvas.draw_idle()


button_reset_zoom.on_clicked(reset_zoom)


plt.show()
