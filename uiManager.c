#include "uiManager.h"
#include "gameManager.h"

Position cords[8][8];
GtkButton *squares[8][8];
GtkWidget *window, *chessboard, *button;

void loadCSS() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    const gchar *style = "Styles/style.css";
    GError *error = 0;
    GFile *css = g_file_new_for_path(style);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css, &error);

    g_object_unref(provider);
}
/*     1 - pawn
       2 - bishop
       3 - knight
       4 - rook
       5 - queen
       6 - king */
char *getPiecePath(int index) {
    if (index == 0)     
        return NULL;

    char color = index > 0 ? 'w' : 'b';
    index = index > 0 ? index : index * -1;

    char *name = malloc(sizeof(char) * 25);

    switch(index) {
        case 1:
            sprintf(name, "Pieces/pawn%c.svg", color);
        break;
        case 2:
            sprintf(name, "Pieces/bishop%c.svg", color);
        break;
        case 3:
            sprintf(name, "Pieces/knight%c.svg", color);
        break;
        case 4:
            sprintf(name, "Pieces/rook%c.svg", color);
        break;
        case 5:
            sprintf(name, "Pieces/queen%c.svg", color);
        break;
        case 6:
            sprintf(name, "Pieces/king%c.svg", color);
        break;   
    }

    return name;
}

void placePieceImage(int piece, int x, int y) {
    char *path = getPiecePath(piece);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(path, 100, 100, NULL);
    GtkImage *image = (GtkImage *)gtk_image_new_from_pixbuf(pixbuf);
    gtk_button_set_image((GtkButton *)squares[x][y], (GtkWidget *)image);

    free(path);
}

void removePieceImage(int x, int y) {
    gtk_button_set_image((GtkButton *)squares[x][y], NULL);
}

void button_clicked(GtkWidget *widget, gpointer data) {
    Position *a = (Position *)data; 
    handleClick(*a);
}

void initPieces() {
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 2; y++) {
            placePieceImage(pieces[x][y], x, y);
        }

    for (int x = 0; x < 8; x++)
        for (int y = 6; y < 8; y++) {
            placePieceImage(pieces[x][y], x, y);
        }
}

void initBoard() {
    chessboard = gtk_grid_new();
    
    for (int y = 0; y < 8; y++) 
        for (int x = 0; x < 8; x++) {
            button = gtk_button_new();
            gtk_widget_set_size_request(button, 100, 100);

            if ((x + y) % 2 == 0) 
                gtk_widget_set_name(button, "black");
            else 
                gtk_widget_set_name(button, "white");
            
            cords[x][y].x = x;
            cords[x][y].y = y;

            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), &cords[x][y]);

            squares[x][y] = (GtkButton *)button;
            gtk_grid_attach((GtkGrid *)chessboard, button, x, y, 1, 1);
        }
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    loadCSS();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable((GtkWindow *)window, FALSE);
    
    initBoard();
    initPieces();

    gtk_container_add(GTK_CONTAINER(window), chessboard);
    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}