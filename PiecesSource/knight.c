#include "knight.h"
#include <stdio.h>

bool legalMoveKnight(Position from, Position to) {

    int offsetX = absolute(from.x - to.x);
    int offsetY = absolute(from.y - to.y);

    if ((offsetX == 2 && offsetY == 1) || (offsetX == 1 && offsetY == 2))
        return true;

    return false;
}   