#include "king.h"
#include <stdio.h>

bool whiteKingMoved = false;
bool blackKingMoved = false;

Position whiteKing;
Position blackKing;

void kingMoved(int t) {
    if (t == 1) 
        whiteKingMoved = true;
    else
        blackKingMoved = true;
}

void changeKingPos(int t, Position to) { // t stands for turn, 1 - white, -1 black
    if (t == 1) {
        whiteKing.x = to.x;
        whiteKing.y = to.y;
    }
    else {
        blackKing.x = to.x;
        blackKing.y = to.y;
    }
} 

bool legalMoveKing(Position from, Position to) {
    int offsetX = from.x - to.x;
    int offsetY = from.y - to.y;

    if (absolute(offsetY) > 1 || absolute(offsetX) > 1)
        return false;

    return true;
}   