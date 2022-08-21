#include "../inc/client.h"

void close_window2(GtkWidget *widget, gpointer window)
{
    if (widget)
    {
    }
    gtk_widget_destroy(GTK_WIDGET(window));
}

void mx_Window_Regist()
{
    real_quit = true;
    load_css();

    if (LoginWindow != NULL)
    {
        real_quit = false;
        gtk_widget_destroy(LoginWindow);
        LoginWindow = NULL;
    }

    registr_window.registration_window = gtk_grid_new();
    gtk_widget_set_name(registr_window.registration_window, "myGrid");
    add_element_to_css(registr_window.registration_window, "registration_window");

    gtk_grid_set_column_spacing(GTK_GRID(registr_window.registration_window), 6);
    gtk_grid_set_row_spacing(GTK_GRID(registr_window.registration_window), 2);


    Window_Regist = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    add_element_to_css(Window_Regist, "window_Login");
    gtk_widget_set_size_request(Window_Regist, 600, 450);

    gtk_window_set_position(GTK_WINDOW(Window_Regist), GTK_WIN_POS_CENTER);

    registr_window.Text_registration = gtk_label_new("Registration");
    add_element_to_css(registr_window.Text_registration, "Text_registration");

    registr_window.login_label = gtk_label_new("Enter login: ");
    registr_window.login_entry = gtk_entry_new();

    add_element_to_css(registr_window.login_label, "register_login__entry_text");
    add_element_to_css(registr_window.login_entry, "register_login__entry");

    registr_window.name_label = gtk_label_new("Enter name: ");
    registr_window.name_entry = gtk_entry_new();

    add_element_to_css(registr_window.name_label, "register_name_entry_text");
    add_element_to_css(registr_window.name_entry, "register_name_entry");

    registr_window.pass_label = gtk_label_new("Enter password:");
    registr_window.pass_entry = gtk_entry_new();

    add_element_to_css(registr_window.pass_label, "register_pass_entry_text");
    add_element_to_css(registr_window.pass_entry, "register_pass_entry");

    registr_window.pass_true_label = gtk_label_new("Confim the password:");
    registr_window.pass_true_entry = gtk_entry_new();

    add_element_to_css(registr_window.pass_true_label, "register_pass_true_entry_text");
    add_element_to_css(registr_window.pass_true_entry, "register_pass_true_entry");

    // Создаем кнопки
    registr_window.login_button = gtk_button_new_with_label("Login");

    registr_window.login_button = gtk_button_new_with_label("Back");
    registr_window.registr_button = gtk_button_new_with_label("Register");

    gtk_button_set_relief(GTK_BUTTON(registr_window.login_button), GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(registr_window.registr_button), GTK_RELIEF_NONE);

    gtk_widget_set_size_request(GTK_WIDGET(registr_window.registr_button), 60, 10);
    gtk_widget_set_size_request(GTK_WIDGET(registr_window.login_button), 60, 10);

    add_element_to_css(registr_window.login_button, "login__button_registration_window");
    add_element_to_css(registr_window.registr_button, "create__button_registration_window");

    g_signal_connect(GTK_BUTTON(registr_window.registr_button), "clicked", G_CALLBACK(register_button_clicked), G_OBJECT(Window_Regist));
    g_signal_connect(GTK_BUTTON(registr_window.login_button), "clicked", G_CALLBACK(login_button_clicked), NULL);
    g_signal_connect(G_OBJECT(registr_window.login_button), "clicked", G_CALLBACK(close_window2), G_OBJECT(Window_Regist));
    g_signal_connect(Window_Regist, "destroy", G_CALLBACK(mx_exit_button_clicked), NULL);


    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.Text_registration, 2, 0, 1, 1);
    // Login
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.login_label, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.login_entry, 2, 2, 1, 1);

    // Name
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.name_label, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.name_entry, 2, 3, 1, 1);

    // Password
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.pass_label, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.pass_entry, 2, 4, 1, 1);

    // Confim the password
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.pass_true_label, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.pass_true_entry, 2, 5, 1, 1);
    // Кнопки
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.registr_button, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(registr_window.registration_window), registr_window.login_button, 2, 6, 1, 1);

    gtk_container_add(GTK_CONTAINER(Window_Regist), registr_window.registration_window);
    gtk_window_set_modal(GTK_WINDOW(Window_Regist), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(Window_Regist), FALSE);

    gtk_widget_show_all(Window_Regist);
}

