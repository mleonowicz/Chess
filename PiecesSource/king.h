#ifndef king
#define king

#include <stdbool.h>
#include "../gameManager.h"

extern bool whiteKingMoved;
extern bool blackKingMoved;

void movedKing(int, Position);
bool legalMoveKing(Position, Position);

#endif