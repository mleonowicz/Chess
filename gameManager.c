#include <stdio.h>
#include "gameManager.h"
#include "uiManager.h"
#include "PiecesSource/pawn.h"
#include "PiecesSource/bishop.h"
#include "PiecesSource/knight.h"
#include "PiecesSource/rook.h"
#include "PiecesSource/king.h"

int pieces[8][8] = { 
                {4, 1, 0, 0, 0, 0, -1, -4}, 
                {3, 1, 0, 0, 0, 0, -1, -3},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {5, 1, 0, 0, 0, 0, -1, -5},
                {6, 1, 0, 0, 0, 0, -1, -6},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {3, 1, 0, 0, 0, 0, -1, -3}, 
                {4, 1, 0, 0, 0, 0, -1, -4} 
                };

int turn = 1;
int selectedPiece = 0;
Position selectedPiecePos;

void initialize() {
    whiteKing.x = 4;
    whiteKing.y = 0;

    blackKing.x = 4;
    blackKing.y = 7;
}

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

bool sameSign(int n, int m) {
    return n > 0 && m > 0 || n < 0 && m < 0;
}

bool opposeSign(int n, int m) {
    return (n > 0 && m < 0) || (n < 0 && m > 0);
}

bool explode(Position tile) {
    bool lost = false;

    for (int x = tile.x - 1; x <= tile.x + 1; x++) {
        for (int y = tile.y - 1; y <= tile.y + 1; y++) {
            if (x >= 0 && y >= 0 && x <= 7 && y <= 7) {
                if (absolute(pieces[x][y]) == 6)
                    lost = true;
                
                pieces[x][y] = 0;
                removePieceImage(x, y);
            }
        }
    }

    return lost;
}

bool isTileAttacked(Position tile, int t) { // t for enemy piece
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (sameSign(pieces[x][y], t)) {
                Position pos;
                pos.x = x;
                pos.y = y;

                switch (pieces[x][y] * t) {
                    case 1:
                        if (legalMovePawn(pos, tile, true, t))
                            return true;
                    break;
                    case 2:
                        if (legalMoveBishop(pos, tile))
                            return true;
                    break;
                    case 3:
                        if (legalMoveKnight(pos, tile))
                            return true;
                    break;
                    case 4:
                        if (legalMoveRook(pos, tile))
                            return true;
                    break;
                    case 5:
                        if (legalMoveBishop(pos, tile) || legalMoveRook(pos, tile))
                            return true;
                    break;
                }
            }  
        }
    }
    return false;  
}

void castle(Position k, Position r) {
    Position newKing;
    Position newRook;

    newKing.y = k.y;
    newRook.y = r.y;

    int x = (k.x - r.x) > 0 ? -1 : 1;
    newKing.x = k.x + x + x;
    newRook.x = r.x - x - x;

    if (newRook.x == newKing.x)
        newKing.x += x;

    placePiece(r, newRook);
    placePiece(k, newKing);
    changeKingPos(turn, newKing);
    
    if (!isTileAttacked(turn == 1 ? whiteKing : blackKing, turn * -1)) { // if castle would put king in a check
        placePiece(k, newKing);
        changeKingPos(turn, newKing);
        kingMoved(turn); // changing kings first move

        placePiece(r, newRook);
        movedRook(newRook);

        turn *= -1;
    }
    else {
        placePiece(newRook, r);
        placePiece(newKing, k);
        changeKingPos(turn, k);
    }
}

bool checkCastle(int newPiece, Position pos) {
    Position kingPos;
    Position rookPos;

    if (absolute(selectedPiece) == 4 && absolute(newPiece) == 6) {
        kingPos = pos;
        rookPos = selectedPiecePos;
    }
    else if (absolute(newPiece) == 4 && absolute(selectedPiece) == 6) {
        kingPos = selectedPiecePos;
        rookPos = pos;
    }
    else 
        return false;

    int x = (kingPos.x - rookPos.x) > 0 ? -1 : 1;
    int y = kingPos.y;

    int newX = kingPos.x + x;

    while (newX != rookPos.x) { // checking if there are pieces between rook and king
        if (pieces[newX][y] != 0)
            return false;

        newX += x;
    }

    if (!checkIfRookMoved(rookPos)) {
        if (turn == 1) {
            if (!whiteKingMoved) {
                castle(kingPos, rookPos);
                return true;
            }
        }
        else {
            if (!blackKingMoved) {
                castle(kingPos, rookPos);
                return true;
            }
        }
    }

    return false;
}

void handleClick(Position pos) {
    int piece = pieces[pos.x][pos.y];
    if (sameSign(piece, turn)) {

        if(checkCastle(piece, pos))
            return;

        setStyleSelected(selectedPiecePos.x, selectedPiecePos.y, false);
        setStyleSelected(pos.x, pos.y, true);
        
        selectedPiecePos = pos;
        selectedPiece = piece;
    }
    else if (selectedPiece != 0) 
    {
        bool correctMove = false;

        switch(absolute(selectedPiece)) {
            case 1:
                if (legalMovePawn(selectedPiecePos, pos, attacking(pos), turn))
                    correctMove = true;
            break;
            case 2:
                if (legalMoveBishop(selectedPiecePos, pos))
                    correctMove = true;               
            break;
            case 3:
                if (legalMoveKnight(selectedPiecePos, pos))
                    correctMove = true;
            break;
            case 4:
                if (legalMoveRook(selectedPiecePos, pos)) {
                    // movedRook(selectedPiecePos);
                    correctMove = true;
                }
            break;
            case 5: 
                if (legalMoveRook(selectedPiecePos, pos) || legalMoveBishop(selectedPiecePos, pos)) // queen movement is same as rook and bishop
                    correctMove = true;
            break;
            case 6:
                if (legalMoveKing(selectedPiecePos, pos)) {
                    // movedKing(turn, pos);
                    correctMove = true;
                }
            break;
        }

        if (correctMove) { // checkin if move would put king in a check
            bool illegalMove = false;

            int tempPiece = pieces[pos.x][pos.y];
            movePiece(selectedPiecePos, pos);

            if (absolute(selectedPiece) == 6)
                changeKingPos(turn, pos);   

            if (turn == 1) {
                if (isTileAttacked(whiteKing, -1))
                    illegalMove = true;
            }
            else {
                if (isTileAttacked(blackKing, 1))
                    illegalMove = true;
            }

            movePiece(pos, selectedPiecePos);
            pieces[pos.x][pos.y] = tempPiece;

            if (illegalMove) {
                if (absolute(selectedPiece) == 6)
                    changeKingPos(turn, selectedPiecePos);

                return;
            }

            if (absolute(selectedPiece) == 6)
                kingMoved(turn);
            else if (absolute(selectedPiece) == 4)
                movedRook(selectedPiecePos);

            if (attacking(pos)) {
                placePiece(selectedPiecePos, pos);
                if (explode(pos))   {
                    printf("lost\n");
                    fflush(stdout);
                }
            }                
            else 
                placePiece(selectedPiecePos, pos);

            turn *= -1;
        }

        printBoard();
    }
}

void placePiece(Position from, Position to) {
    movePiece(from, to);
    removePieceImage(from.x, from.y);
    placePieceImage(pieces[to.x][to.y], to.x, to.y);

    setStyleSelected(from.x, from.y, false);
    selectedPiece = 0;
}

void movePiece(Position from, Position to) {
    pieces[to.x][to.y] = pieces[from.x][from.y];
    pieces[from.x][from.y] = 0;
}