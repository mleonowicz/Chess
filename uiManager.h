#ifndef uiManager
#define uiManager

#include <gtk/gtk.h>

void loadCSS();
char *getPiecePath(int);
void placePieceImage(int, int, int);
void removePieceImage(int, int);
void button_clicked(GtkWidget *, gpointer);
void initPieces();
void initBoard();
int main(int, char **);

#endif