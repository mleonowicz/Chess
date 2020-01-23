#ifndef rook
#define rook

#include <stdbool.h>
#include "../gameManager.h"

extern int rookMoved[2][2];

bool legalMoveRook(Position, Position);

#endif