#include <iostream>
#include <fstream>
#include <string>

#include "../include/minimax.hpp"
#include "tests.hpp"

using namespace std;
using namespace nlohmann;
using namespace Battlesnake;

bool TestBuildWorldMap() {
    ifstream i("exampleMove.json");
    json data;
    i >> data;

    Board board = data["board"].get<Board>();

    Minimax::Minimax paranoid(11, 11);
    Minimax::Grid grid = paranoid.buildWorldMap(board);

    // Player snake
    if (grid[5][10] != BoardElement::head) { EVAL_LOG(); return false; }
    if (grid[5][9]  != BoardElement::body) { EVAL_LOG(); return false; }
    if (grid[5][8]  != BoardElement::body) { EVAL_LOG(); return false; }
    if (grid[5][7]  != BoardElement::tail) { EVAL_LOG(); return false; }
    
    // Enemy
    if (grid[1][7]  != BoardElement::head) { EVAL_LOG(); return false; }
    if (grid[1][6]  != BoardElement::tail) { EVAL_LOG(); return false; }

    // Food
    if (grid[0][6]  != BoardElement::food) { EVAL_LOG(); return false; }
    if (grid[5][5]  != BoardElement::food) { EVAL_LOG(); return false; }
    if (grid[7][8]  != BoardElement::food) { EVAL_LOG(); return false; }

    // Random
    if (grid[4][2]  != BoardElement::empty) { EVAL_LOG(); return false; }
    if (grid[3][5]  != BoardElement::empty) { EVAL_LOG(); return false; }
    if (grid[7][1]  != BoardElement::empty) { EVAL_LOG(); return false; }
    if (grid[3][7]  != BoardElement::empty) { EVAL_LOG(); return false; }
    if (grid[7][9]  != BoardElement::empty) { EVAL_LOG(); return false; }
    if (grid[5][0]  != BoardElement::empty) { EVAL_LOG(); return false; }

    return true;
}

bool TestDistanceTo() {
    Point src1(5, 10);
    Point src2(7, 8);

    Minimax::Minimax paranoid;

    if (paranoid.distanceTo(src1, src2) != 4) { EVAL_LOG(); return false; }

    return true;
}

void eval(std::string name, bool (*func)()) {
    printf("[%s]", name.c_str());
    printf(" %s\n", (*func)() ? "passed" : "failed");
}


int main(void) {    
    eval("TestBuildWorldMap", &TestBuildWorldMap);
    eval("TestDistanceTo", &TestDistanceTo);

    return 0;
}