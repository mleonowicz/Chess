#include "king.h"
#include <stdio.h>

bool whiteKingMoved = false;
bool blackKingMoved = false;

bool legalMoveKing(Position from, Position to) {
    int offsetX = from.x - to.x;
    int offsetY = from.y - to.y;

    if (absolute(offsetY) > 1 || absolute(offsetX) > 1)
        return false;

    if (turn == 1) 
        whiteKingMoved = true;
    else 
        blackKingMoved = true;

    return true;
}   