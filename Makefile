PRE = `pkg-config --cflags gtk+-3.0`
POST = `pkg-config --libs gtk+-3.0`
CC = gcc
DEP =\
	gameManager.c \
	uiManager.c \
	PiecesSource/pawn.c \
	PiecesSource/bishop.c \
	PiecesSource/rook.c \
	PiecesSource/knight.c \
	PiecesSource/king.c 

all: main

main: $(DEP)
	$(CC) $(PRE) -o chess $(DEP) $(POST)

clean:
	rm -f *.o chess