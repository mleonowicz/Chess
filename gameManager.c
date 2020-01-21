#include <stdio.h>
#include "gameManager.h"
#include "uiManager.h"
#include "PiecesSource/pawn.h"
#include "PiecesSource/bishop.h"

int pieces[8][8] = { {4, 1, 0, 0, 0, 0, -1, -4}, 
                {3, 1, 0, 0, 0, 0, -1, -3},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {5, 1, 0, 0, 0, 0, -1, -5},
                {6, 1, 0, 0, 0, 0, -1, -6},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {3, 1, 0, 0, 0, 0, -1, -3}, 
                {4, 1, 0, 0, 0, 0, -1, -4} };

int turn = 1;
int selectedPiece = 0;
Position selectedPiecePos;

void printBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[j][i] < 0)
                printf("%d ", pieces[j][i]);
            else 
                printf("%d  ", pieces[j][i]);
        }
        putchar('\n');
    }
}

int absolute(int i) {
    if (i < 0)
        i *= -1;

    return i;
}

bool attacking(Position to) {
    return pieces[to.x][to.y] != 0;
}

bool sameSign(int n, int m)
{
    return n > 0 && m > 0 || n < 0 && m < 0;
}

bool opposeSign(int n, int m)
{
    return (n > 0 && m < 0) || (n < 0 && m > 0);
}

void handleClick(Position pos) {
    int piece = pieces[pos.x][pos.y];

    if (sameSign(piece, turn)) {
        selectedPiecePos = pos;
        selectedPiece = piece;
    }
    else if (selectedPiece != 0) 
    {
        switch(absolute(selectedPiece)) {
            case 1:
                if (legalMovePawn(selectedPiecePos, pos, attacking(pos)))
                    makeMove(pos);
            break;
            case 2:
                if (legalMoveBishop(selectedPiecePos, pos))
                    makeMove(pos);                
            break;
            default:
               makeMove(pos);
            break;
        }
    }
}

void makeMove(Position pos) {
    movePiece(selectedPiecePos, pos);
    removePieceImage(selectedPiecePos.x, selectedPiecePos.y);
    placePieceImage(selectedPiece, pos.x, pos.y);
    selectedPiece = 0;

    turn *= -1;
}

void movePiece(Position from, Position to) {
    pieces[to.x][to.y] = pieces[from.x][from.y];
    pieces[from.x][from.y] = 0;
}