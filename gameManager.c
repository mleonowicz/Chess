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

void printBoard(int array[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (array[j][i] < 0)
                printf("%d ", array[j][i]);
            else 
                printf("%d  ", array[j][i]);
        }
        putchar('\n');
    }

    fflush(stdout);
}

bool isKingDead(int array[8][8], int t) { // 1 for white, -1 for black 
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) {
            if (array[i][j] == 6 * t)
                return false;
        }
    
    return true;
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

bool explode(int array[8][8], Position tile) {
    bool lost = false;

    for (int x = tile.x - 1; x <= tile.x + 1; x++) {
        for (int y = tile.y - 1; y <= tile.y + 1; y++) {
            if (x >= 0 && y >= 0 && x <= 7 && y <= 7) {
                if (absolute(array[x][y]) == 6)
                    lost = true;
                
                array[x][y] = 0;

                if (array == pieces)
                    removePieceImage(x, y);
            }
        }
    }

    return lost;
}

bool legalMove(Position from, Position to, int piece, bool attacking, int turn, int array[8][8]) {
    switch(piece) {
        case 1:
            return legalMovePawn(from, to, attacking, turn, array); 
        break;
        case 2:
            return legalMoveBishop(array, from, to);
        break;
        case 3:
            return legalMoveKnight(from, to);
        break;
        case 4:
            return legalMoveRook(array, from, to);
        break;
        case 5:
            return (legalMoveBishop(array, from, to) || legalMoveRook(array, from, to));
        break;
        case 6:
            return legalMoveKing(from, to, attacking);
        break;
    }

    return false;
}

bool isTileAttacked(int array[8][8], Position tile, int t) { // t for enemy piece
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (sameSign(array[x][y], t)) {
                Position pos;
                pos.x = x;
                pos.y = y;

                if (legalMove(pos, tile, array[x][y] * t, true, t, array))
                    return true;
            }  
        }
    }
    return false;  
}

bool moveWouldPutInCheck(int array[8][8], Position from, Position to, int piece) {
    bool illegalMove = false;
    int arrayCopy[8][8];

    memcpy(arrayCopy, array, sizeof(pieces));

    arrayCopy[to.x][to.y] = arrayCopy[from.x][from.y];
    arrayCopy[from.x][from.y] = 0;
        
    if (absolute(piece) == 6)
        changeKingPos(turn, to);   

    if (turn == 1) {
        if (isTileAttacked(arrayCopy, whiteKing, -1))
            illegalMove = true;
    }
    else {
        if (isTileAttacked(arrayCopy, blackKing, 1))
            illegalMove = true;
    }

    if (absolute(piece) == 6)
        changeKingPos(turn, selectedPiecePos);

    return illegalMove;
}

void castle(Position k, Position r) {
    Position newKing;
    Position newRook;

    if (r.x == 4) {
        Position temp = r;
        r = k;
        k = temp;
    } 

    newKing.y = k.y;
    newRook.y = r.y;

    int x = (k.x - r.x) > 0 ? -1 : 1;
    newKing.x = k.x + x + x;
    newRook.x = r.x - x - x;

    if (newRook.x == newKing.x)
        newKing.x += x;

    movedRook(r);

    placePiece(k, newKing);
    placePiece(r, newRook);

    kingMoved(turn);
    changeKingPos(turn, newKing);

    turn *= -1;
}

bool canCastle(int pieceFrom, int pieceTo, Position from, Position to, int t) {
    Position kingPos;
    Position rookPos;

    if (absolute(pieceFrom) == 4 && absolute(pieceTo) == 6) {
        kingPos = to;
        rookPos = from;
    }
    else if (absolute(pieceTo) == 4 && absolute(pieceFrom) == 6) {
        kingPos = from;
        rookPos = to;
    }
    else 
        return false;

    if (checkIfRookMoved(rookPos))
        return false;

    if (t == 1) {
        if (whiteKingMoved)
            return false;
    }
    else if (blackKingMoved)
        return false;

    int x = (kingPos.x - rookPos.x) > 0 ? -1 : 1;
    int y = kingPos.y;

    int newX = kingPos.x + x;

    while (newX != rookPos.x) { // checking if there are pieces between rook and king
        if (pieces[newX][y] != 0)
            return false;

        newX += x;
    }

    Position newKing;
    Position newRook;

    newKing.y = kingPos.y;
    newRook.y = rookPos.y;

    x = (kingPos.x - rookPos.x) > 0 ? -1 : 1;
    newKing.x = kingPos.x + x + x;
    newRook.x = rookPos.x - x - x;

    if (newRook.x == newKing.x)
        newKing.x += x;

    movePiece(kingPos, newKing);
    movePiece(rookPos, newRook);
    changeKingPos(t, newKing);

    bool canCastle = true;

    // if castle would put king in a check
    if (isTileAttacked(pieces, t == 1 ? whiteKing : blackKing, t * -1))
        canCastle = false;

    movePiece(newKing, kingPos);
    movePiece(newRook, rookPos);
    changeKingPos(t, kingPos);

    return canCastle;
}

