#include "minimax.hpp"
#include <iostream>

Direction Minimax::minimax(const Board& board, const Snake& player) {
    this->m_possibleMoves = PossibleMoves();
  
    this->checkNeck(player);
    this->checkBoundaries(board, player);

    // TODO: Step 2 - Don't hit yourself.
    // Use information in gameState to prevent your Battlesnake from colliding with itself.
    // const mybody = gameState.you.body

    // TODO: Step 3 - Don't collide with others.
    // Use information in gameState to prevent your Battlesnake from colliding with others.

    // TODO: Step 4 - Find food.
    // Use information in gameState to seek out and find food.

    return this->move();
};

Direction Minimax::move() const {
    if (this->m_possibleMoves.up) {
        return Direction::up;
    } else if (this->m_possibleMoves.down) {
        return Direction::down;
    } else if (this->m_possibleMoves.left) {
        return Direction::left;
    } else if (this->m_possibleMoves.right) {
        return Direction::right;
    } else {
      return Direction::right;
    }
}

Point Minimax::getHead(Snake snake) {
    return snake.head;
};

Point Minimax::getNeck(Snake snake) {
    return snake.body[1];
};

// Don't move back on your own neck
void Minimax::checkNeck(const Snake& player) {
    Point head = this->getHead(player);
    Point neck = this->getNeck(player);
    
    if (neck.x < head.x) {
        this->m_possibleMoves.left = false;
    } else if (neck.x > head.x) {
        this->m_possibleMoves.right = false;
    } else if (neck.y < head.y) {
        this->m_possibleMoves.down = false;
    } else if (neck.y > head.y) {
        this->m_possibleMoves.up = false;
    }

    this->logPossibleMoves("checkNeck");
}

void Minimax::checkBoundaries(const Board& board, const Snake& player) {
    // std::cout << " height: " << board.height << " width: " << board.width << " x: " << player.head.x << " y: " << player.head.y << std::endl;
    if (player.head.x == 0) {
        this->m_possibleMoves.left = false;
    } 
    if (player.head.x == board.width - 1) {
        this->m_possibleMoves.right = false;
    }
    if (player.head.y == 0) {
        this->m_possibleMoves.down = false;
    }
    if (player.head.y  == board.height - 1) { 
        this->m_possibleMoves.up = false;
    }
    
    this->logPossibleMoves("checkBoundaries");
}

void Minimax::logPossibleMoves(const std::string location) const {
    printf("[PossibleMoves:%s]: up: %d - down: %d - left: %d - right: %d\n", 
        location.c_str(), 
        this->m_possibleMoves.up, 
        this->m_possibleMoves.down, 
        this->m_possibleMoves.left, 
        this->m_possibleMoves.right
    );
}

    