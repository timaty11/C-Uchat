#include "../inc/client.h"
GtkWidget *window;

void close_window()
{

    gtk_widget_destroy(GTK_WIDGET(window));
    mx_LoginWindow();
}


void First_Winsow()
{
    real_quit = false;
  
    GtkWidget *button;
    GtkWidget *grid;

    grid = gtk_grid_new();
    add_element_to_css(grid, "window__hello");

    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);

    load_css();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 1280, 720);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    button = gtk_button_new_with_label("UCat");

    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 2);


    
    g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(close_window), NULL);

    gtk_widget_set_size_request(button, 500, 200);

    add_element_to_css(button, "button__hui");

    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_widget_show_all(window);

}










