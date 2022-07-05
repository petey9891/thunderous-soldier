#pragma once

#include <string.h>
#include <vector>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

struct Point {
    int x;
    int y; 
};

struct Board {
    int height;
    int width;
    vector<Point> food;
    vector<Point> hazards;
    vector<Point> snakes;
};

struct You {
    int health;
    int length;
    Point head;
    vector<Point> body;
    // string id;
};

struct Round: public json {
    int turn;
    Board board;
    You you;
};
