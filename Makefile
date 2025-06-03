# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Source and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Executable name
TARGET := $(BIN_DIR)/sysmon

# Default rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(TARGET)

.PHONY: all clean run
