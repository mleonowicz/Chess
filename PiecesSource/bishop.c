#include "bishop.h"
#include <stdio.h>

bool legalMoveBishop(Position from, Position to) {
    int offsetX = to.x - from.x;
    int offsetY = to.y - from.y;

    if ((absolute(offsetX) - absolute(offsetY)) != 0) 
        return false;
    
    bool foundPiece = false;;
        
    int x = offsetX > 0 ? 1 : -1;// sign of xMovement
    int y = offsetY > 0 ? 1 : -1; // sign of xMovement

    int count = absolute(offsetX);

    from.x += x;
    from.y += y;
    count--;

    while(count-- > 0) {
        if (pieces[from.x][from.y] != 0)
            return false;
        from.x += x;
        from.y += y;
    }
    
    return true;
}   