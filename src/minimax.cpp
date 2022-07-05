#include "minimax.h"
#include <iostream>

Minimax::Minimax() {
    // do nothing
};

Point Minimax::getHead(Round data) {
    return data.you.head;
};

Point Minimax::getNeck(Round data) {
    return data.you.body[1];
};

string Minimax::minimax(json data) {
    Point head = this->getHead((Round) data);
    Point neck = this->getNeck((Round) data);

    cout << "sup";
    cout << "\n\n";
    // printf("x:%d, y:%d", head.x, head.y);

    return "right";
};
