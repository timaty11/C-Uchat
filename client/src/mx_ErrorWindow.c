#include "../inc/client.h"

void mx_destroy_button_clicked()
{
    gtk_widget_destroy(ErrorWindow);
    mx_LoginWindow();
}

void mx_ErrorWindow(char * error_text)
{
    ErrorWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_resizable(GTK_WINDOW(ErrorWindow), TRUE);
	gtk_window_set_title(GTK_WINDOW(ErrorWindow), "DONBASSgo ERROR");
	gtk_widget_set_size_request(ErrorWindow, 400, 100);
	gtk_window_set_position(GTK_WINDOW(ErrorWindow), GTK_WIN_POS_CENTER_ALWAYS);

	GtkWidget * error_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget * error_label = gtk_label_new(error_text);

	GtkWidget * ok_button = gtk_button_new_with_label("OK"); // кнопка с шестерней !!!
    g_signal_connect(ok_button, "clicked", G_CALLBACK(mx_destroy_button_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(error_box), error_label);
    gtk_container_add(GTK_CONTAINER(error_box), ok_button);
	gtk_container_add(GTK_CONTAINER(ErrorWindow), error_box);
    
    gtk_widget_show_all(ErrorWindow);
}

