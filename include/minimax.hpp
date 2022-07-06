#pragma once

#include "battlesnake.hpp"
class Minimax {
private:
    struct PossibleMoves {
        bool up = true;
        bool down = true;
        bool left = true;
        bool right = true;
    } m_possibleMoves;


public:
    Minimax() = default;

public:
    Direction minimax(const Board& board, const Snake& player); 

private:
    Point getHead(Snake snake);
    Point getNeck(Snake snake);

    void checkBoundaries(const Board& board, const Snake& player);
};