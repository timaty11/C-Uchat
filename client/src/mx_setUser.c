#include "../inc/client.h"

GtkWidget *image;

GtkWidget *button_avatar;

void img1()
{
    image = gtk_image_new_from_file("client/img/avatar/amine.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img2()
{
    image = gtk_image_new_from_file("client/img/avatar/am-am.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img3()
{
    image = gtk_image_new_from_file("client/img/avatar/bat-man.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img4()
{
    image = gtk_image_new_from_file("client/img/avatar/blackcat.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img5()
{
    image = gtk_image_new_from_file("client/img/avatar/jerry.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img6()
{
    image = gtk_image_new_from_file("client/img/avatar/kek.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img7()
{
    image = gtk_image_new_from_file("client/img/avatar/luntic.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img8()
{
    image = gtk_image_new_from_file("client/img/avatar/pingvin.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img9()
{
    image = gtk_image_new_from_file("client/img/avatar/tom.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void mx_setUser()
{

    GtkWidget *window_User_avatar;
    // GtkWidget *main_box;
    GtkWidget *vbox_avatar1, *vbox_avatar2, *vbox_avatar3, *main_vbox_avatar;
    GtkWidget *popover_avatar, *avatar_box;
    //боксы для боксов в боксы
    GtkWidget *box_login, *box_all_login, *box_all_widget;
    GtkWidget *label_login;

    GtkWidget *box_pass, *box_all_pass;
    GtkWidget *label_pass;

    GtkWidget *box_pass_true, *box_all_pass_true;
    GtkWidget *label_pass_true, *button_pass_true;

    window_User_avatar = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window_User_avatar), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window_User_avatar), 5);
    gtk_window_set_default_size(GTK_WINDOW(window_User_avatar), 640, 640);
    add_element_to_css(window_User_avatar, "setting_window");

    button_avatar = gtk_menu_button_new();
    gtk_container_add(GTK_CONTAINER(button_avatar), gtk_label_new(""));

    image = gtk_image_new_from_file("client/img/avatar/amine.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
    add_element_to_css(button_avatar, "btn__avatar");

    main_vbox_avatar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    vbox_avatar1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    vbox_avatar2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    vbox_avatar3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *avatar1 = gtk_button_new_with_label("");

    GtkWidget *avatar2 = gtk_button_new_with_label("");
    GtkWidget *avatar3 = gtk_button_new_with_label("");

    GtkWidget *avatar4 = gtk_button_new_with_label("");
    GtkWidget *avatar5 = gtk_button_new_with_label("");
    GtkWidget *avatar6 = gtk_button_new_with_label("");

    GtkWidget *avatar7 = gtk_button_new_with_label("");
    GtkWidget *avatar8 = gtk_button_new_with_label("");
    GtkWidget *avatar9 = gtk_button_new_with_label("");

    add_element_to_css(avatar1, "btn__avatar1");
    add_element_to_css(avatar2, "btn__avatar2");
    add_element_to_css(avatar3, "btn__avatar3");
    add_element_to_css(avatar4, "btn__avatar4");
    add_element_to_css(avatar5, "btn__avatar5");
    add_element_to_css(avatar6, "btn__avatar6");
    add_element_to_css(avatar7, "btn__avatar7");
    add_element_to_css(avatar8, "btn__avatar8");
    add_element_to_css(avatar9, "btn__avatar9");

    g_signal_connect(avatar1, "clicked", G_CALLBACK(img1), NULL);
    g_signal_connect(avatar2, "clicked", G_CALLBACK(img2), NULL);
    g_signal_connect(avatar3, "clicked", G_CALLBACK(img3), NULL);
    g_signal_connect(avatar4, "clicked", G_CALLBACK(img4), NULL);
    g_signal_connect(avatar5, "clicked", G_CALLBACK(img5), NULL);
    g_signal_connect(avatar6, "clicked", G_CALLBACK(img6), NULL);
    g_signal_connect(avatar7, "clicked", G_CALLBACK(img7), NULL);
    g_signal_connect(avatar8, "clicked", G_CALLBACK(img8), NULL);
    g_signal_connect(avatar9, "clicked", G_CALLBACK(img9), NULL);

    gtk_box_pack_start(GTK_BOX(vbox_avatar1), avatar1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar1), avatar2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar1), avatar3, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_avatar2), avatar4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar2), avatar5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar2), avatar6, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_avatar3), avatar7, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar3), avatar8, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avatar3), avatar9, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(main_vbox_avatar), vbox_avatar1);
    gtk_container_add(GTK_CONTAINER(main_vbox_avatar), vbox_avatar2);
    gtk_container_add(GTK_CONTAINER(main_vbox_avatar), vbox_avatar3);

    popover_avatar = gtk_popover_new(button_avatar);
    gtk_container_add(GTK_CONTAINER(popover_avatar), main_vbox_avatar);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(button_avatar), popover_avatar);
    gtk_widget_show_all(popover_avatar);

    avatar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(avatar_box), button_avatar);

    box_all_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // login rename
    box_all_login = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    label_login = gtk_label_new("Enter new name");
    entry_login = gtk_entry_new();
    add_element_to_css(box_all_login, "login__style");

    box_login = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box_login), label_login, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_login), entry_login, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_all_login), box_login, TRUE, FALSE, 5);

    // pass rename
    box_all_pass = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    label_pass = gtk_label_new("Enter old pass");
    entry_pass = gtk_entry_new();
    add_element_to_css(box_all_pass, "login__style");

    box_pass = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box_pass), label_pass, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_pass), entry_pass, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_all_pass), box_pass, TRUE, FALSE, 5);

    box_all_pass_true = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    label_pass_true = gtk_label_new("Enter new pass");
    entry_pass_true = gtk_entry_new();
    add_element_to_css(box_all_pass_true, "login__style");
    GtkWidget *box_button = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    button_pass_true = gtk_button_new_with_label("Enter");
    g_signal_connect(button_pass_true, "clicked", G_CALLBACK(change_something), NULL);
    gtk_box_pack_start(GTK_BOX(box_button), button_pass_true, TRUE, FALSE, 5);
    add_element_to_css(button_pass_true, "btn_size");
    gtk_button_set_relief(GTK_BUTTON(button_pass_true), GTK_RELIEF_NONE);

    add_element_to_css(entry_pass_true, "entry__set");
    add_element_to_css(entry_pass, "entry__set");
    add_element_to_css(entry_login, "entry__set");

    box_pass_true = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box_pass_true), label_pass_true, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_pass_true), entry_pass_true, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_pass_true), box_button, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box_all_pass_true), box_pass_true, TRUE, FALSE, 5);

    // вставляем все боксы в один бокс чтобы отобразить
    gtk_container_add(GTK_CONTAINER(box_all_widget), avatar_box);
    gtk_container_add(GTK_CONTAINER(box_all_widget), box_all_login);
    gtk_container_add(GTK_CONTAINER(box_all_widget), box_all_pass);
    gtk_container_add(GTK_CONTAINER(box_all_widget), box_all_pass_true);
    add_element_to_css(box_all_widget, "main_box_set");

    gtk_container_add(GTK_CONTAINER(window_User_avatar), box_all_widget);

    g_signal_connect(G_OBJECT(window_User_avatar), "destroy", G_CALLBACK(gtk_widget_destroy), window_User_avatar);

    gtk_widget_show_all(window_User_avatar);
}

