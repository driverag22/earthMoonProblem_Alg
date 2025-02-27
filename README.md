# Earth-Moon problem Algorithms
Algorithms for work on the earth-moon problem (determining chromatic number of biplanar graphs).
The implementation uses backtracking to determine if a given graph is biplanar.

## Directory structure
```
.
├── cpp/
│   ├── biplanarTester.h
│   ├── biplanarTester.cpp
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
The `cpp` code has the actual backtracking algorithm.
 - `biplanarTester.h` is simply the header file, it contains function prototypes and datastructures.
 - `biplanarTester.cpp` has the implementation, including functions to construct graphs, and the backtracking recursive code.
 - `main.cpp` is simply where the code is run from. Here you choose what graph to test biplanarity for.

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
The graph you want to check biplanarity of can be described in `cpp/main.cpp`, check `biplanarityTester.h` 
to see the different graphs that can be used (path, cycle and complete) and operations 
that can be applied on them (removeVertex, strongProduct).

You can also manually define the graph.

Drawing the graph (if biplanar) can be done via `make drawPart` (to get the graph with one partition in red, the other in blue) 
and `make plane` to get the plane drawings of both partitions.
To simply draw an arbitrary graph (not partitioned) use `make draw` and give the (relative) file path as input when asked
(e.g. `c5_44443.txt`), or pass it directly (e.g. `make draw c5_44443.txt`).

