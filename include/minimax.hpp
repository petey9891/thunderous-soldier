#pragma once

#include "battlesnake.hpp"

using namespace std;
using namespace nlohmann;

class Minimax {

public:
    Minimax();

public:
    string minimax(Board board); 

private:
    Point getHead(Snake snake);
    Point getNeck(Snake snake);
};