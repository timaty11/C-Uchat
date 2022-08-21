#include "../inc/client.h"

GtkWidget *vbox_Login = NULL;
GtkWidget *label_Login = NULL;

void load_css(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    const gchar *css_style_fille = "client/css/style.css";
    GFile *css_fp = g_file_new_for_path(css_style_fille);
    GError *error = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_fp, &error);
    g_object_unref(provider);
}


void mx_LoginWindow()
{    
    real_quit = true;
    load_css();

    grid_widget.label_window = gtk_label_new("Login");

    grid_widget.vgrid = gtk_grid_new();

    // Добавление класса гридам
    add_element_to_css(grid_widget.label_window, "label__window");

    add_element_to_css(grid_widget.vgrid, "grid__widget");

    // добавление столбов и строк в грид
    gtk_grid_set_column_spacing(GTK_GRID(grid_widget.vgrid), 6);
    gtk_grid_set_row_spacing(GTK_GRID(grid_widget.vgrid), 4);

    LoginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    add_element_to_css(LoginWindow, "window_Login");

    gtk_window_set_title(GTK_WINDOW (LoginWindow), "Login Window");
    gtk_window_set_position(GTK_WINDOW(LoginWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(LoginWindow), 600, 400);
    gtk_window_set_resizable(GTK_WINDOW(LoginWindow), FALSE);

    // Создаем ярлык и поле ввода логина
    grid_widget.label_login = gtk_label_new("Write login: ");
    grid_widget.login_entry = gtk_entry_new();

    add_element_to_css(grid_widget.login_entry, "login__entry");
    add_element_to_css(grid_widget.label_login, "label__login");

    // Создаем ярлык и поле ввода пароля
    grid_widget.password_entry = gtk_entry_new();
    grid_widget.label_password = gtk_label_new("Write password: ");

    gtk_entry_set_visibility(GTK_ENTRY(grid_widget.password_entry), FALSE);

    gtk_widget_set_size_request(GTK_WIDGET(grid_widget.login_entry), 200, 10);
    gtk_widget_set_size_request(GTK_WIDGET(grid_widget.password_entry), 300, 10);

    add_element_to_css(grid_widget.label_password, "label__password");
    add_element_to_css(grid_widget.password_entry, "password__entry");

    // Создаем кнопки
    grid_widget.login_button = gtk_button_new_with_label("Login");
    grid_widget.create_button = gtk_button_new_with_label("Register");

    gtk_button_set_relief(GTK_BUTTON(grid_widget.login_button), GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(grid_widget.create_button), GTK_RELIEF_NONE);

    gtk_widget_set_size_request(GTK_WIDGET(grid_widget.login_button), 60, 10);
    gtk_widget_set_size_request(GTK_WIDGET(grid_widget.create_button), 60, 10);

    add_element_to_css(grid_widget.login_button, "login__button");
    add_element_to_css(grid_widget.create_button, "create__button");

    // Задаем функции кнопок
    g_signal_connect(GTK_BUTTON(grid_widget.login_button), "clicked", G_CALLBACK(login_button_clicked), grid_widget.login_entry);

    g_signal_connect(G_OBJECT(grid_widget.create_button), "clicked", G_CALLBACK(mx_Window_Regist), NULL);
    g_signal_connect(LoginWindow, "destroy", G_CALLBACK(mx_exit_button_clicked), NULL);
    
    //расположение

    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.label_window, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.label_login, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.login_entry, 1, 3, 4, 1);

    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.label_password, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.password_entry, 1, 5, 4, 1);

    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.login_button, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_widget.vgrid), grid_widget.create_button, 2, 6, 1, 1);

    gtk_container_add(GTK_CONTAINER(LoginWindow), grid_widget.vgrid);

    gtk_window_set_modal(GTK_WINDOW(LoginWindow), TRUE);

    gtk_widget_show_all(LoginWindow);
}

