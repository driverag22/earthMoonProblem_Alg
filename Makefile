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
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/biplanarTester.cpp
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

# Run the Python script
drawPart:
	python $(PYTHON_DIR)/visualize.py $(filter-out $@,$(MAKECMDGOALS))

plane:
	python $(PYTHON_DIR)/visualize_plane.py $(filter-out $@,$(MAKECMDGOALS))

draw:
	python $(PYTHON_DIR)/drawGraph.py $(filter-out $@,$(MAKECMDGOALS))

# Clean compiled files
clean:
	rm -rf $(BIN_DIR) $(SRC_DIR)/*.o

# Ensure data directory exists before running the program
prepare_data:
	mkdir -p $(DATA_DIR)

