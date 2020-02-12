#include "uiManager.h"
#include "gameManager.h"

Position cords[8][8];
GtkButton *squares[8][8];
GtkWidget *window, *chessboard, *button, *menuLayout, *label, *labelInfo;

void loadCSS() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gchar *style = "Styles/style.css";
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

void setStyleCheck(int x, int y, bool check) {
    GtkWidget *button = (GtkWidget *)squares[x][y];
    int offset = absolute(x - y);

    if (check) {
        if (offset % 2 == 1) 
            gtk_widget_set_name(button, "whiteCheck");
        else {
            gtk_widget_set_name(button, "blackCheck");
        }
    }
    else {
        if (offset % 2 == 1) 
            gtk_widget_set_name(button, "white");
        else {
            gtk_widget_set_name(button, "black");
        }
    }
}

void setStyleSelected(int x, int y, bool selected) {
    GtkWidget *button = (GtkWidget *)squares[x][y];
    int offset = absolute(x - y);

    if (selected) {
        if (offset % 2 == 1) 
            gtk_widget_set_name(button, "whiteSelected");
        else {
            gtk_widget_set_name(button, "blackSelected");
        }
    }
    else {
        if (offset % 2 == 1) 
            gtk_widget_set_name(button, "white");
        else {
            gtk_widget_set_name(button, "black");
        }
    }
} 

void resetGame(GtkWidget *widget, gpointer data) {
    gtk_container_remove(GTK_CONTAINER (window), chessboard);

    initialize();
    initBoard();
    initPieces();
    changeLabel("");

    gtk_container_add(GTK_CONTAINER(window), chessboard);
    gtk_widget_show_all(window);
}

void newGame(GtkWidget *widget, gpointer data) {
    gtk_container_remove(GTK_CONTAINER (window), menuLayout);
    gtk_window_resize(GTK_WINDOW(window), 800, 900);

    initialize();
    initBoard();
    initPieces();
    changeLabel("");

    gtk_container_add(GTK_CONTAINER(window), chessboard);
    gtk_widget_show_all(window);
}

void changeLabel(gchar *g) {
    gtk_label_set_text((GtkLabel *)labelInfo, g);
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

            g_signal_connect(button, "clicked", 
                            G_CALLBACK(button_clicked), &cords[x][y]);

            squares[x][y] = (GtkButton *)button;
            gtk_grid_attach((GtkGrid *)chessboard, button, x, y, 1, 1);
        }

    //exit button
    button = gtk_button_new();
    gtk_widget_set_size_request(button, 190, 100);
    g_signal_connect(button, "clicked",
		      G_CALLBACK(createMenu), NULL);

    label = gtk_label_new("Exit");

    gtk_widget_set_name(label, "newGame");
    gtk_widget_set_name(button, "menu");

    gtk_container_add(GTK_CONTAINER (button), label);
    gtk_grid_attach((GtkGrid *)chessboard, button, 0, 8, 2, 1);

    //reset button
    button = gtk_button_new();
    gtk_widget_set_size_request(button, 190, 100);
    g_signal_connect(button, "clicked",
		      G_CALLBACK(resetGame), NULL);

    label = gtk_label_new("Reset");

    gtk_widget_set_name(label, "newGame");
    gtk_widget_set_name(button, "menu");

    gtk_container_add(GTK_CONTAINER (button), label);
    gtk_grid_attach((GtkGrid *)chessboard, button, 2, 8, 2, 1);

    //adding label
    labelInfo = gtk_label_new("WHITE");
    gtk_widget_set_size_request(labelInfo, 400, 100);
    gtk_widget_set_name(labelInfo, "infoWhite");

    gtk_grid_attach((GtkGrid *)chessboard, labelInfo, 4, 8, 4, 1);
}

void close_window(GtkWidget *widget, gpointer w) {
    gtk_window_close((GtkWindow *)window);
}

void createMenu() {
    if (gtk_container_get_children(GTK_CONTAINER(window)) != NULL)
        gtk_container_remove(GTK_CONTAINER(window), chessboard);
        
    gtk_window_resize(GTK_WINDOW(window), 500, 300);
    
    menuLayout = gtk_grid_new();
    gtk_widget_set_name(menuLayout, "menuLayout");

    //title
    label = gtk_label_new("Atomic Chess");
    gtk_widget_set_name(label, "title");
    gtk_widget_set_size_request(label, 500, 100);
    gtk_grid_attach((GtkGrid *)menuLayout, label, 0, 0, 1, 1);

    //exit button
    button = gtk_button_new();
    gtk_widget_set_size_request(button, 490, 100);
    g_signal_connect(button, "clicked",
		      G_CALLBACK(close_window), NULL);

    label = gtk_label_new("Exit");

    gtk_widget_set_name(label, "newGame");
    gtk_widget_set_name(button, "menu");

    gtk_container_add(GTK_CONTAINER (button), label);
    gtk_grid_attach((GtkGrid *)menuLayout, button, 0, 2, 1, 1);

    //new game button
    button = gtk_button_new();
    gtk_widget_set_size_request(button, 490, 100);
    g_signal_connect(button, "clicked",
		      G_CALLBACK (newGame), NULL);

    label = gtk_label_new("New Game");

    gtk_widget_set_name(label, "newGame");
    gtk_widget_set_name(button, "menu");

    gtk_container_add(GTK_CONTAINER (button), label);
    gtk_grid_attach((GtkGrid *)menuLayout, button, 0, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(window), menuLayout);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    loadCSS();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);

    gtk_window_set_title((GtkWindow *)window, "Atomic chess");
    gtk_window_set_resizable((GtkWindow *)window, FALSE);
    
    g_signal_connect (window, "destroy",
	                G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (window, "delete-event",
	 	            G_CALLBACK (gtk_main_quit), NULL);

    createMenu();
    gtk_main();
    
    return 0;
}
