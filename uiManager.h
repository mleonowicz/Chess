#ifndef uiManager
#define uiManager

#include <gtk/gtk.h>
#include <stdbool.h>

void loadCSS();
char *getPiecePath(int);
void placePieceImage(int, int, int);
void removePieceImage(int, int);
void button_clicked(GtkWidget *, gpointer);
void setStyleSelected(int, int, bool);
void setStyleCanMove(int, int, bool);
void initPieces();
void setStyleCheck(int, int, bool);
void createMenu();
void changeLabel(gchar *);
void initBoard();
int main(int, char **);

#endif