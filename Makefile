# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lboost_graph  # Boost Graph Library

# Directories
SRC_DIR = cpp
BIN_DIR = bin
DATA_DIR = data
PYTHON_DIR = python

# Executable name
TARGET = $(BIN_DIR)/program

# Source and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/candidateBuilder.cpp $(SRC_DIR)/biplanarTester.cpp $(SRC_DIR)/helperFunctions.cpp
OBJS = $(SRCS:.cpp=.o)

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

# Run the ILP solver
ilp:
	PYTHONPATH=$(PYTHON_DIR) python $(PYTHON_DIR)/ilp/biplanarSAT.py $(filter-out $@,$(MAKECMDGOALS))

# Clean compiled files
clean:
	rm -rf $(BIN_DIR) $(SRC_DIR)/*.o