void removeStyle() {
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++) 
            setStyleSelected(x, y, false);
}

void drawMoves(Position pos) {
    int piece = pieces[pos.x][pos.y];

    for (int x = 0; x < 8; x++)
            for (int y = 0; y < 8; y++) {
                Position newPos;
                newPos.x = x;
                newPos.y = y;
                int pieceTemp = pieces[x][y];

                if (!sameSign(pieceTemp, turn)) {
                    if (legalMove(pos, newPos, absolute(piece), attacking(newPos), turn, pieces)) 
                        if (!moveWouldPutInCheck(pieces, pos, newPos, absolute(piece)))
                            setStyleSelected(x, y, true);  
                }
                else if (isTileAttacked(pieces, turn == 1 ? blackKing : whiteKing, turn) && canCastle(piece, pieceTemp, pos, newPos, turn))
                    setStyleSelected(x, y, true);  
            }
}

void handleClick(Position pos) {
    int piece = pieces[pos.x][pos.y];

    if (sameSign(piece, turn)) {
        removeStyle();

        if (isTileAttacked(pieces, turn == 1 ? blackKing : whiteKing, turn) && canCastle(selectedPiece, piece, selectedPiecePos, pos, turn)) {
            castle(selectedPiecePos, pos);
            printBoard(pieces);
            return;
        }

        setStyleSelected(pos.x, pos.y, true);
        
        selectedPiecePos = pos;
        selectedPiece = piece;

        drawMoves(pos);
    }
    else if (selectedPiece != 0) {
        bool correctMove = legalMove(selectedPiecePos, pos, absolute(selectedPiece), attacking(pos), 1, pieces);
        bool pawnMove = false;

        // printBoard(pieces);
        // printf("WHITE: X:%d Y:%d\n", whiteKing.x, whiteKing.y);
        // printf("BLACK: X:%d Y:%d\n", blackKing.x, blackKing.y);

        if (!correctMove)
            return;

        int array[8][8];
        memcpy(array, pieces, sizeof(pieces));
        
        // checking if can kill king in one turn under check without killing own king
        if (attacking(pos) || (pawnMove && (pos.y == 0 || pos.y == 7))) {
            explode(array, pos);
            array[selectedPiecePos.x][selectedPiecePos.y] = 0;
        }

        if (isKingDead(array, turn * -1) && !isKingDead(array, turn)) { 
            placePiece(selectedPiecePos, pos);
            explode(pieces, pos);
                
            printf("lost\n");
            fflush(stdout);
            return;
        } else if (isKingDead(array, turn)) {
            printf("you cant kill your own king\n");
            fflush(stdout);
            return;
        }

        // checkin if move would put king in a check
        if (moveWouldPutInCheck(pieces, selectedPiecePos, pos, absolute(selectedPiece)))
            return;

        // making move
        if (absolute(selectedPiece) == 6) { 
            changeKingPos(turn, selectedPiecePos);
            kingMoved(turn);
        }
        else if (absolute(selectedPiece) == 4)
            movedRook(selectedPiecePos);

        if (attacking(pos) || (pawnMove && (pos.y == 0 || pos.y == 7))) {
            placePiece(selectedPiecePos, pos);
            if (explode(pieces, pos))   {
                printf("lost\n");
                fflush(stdout);
            }
        }                
        else 
            placePiece(selectedPiecePos, pos);

        turn *= -1;
        removeStyle();
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