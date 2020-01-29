#ifndef rook
#define rook

#include <stdbool.h>
#include "../gameManager.h"

void movedRook(Position);
bool legalMoveRook(Position, Position);
bool checkIfRookMoved(Position);

#endif