# Earth-Moon problem Algorithms
Algorithms for work on the earth-moon problem.
Current two different algorithms (sharing helper functions):
 - biplanarTester: backtracking algorithm to determine if a given graph is biplanar.
 - candidateBuilder: brute-force algorithm that searches for thickness-2 graphs of a chosen 
 chromatic number.
Both inherit functions form the module helperFunctions.

## Directory structure
```
.
├── cpp/
│   ├── helperFunctions.h
│   ├── helperFunctions.cpp
│   ├── biplanarTester.h
│   ├── biplanarTester.cpp
│   ├── candidateBuilder.h
│   ├── candidateBuilder.cpp
│   └── main.cpp
├── data/
├── python/
│   ├── requirements.txt
│   ├── visualize.py
│   ├── visualize_plane.py
│   └── drawGraph.py
└── Makefile
```

### cpp
The `cpp` code has the actual algorithm implementations.
 - `helperFunctions.cpp` includes functions to construct graphs, test planarity, and some other helpful things.
 - `biplanarTester.cpp` has the implementation of the backtracking recursive code to determine if a given 
 graph is biplanar by building different edge partitions.
 - `candidateBuilder.cpp` has the implementation of the code to build candidate graphs of chromatic number 
 at least 9 and/or 10 with a certain number of vertices.
 - `*.h` are simply the header file, they contain function prototypes and datastructures.
 - `main.cpp` is simply where the code is run from. Here you choose what graph to test biplanarity for, or what 
 parameters to use when searching for biplanarity graphs of high chromatic number.

Every function is commented, so if something is unclear consider looking through the codebase.

### python
To install the requirements run `pip install -r python/requirements.txt`

The python code is only used to draw the graphs.
  - `drawGraph.py` draws the graph from the file passed as an argument. Example files can be found under `data`
  - `visualize.py` draws the graph in `~/data/partitions.txt` with the two partitions colored red and blue.
  - `visualize_plane.py` draws the partitions of the graph in `data/partitions.txt` as plane graphs (if possible).

### data
The `data` directory is where we store different graphs and partitions.
The `cpp` code writes to the directory, and the `python` code reads from it.

The format to describe a graph/two partitions is clear from reading the `.txt` files in the directory.

## Running the project

To run the code first run 
```
make clean 
make 
make run
```

### Biplanar test
The graph you want to check biplanarity of can be described in `cpp/main.cpp`, check `helperFunctions.h` 
to see the different graphs that can be used (path, cycle and complete) and operations that can be applied on them (removeVertex, strongProduct).

You can also manually define the graph.

Once the edge-set is defined, simply call `isBiplanar(edges, 0, g1, g2);` (also from `main.cpp`), where `g1,g2` are empty graphs on n vertices.
To avoid symmetric case, you can consider adding the first edge to `g1` and instead calling `isBiplanar(edges, 1, g1, g2);`.

If a partition is found, it will be ouput `data/partitions.txt`.

### Candidate builder

Simply call `computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts);` from `main.cpp` where `numVertLow, numVertHigh, numAttempts`
are respectively the lower number of vertices to find a graph for, the highest number of vertices, and the number of attempts per each number of vertices.

If finds a biplanar graph of chromatic number \geq 9 or 10, the graph and the two partitions are saved at:
   - `data/candidates{x}/graph_{i}_{n}.txt`
where `{x}` is 9 or 10 (chromatic number), `{i}` current attempt, and `{n}` number of vertices of the graph.

### Graph visualizing
Drawing the graph (if biplanar) can be done via `make drawPart` (to get the graph with one partition in red, the other in blue) 
and `make plane` to get the plane drawings of both partitions.
By default it will plot the partitions from `data/partitions.txt`, but you can choose a different one by passing 
the path as an argument (e.g. `make draw data/p3_k4_partitioned.txt`).
For exampel, to plot a candidate graph, you'd have to call `make drawPart data/candidates10/graph_10_50.txt` (or with `make plane`).

To simply draw an arbitrary graph (not partitioned) use `make draw` and give the (relative) file path as input when asked
(e.g. `c5_44443.txt`), or pass it directly (e.g. `make draw c5_44443.txt`).

