#pragma once

#include <vector>
#include "json.hpp"

enum class Direction {
    up,
    left,
    right,
    down
};

inline std::ostream & operator<<(std::ostream &os, const Direction& d)
{
    switch(d) {
    case Direction::up:
        os << "up";
        break;
    case Direction::left:
        os << "left";
        break;
    case Direction::down:
        os << "down";
        break;
    case Direction::right:
        os << "right";
        break;
    }
    return os;
}

// Used for positions on board, to make it easier to switch integer type.
using Index = int;

struct Point {
    Point() {};
    Point (const Index t_x, const Index t_y) : x(t_x), y(t_y) {}

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
