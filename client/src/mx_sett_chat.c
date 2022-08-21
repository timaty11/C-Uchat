#include "../inc/client.h"

GtkWidget *image;

GtkWidget *button_avatar;

// GtkWidget *batton_avatar, *popover_avatar; // не пизди

// void display_avatar()
// {
//  gtk_widget_show_all (popover_avatar);
// }

void img10()
{
    image = gtk_image_new_from_file("client/img/avatar/amine.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img20()
{
    image = gtk_image_new_from_file("client/img/avatar/am-am.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img30()
{
    image = gtk_image_new_from_file("client/img/avatar/bat-man.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img40()
{
    image = gtk_image_new_from_file("client/img/avatar/blackcat.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img50()
{
    image = gtk_image_new_from_file("client/img/avatar/jerry.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img60()
{
    image = gtk_image_new_from_file("client/img/avatar/kek.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img70()
{
    image = gtk_image_new_from_file("client/img/avatar/luntic.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img80()
{
    image = gtk_image_new_from_file("client/img/avatar/pingvin.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void img90()
{
    image = gtk_image_new_from_file("client/img/avatar/tom.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
}

void mx_sett_chat()
{
    GtkWidget *window_User_avatar;
    // GtkWidget *main_box;
    GtkWidget *vbox_avatar1, *vbox_avatar2, *vbox_avatar3, *main_vbox_avatar;
    GtkWidget *popover_avatar, *avatar_box;
    //боксы для боксов в боксы
    GtkWidget *box_all_login, *box_all_widget;
    GtkWidget *entry_login, *label_login;

    window_User_avatar = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window_User_avatar), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window_User_avatar), 5);
    gtk_window_set_default_size(GTK_WINDOW(window_User_avatar), 640, 640);
    add_element_to_css(window_User_avatar, "setting_window");

    button_avatar = gtk_menu_button_new();
    gtk_container_add(GTK_CONTAINER(button_avatar), gtk_label_new("")); // этот лейбл помогает нам не отображать хуету

    image = gtk_image_new_from_file("client/img/avatar/amine.jpg");
    gtk_button_set_image(GTK_BUTTON(button_avatar), image);
    add_element_to_css(button_avatar, "btn__avatar");

    main_vbox_avatar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    // add_element_to_css(main_vbox_avatar, "vbox_sett");

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

    g_signal_connect(avatar1, "clicked", G_CALLBACK(img10), NULL);
    g_signal_connect(avatar2, "clicked", G_CALLBACK(img20), NULL);
    g_signal_connect(avatar3, "clicked", G_CALLBACK(img30), NULL);
    g_signal_connect(avatar4, "clicked", G_CALLBACK(img40), NULL);
    g_signal_connect(avatar5, "clicked", G_CALLBACK(img50), NULL);
    g_signal_connect(avatar6, "clicked", G_CALLBACK(img60), NULL);
    g_signal_connect(avatar7, "clicked", G_CALLBACK(img70), NULL);
    g_signal_connect(avatar8, "clicked", G_CALLBACK(img80), NULL);
    g_signal_connect(avatar9, "clicked", G_CALLBACK(img90), NULL);

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
    label_login = gtk_label_new("ИДИ нахуй заебал");
    entry_login = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_all_login), label_login, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_all_login), entry_login, FALSE, FALSE, 0);
    add_element_to_css(box_all_login, "login__style1");
    add_element_to_css(box_all_login, "label_login1");

    GtkWidget *box_button = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *button_pass_true = gtk_button_new_with_label("подтверди");
    gtk_box_pack_start(GTK_BOX(box_button), button_pass_true, TRUE, FALSE, 5);
    add_element_to_css(button_pass_true, "btn_size1");
    gtk_button_set_relief(GTK_BUTTON(button_pass_true), GTK_RELIEF_NONE);

    add_element_to_css(entry_login, "entry__set1");

    // pass rename

    //подтвердить пароль нахуй сука

    // вставляем все боксы в один бокс чтобы отобразить
    gtk_container_add(GTK_CONTAINER(box_all_widget), avatar_box);
    gtk_container_add(GTK_CONTAINER(box_all_widget), box_all_login);
    gtk_container_add(GTK_CONTAINER(box_all_widget), box_button);

    gtk_container_add(GTK_CONTAINER(window_User_avatar), box_all_widget);

    g_signal_connect(G_OBJECT(window_User_avatar), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window_User_avatar);
}
