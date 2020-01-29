#include "rook.h"
#include <stdio.h>

bool rookMoved[2][2] = { 
        {false, false},
        {false, false} 
        };
/*
    0,0 - left white
    0,1 - right white
    1,0 - left black
    1,1 - right black
*/

bool *mapRook(Position from) {
    bool *b = NULL;

    if (from.x == 0) {
        if (from.y == 0)
            b = &rookMoved[0][0];
        else if (from.y == 7)
            b = &rookMoved[0][1];
    }
    else if (from.x == 7) {
        if (from.y == 0)
            b = &rookMoved[0][1];
        else if (from.y == 7)
            b = &rookMoved[1][1];
    }

    return b;
}

void checkIfFirstMove(Position from) {
    if ((from.x != 0 || (from.y != 0 && from.y != 7)) 
    && (from.x != 7 || (from.y != 0 && from.y != 7))) 
        return;

    bool *b = mapRook(from);
    *b = true;
}

bool checkIfRookMoved(Position from) {
    if ((from.x != 0 || (from.y != 0 && from.y != 7)) 
    && (from.x != 7 || (from.y != 0 && from.y != 7))) 
        return true;

    return *mapRook(from);
}

void movedRook(Position from) {
    checkIfFirstMove(from);
}

bool legalMoveRook(Position from, Position to) {
    
    int offsetX = to.x - from.x;
    int offsetY = to.y - from.y;

    if (offsetX != 0 && offsetY != 0)
        return false;
    
    int x, y;

    if (offsetX == 0)
        x = 0;
    else 
        x = offsetX > 0 ? 1 : -1;

    if (offsetY == 0)
        y = 0;
    else 
        y = offsetY > 0 ? 1 : -1;

    int count = absolute(offsetY) > absolute(offsetX) ? absolute(offsetY) : absolute(offsetX);

    int newX = from.x;
    int newY = from.y;

    newX += x;
    newY += y;
    count--;

    while (count-- > 0) {
        if (pieces[newX][newY] != 0)
            return false;

        newX += x;
        newY += y;
    }

    return true;
}   