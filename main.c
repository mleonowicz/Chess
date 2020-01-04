#include <gtk/gtk.h>

void loadCSS() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    const gchar *style = "style.css";
    GError *error = 0;
    GFile *css = g_file_new_for_path(style);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css, &error);

    g_object_unref(provider);
}

void button_clicked(GtkWidget *widget, gpointer data) {
  printf("clicked\n");
}

int main(int argc, char **argv) {
    GtkWidget *window, *chessboard, *button;

    gtk_init(&argc, &argv);

    loadCSS();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable((GtkWindow *) window, FALSE);

    chessboard = gtk_grid_new();

    for (int x = 1; x < 9; x++) 
        for (int y = 1; y < 9; y++) {
            button = gtk_button_new();
            gtk_widget_set_size_request(button, 80, 80);

            if ((x + y) % 2 == 0) 
                gtk_widget_set_name(button, "black");
            else 
                gtk_widget_set_name(button, "white");

            g_signal_connect (button, "clicked",
                        G_CALLBACK(button_clicked), NULL);

            gtk_grid_attach((GtkGrid *) chessboard, button, x, y, 1, 1);
        }

    gtk_container_add(GTK_CONTAINER(window), chessboard);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}