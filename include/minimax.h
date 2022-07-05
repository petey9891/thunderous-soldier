#pragma once

#include <string.h>

#include "json.hpp"
#include "Battlesnake.h"

using namespace std;
using namespace nlohmann;

class Minimax {

public:
    Minimax();

public:
    string minimax(json data); 

private:
    Point getHead(Round data);
    Point getNeck(Round data);
};