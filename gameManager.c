#include <stdio.h>
#include "gameManager.h"
#include "uiManager.h"
#include "PiecesSource/pawn.h"
#include "PiecesSource/bishop.h"
#include "PiecesSource/knight.h"
#include "PiecesSource/rook.h"
#include "PiecesSource/king.h"

int piecesSample[8][8] = { 
                {4, 1, 0, 0, 0, 0, -1, -4}, 
                {3, 1, 0, 0, 0, 0, -1, -3},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {5, 1, 0, 0, 0, 0, -1, -5},
                {6, 1, 0, 0, 0, 0, -1, -6},
                {2, 1, 0, 0, 0, 0, -1, -2},
                {3, 1, 0, 0, 0, 0, -1, -3}, 
                {4, 1, 0, 0, 0, 0, -1, -4} 
                };

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

bool check = false;
int turn = 1;
int selectedPiece = 0;
Position selectedPiecePos;

void initialize() {
    check = false;
    turn = 1;
    selectedPiece = 0;

    whiteKing.x = 4;
    whiteKing.y = 0;

    blackKing.x = 4;
    blackKing.y = 7;

    whiteKingMoved = false;
    blackKingMoved = false;

    resetRooks();

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            pieces[x][y] = piecesSample[x][y];
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
                
                if (absolute(array[x][y]) != 1 && array[x][y] != 0) {
                    array[x][y] = 0;

                    if (array == pieces)
                        removePieceImage(x, y);
                }
            }
        }
    }

    if (absolute(array[tile.x][tile.y]) == 1) {
        if (array == pieces)
            removePieceImage(tile.x, tile.y);

        array[tile.x][tile.y] = 0;
    }

    return lost;
}

// 1 for sucess, 0 for imposibble move and -1 for default
int moveWouldKillKing(int array[8][8], int piece, Position from, Position to, int t, bool pawnMove) { 

    int arrayCopy[8][8];
    memcpy(arrayCopy, array, sizeof(arrayCopy));

    if (attacking(to) || (pawnMove && (to.y == 0 || to.y == 7))) {
        explode(arrayCopy, to);
        arrayCopy[from.x][from.y] = 0;
    }

    if (isKingDead(arrayCopy, t * -1) && !isKingDead(arrayCopy, t))
        return 1;
    else if (isKingDead(arrayCopy, t)) {
        // printf("you cant kill your own king\n");
        // fflush(stdout);
        return 0;
    }

    return -1;
}

bool legalMove(Position from, Position to, int piece, bool attacking, int t, int array[8][8]) {
    switch(piece) {
        case 1:
            return legalMovePawn(from, to, attacking, t, array); 
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

bool moveWouldPutInCheck(int array[8][8], Position from, Position to, int piece, int t) {
    bool illegalMove = false;
    int arrayCopy[8][8];

    memcpy(arrayCopy, array, sizeof(pieces));

    if (attacking(to)) {
        explode(arrayCopy, to);
        arrayCopy[from.x][from.y] = 0;
    } 
    else {
        arrayCopy[to.x][to.y] = arrayCopy[from.x][from.y];
        arrayCopy[from.x][from.y] = 0;
    }
        
    if (absolute(piece) == 6)
        changeKingPos(t, to);   

    if (t == 1) {
        if (isTileAttacked(arrayCopy, whiteKing, -1))
            illegalMove = true;
    }
    else {
        if (isTileAttacked(arrayCopy, blackKing, 1))
            illegalMove = true;
    }
    
    if (absolute(piece) == 6)
        changeKingPos(t, from);

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

    if (isTileAttacked(pieces, kingPos, t * -1))
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
    changeLabel("");

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++) 
            setStyleSelected(x, y, false);
}

bool checkIfCheckMate(int array[8][8], int t) {
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) {
            Position piecePos;
            piecePos.x = i;
            piecePos.y = j;
            int piece = array[i][j];

            if (sameSign(piece, t)) {
                for (int x = 0; x < 8; x++)
                    for (int y = 0; y < 8; y++) {
                        Position newPos;
                        newPos.x = x;
                        newPos.y = y;
                        int pieceTemp = pieces[x][y];
                        bool pawnMove = absolute(piece) == 1 ? true : false;

                        if (!sameSign(pieceTemp, t)) {
                            if (legalMove(piecePos, newPos, absolute(piece), attacking(newPos), t, pieces)) {
                                int k = moveWouldKillKing(pieces, selectedPiece, piecePos, newPos, t, pawnMove);

                                if (k == 1) {
                                    // printf("Not in check from X:%d Y:%d, To X:%d Y:%d, attacking\n",i, j, x, y);
                                    // fflush(stdout);
                                    return false;
                                }
                                else if (k == -1 && !moveWouldPutInCheck(pieces, piecePos, newPos, absolute(piece), t)) {
                                    // printf("Not in check from X:%d Y:%d, To X:%d Y:%d\n",i, j, x, y);
                                    // fflush(stdout);
                                    return false;
                                }
                            }
                        }

                    }
            }
        }
        
    check = true;
    changeLabel("Checkmate");
    // Position pos = t == 1 ? whiteKing : blackKing;
    // setStyleCheck(pos.x, pos.y, true);

    return true;
}

