# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lboost_graph -lgurobi_c++ -lgurobi1201

# Directories
SRC_DIR = cpp
BIN_DIR = bin
DATA_DIR = data
PYTHON_DIR = python

# Executable name
TARGET = $(BIN_DIR)/program

# Source and object files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/candidateBuilder.cpp $(SRC_DIR)/biplanarTester.cpp $(SRC_DIR)/SATBiplanarTester.cpp $(SRC_DIR)/helperFunctions.cpp
OBJS = $(SRCS:.cpp=.o)

# Default rule: Compile everything
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)  # Ensure bin/ exists
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS) -I/opt/gurobi1201/linux64/include -L/opt/gurobi1201/linux64/lib

# Run the compiled program
run: all
	./$(TARGET)

# Run the Python script to draw the two partitions
drawPart:
	python $(PYTHON_DIR)/visualize.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw the two partitions separately as plane graphs
plane:
	python $(PYTHON_DIR)/visualize_plane.py $(filter-out $@,$(MAKECMDGOALS))

# Run the Python script to draw a graph
draw:
	python $(PYTHON_DIR)/drawGraph.py $(filter-out $@,$(MAKECMDGOALS))

# Clean compiled files
clean:
	rm -rf $(BIN_DIR) $(SRC_DIR)/*.o
