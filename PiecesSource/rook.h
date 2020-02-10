#ifndef rook
#define rook

#include <stdbool.h>
#include "../gameManager.h"

void movedRook(Position);
bool legalMoveRook(int[8][8], Position, Position);
bool checkIfRookMoved(Position);
void resetRooks();

#endif