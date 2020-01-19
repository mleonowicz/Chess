#include "pawn.h"
#include <stdio.h>

bool legalMovePawn(Position from, Position to, bool attacking) {
    bool firstMove = false;

    if (from.x == to.x && !attacking) { // moving forward
        int offset = from.y - to.y;

        if (turn == 1) {
            if (from.y == 1)
                firstMove = true;

            return (offset == -1 || (firstMove && offset == -2));
                
        }
        else {
            if (from.y == 6)
                firstMove = true;

            return (offset == 1 || (firstMove && offset == 2));
        }
    }
    
    int offset = from.x - to.x;

    if (attacking && (offset == -1 || offset == 1)) {
        int offset = from.y - to.y;

        if (turn == 1) 
            return (offset == -1);
        else 
            return (offset == 1);
    }
    return false;
}   