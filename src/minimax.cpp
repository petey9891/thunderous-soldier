#include "minimax.hpp"
#include <iostream>

Minimax::Minimax() {
    // do nothing
};

Point Minimax::getHead(Snake snake) {
    return snake.head;
};

Point Minimax::getNeck(Snake snake) {
    return snake.body[1];
};

Direction Minimax::minimax(Board board) {
    Point head = this->getHead(board.snakes[0]);
    Point neck = this->getNeck(board.snakes[0]);

    // Don't move back on your own neck
    if (neck.x < head.x) {
        this->m_possibleMoves.left = false;
    } else if (neck.x > head.x) {
        this->m_possibleMoves.right = false;
    } else if (neck.y < head.y) {
        this->m_possibleMoves.down = false;
    } else if (neck.y > head.y) {
        this->m_possibleMoves.up = false;
    }

    if (this->m_possibleMoves.up) {
        return Direction::up;
    } else if (this->m_possibleMoves.down) {
        return Direction::down;
    } else if (this->m_possibleMoves.left) {
        return Direction::left;
    } else if (this->m_possibleMoves.right) {
        return Direction::right;
    }

    // Pick a random direction
    return Direction::right;
};
