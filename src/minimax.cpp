#include "minimax.hpp"
#include <iostream>

Direction Minimax::minimax(const Board& board, const Snake& player) {
    Point head = this->getHead(board.snakes[0]);
    Point neck = this->getNeck(board.snakes[0]);
    std::cout << "Start up: " << this->m_possibleMoves.up << " down: " << this->m_possibleMoves.down << " left: " << this->m_possibleMoves.left << " right: " << this->m_possibleMoves.right << std::endl;
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

    std::cout << "Post-check neck up: " << this->m_possibleMoves.up << " down: " << this->m_possibleMoves.down << " left: " << this->m_possibleMoves.left << " right: " << this->m_possibleMoves.right << std::endl;


    // TODO: Step 1 - Don't hit walls.
    // Use information in gameState to prevent your Battlesnake from moving beyond the boundaries of the board.
    this->checkBoundaries(board, player);

    // TODO: Step 2 - Don't hit yourself.
    // Use information in gameState to prevent your Battlesnake from colliding with itself.
    // const mybody = gameState.you.body

    // TODO: Step 3 - Don't collide with others.
    // Use information in gameState to prevent your Battlesnake from colliding with others.

    // TODO: Step 4 - Find food.
    // Use information in gameState to seek out and find food.

    std::cout << "Post-check boundaries up: " << this->m_possibleMoves.up << " down: " << this->m_possibleMoves.down << " left: " << this->m_possibleMoves.left << " right: " << this->m_possibleMoves.right << std::endl;
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

Point Minimax::getHead(Snake snake) {
    return snake.head;
};

Point Minimax::getNeck(Snake snake) {
    return snake.body[1];
};


void Minimax::checkBoundaries(const Board& board, const Snake& player) {
    std::cout << " height: " << board.height << " width: " << board.width << " x: " << player.head.x << " y: " << player.head.y << std::endl;
    if (player.head.x == 0) {
        this->m_possibleMoves.left = false;
    } else if (player.head.x == board.width - 1) {
        this->m_possibleMoves.right = false;
    } else if (player.head.y == 0) {
        this->m_possibleMoves.down = false;
    } else if (player.head.y  == board.height - 1) { 
        this->m_possibleMoves.up = false;
    }
}