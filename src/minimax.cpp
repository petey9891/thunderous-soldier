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

string Minimax::minimax(Board board) {
    Point head = this->getHead(board.snakes[0]);
    Point neck = this->getNeck(board.snakes[0]);

    cout << head.x << " " << head.y << endl;
    cout << neck.x << " " << neck.y << endl;

    return "right";
};
