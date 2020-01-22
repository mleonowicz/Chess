#include "rook.h"
#include <stdio.h>

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

    from.x += x;
    from.y += y;
    count--;

    while (count-- > 0) {
        if (pieces[from.x][from.y] != 0)
            return false;

        from.x += x;
        from.y += y;
    }

    return true;
}   