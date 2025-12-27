# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -w \
	-Icpp/glucose \
	-Icpp/glucose/core \
	-Icpp/glucose/mtl \
	-Icpp/glucose/simp
LDFLAGS = -lboost_graph -lz  # Boost Graph Library

# Directories
SRC_DIR = cpp
BIN_DIR = bin
DATA_DIR = data
PYTHON_DIR = python

# Executable name
TARGET = $(BIN_DIR)/program

# Source files
SRCS = cpp/main.cpp \
       cpp/candidateBuilder.cpp \
       cpp/biplanarTester.cpp \
       cpp/biplanarSAT.cpp \
       cpp/helperFunctions.cpp \
       cpp/glucose/core/Solver.cc \
       cpp/glucose/core/lcm.cc \
       cpp/glucose/parallel/ClausesBuffer.cc \
       cpp/glucose/parallel/MultiSolvers.cc \
       cpp/glucose/parallel/ParallelSolver.cc \
       cpp/glucose/parallel/SharedCompanion.cc \
       cpp/glucose/parallel/SolverCompanion.cc \
       cpp/glucose/parallel/SolverConfiguration.cc \
       cpp/glucose/simp/SimpSolver.cc \
       cpp/glucose/utils/Options.cc \
       cpp/glucose/utils/System.cc

# Default rule: Compile everything
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)  # Ensure bin/ exists
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Run the compiled program
run: all
	./$(TARGET)

# Run the Python script to draw the two partitions
drawPart:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/partitionsColored.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw the two partitions separately as plane graphs
plane:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/planePartitions.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw a graph
draw:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/drawGraph.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw a plane graph
drawPlane:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/drawPlaneGraph.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw the two partitions with colors in same blow-up colored the same.
drawPartBlowUp:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/partitionsColoredBlowUp.py $(filter-out $@,$(MAKECMDGOALS))
	
# Run the Python script to draw a graph with colors in same blow-up colored the same.
drawBlowUp:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/vis/drawGraphBlowUp.py $(filter-out $@,$(MAKECMDGOALS))

# Run the ILP solver
ilp:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/ilp/biplanarILP.py $(filter-out $@,$(MAKECMDGOALS))

# Run the SAT solver
sat:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/sat/biplanarSAT.py $(filter-out $@,$(MAKECMDGOALS))

# Run the SAT solver for blown-up graphs
sat2:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/sat/biplanarBlowupSAT.py $(filter-out $@,$(MAKECMDGOALS))

satC:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/sat/blowUpExtendedSAT.py $(filter-out $@,$(MAKECMDGOALS))

# Clean compiled files
clean:
	rm -rf $(BIN_DIR) $(SRC_DIR)/*.o