void drawMoves(Position pos) {
    int piece = pieces[pos.x][pos.y];

    for (int x = 0; x < 8; x++)
            for (int y = 0; y < 8; y++) {
                Position newPos;
                newPos.x = x;
                newPos.y = y;
                int pieceTemp = pieces[x][y];

                bool pawnMove = absolute(piece) == 1 ? true : false;
                if (!sameSign(pieceTemp, turn)) {
                    if (legalMove(pos, newPos, absolute(piece), attacking(newPos), turn, pieces)) {
                        int i = moveWouldKillKing(pieces, selectedPiece, pos, newPos, turn, pawnMove);

                        if (i == 1)
                            setStyleSelected(x, y, true);  
                        else if (i == -1 && !moveWouldPutInCheck(pieces, pos, newPos, absolute(piece), turn)) 
                            setStyleSelected(x, y, true);
                    }
                }
                else if (canCastle(piece, pieceTemp, pos, newPos, turn))
                    setStyleSelected(x, y, true);  
            }
}

void drawCheck() {
    if (isTileAttacked(pieces, whiteKing, -1)) {
        changeLabel("Check");
        setStyleCheck(whiteKing.x, whiteKing.y, true);
    }
    if (isTileAttacked(pieces, blackKing, 1)) {
        changeLabel("Check");
        setStyleCheck(blackKing.x, blackKing.y, true);
    }
}

void handleClick(Position pos) {
    if (check)
        return;

    int piece = pieces[pos.x][pos.y];

    if (sameSign(piece, turn)) {
        removeStyle();

        if (pos.x == selectedPiecePos.x && pos.y == selectedPiecePos.y) {
            selectedPiece = 0;
            selectedPiecePos.x = -1;
            selectedPiecePos.y = -1;
            return;
        }

        if (canCastle(selectedPiece, piece, selectedPiecePos, pos, turn)) {
            castle(selectedPiecePos, pos);
            return;
        }

        setStyleSelected(pos.x, pos.y, true);
        
        selectedPiecePos = pos;
        selectedPiece = piece;

        drawMoves(pos);
        drawCheck();
    }
    else if (selectedPiece != 0) {
        
        bool correctMove = legalMove(selectedPiecePos, pos, absolute(selectedPiece), attacking(pos), turn, pieces);
        bool pawnMove = absolute(selectedPiece) == 1 ? true : false;

        if (!correctMove)
            return;
        
        // checking if can kill king in one turn under check without killing own king
        int i = moveWouldKillKing(pieces, selectedPiece, selectedPiecePos, pos, turn, pawnMove);

        if (i == 1) {
            placePiece(selectedPiecePos, pos);
            explode(pieces, pos);

            check = true;
            changeLabel("Lost Game");
            return;
        }
        else if (i == 0)
            return;

        // checkin if move would put king in a check
        if (moveWouldPutInCheck(pieces, selectedPiecePos, pos, absolute(selectedPiece), turn))
            return;

        // making move
        if (absolute(selectedPiece) == 6) { 
            changeKingPos(turn, pos);
            kingMoved(turn);
        }
        else if (absolute(selectedPiece) == 4)
            movedRook(selectedPiecePos);

        if (attacking(pos) || (pawnMove && (pos.y == 0 || pos.y == 7))) {
            placePiece(selectedPiecePos, pos);
            if (explode(pieces, pos))   {
                check = true;
                changeLabel("Lost Game");
                return;
            }
        }                
        else 
            placePiece(selectedPiecePos, pos);

        checkIfRookMoved(pos);
        turn *= -1;
        
        removeStyle();

        // printBoard(pieces);
        // printf("Selected %d X:%d Y:%d\n", selectedPiece, selectedPiecePos.x, selectedPiecePos.y);
        // printf("WHITE: X:%d Y:%d\n", whiteKing.x, whiteKing.y); 
        // printf("BLACK: X:%d Y:%d\n", blackKing.x, blackKing.y);
        // printf("TURN: %d\n", turn);

        drawCheck();
        checkIfCheckMate(pieces, turn);
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