#ifndef knight
#define knight

#include <stdbool.h>
#include "../gameManager.h"

extern bool whiteKingMoved;
extern bool blackKingMoved;

extern Position whiteKing;
extern Position blackKing;

bool legalMoveKnight(Position, Position);

#endif