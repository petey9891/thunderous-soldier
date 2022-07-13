SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

EXE := $(BIN_DIR)/main

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
COMPILED_OBJECTS := $(wildcard $(OBJ_DIR)/*.o)

TEST_SOURCES := $(wildcard $(SRC_DIR)/minimax.cpp)
TEST_OBJECTS := $(patsubst $(SRC_DIR)/minimax.cpp, $(OBJ_DIR)/minimax.o, $(TEST_SOURCES))

CPPFLAGS :=-Iinclude -MMD -MP
CXXFLAGS :=-std=c++17 -O3 
# CXXFLAGS :=-std=c++17 -O0 -glldb
# CXXFLAGS :=-std=c++17 -O0
LDLIBS :=-lpthread

.PHONY: all tests clean

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

tests:
	+$(MAKE) $(TEST_OBJECTS)
	+$(MAKE) -C tests/

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command
	

-include $(OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)