#include "gameManager.h"

int pieces[8][8] = { {4, 1, 0, 0, 0, 0, -1, -4}, 
                {3, 1, 0, 0, 0, 0, -1, -3},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {5, 1, 0, 0, 0, 0, -1, -5},
                {6, 1, 0, 0, 0, 0, -1, -6},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {3, 1, 0, 0, 0, 0, -1, -3}, 
                {4, 1, 0, 0, 0, 0, -1, -4} };

void movePiece(Position from, Position to) {
    pieces[to.x][to.y] = pieces[from.x][from.y];
    pieces[from.x][from.y] = 0;
}