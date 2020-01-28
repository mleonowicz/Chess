#include "king.h"
#include <stdio.h>

bool whiteKingMoved = false;
bool blackKingMoved = false;

Position whiteKing;
Position blackKing;

bool legalMoveKing(Position from, Position to) {
    int offsetX = from.x - to.x;
    int offsetY = from.y - to.y;

    if (absolute(offsetY) > 1 || absolute(offsetX) > 1)
        return false;

    if (turn == 1) {
        whiteKingMoved = true;
        whiteKing.x = to.x;
        whiteKing.y = to.y;
    }
    else {
        blackKingMoved = true;
        blackKing.x = to.x;
        blackKing.y = to.y;
    }
    return true;
}   