#ifndef king
#define king

#include <stdbool.h>
#include "../gameManager.h"

extern bool whiteKingMoved;
extern bool blackKingMoved;

void kingMoved(int);
void changeKingPos(int, Position);
bool legalMoveKing(Position, Position);

#endif