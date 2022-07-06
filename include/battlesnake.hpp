#pragma once

#include <vector>
#include "json.hpp"

enum class Direction {
    up,
    left,
    right,
    down
};

// Used for positions on board, to make it easier to switch integer type.
using Index = int;

struct Point {
    Point() {};
    Point (const Index x, const Index y) : x(x), y(y) {}

    Index x = 0;
    Index y = 0;
};

using Points = std::vector<Point>;

struct Customizations {
    std::string color;
    std::string head;
    std::string tail;
};

struct Snake {
    int health;
    int length;
    Point head;
    Points body;
    std::string id;
    std::string name;
};

using Snakes = std::vector<Snake>;

struct Board {
    int height;
    int width;
    Points food;
    Points hazards;
    Snakes snakes;
};
