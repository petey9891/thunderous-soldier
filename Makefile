SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

EXE := $(BIN_DIR)/main

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
COMPILED_OBJECTS := $(wildcard $(OBJ_DIR)/*.o)

CURRENT_DIRECTORY=$(shell pwd)

CPPFLAGS :=-Iinclude -MMD -MP
CXXFLAGS :=-std=c++17 -O3 
LDLIBS :=-lpthread

.PHONY: all clean

all:
	+make $(EXE)

$(EXE): $(COMPILED_OBJECTS) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command


-include $(OBJECTS:.o=.d)