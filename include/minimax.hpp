#pragma once

#include "battlesnake.hpp"
class Minimax {

public:
    Minimax();

public:
    Direction minimax(Board board); 

private:
    Point getHead(Snake snake);
    Point getNeck(Snake snake);

private:
    struct PossibleMoves {
        bool up = true;
        bool down = true;
        bool left = true;
        bool right = true;
    } m_possibleMoves;
};