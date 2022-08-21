#include "../inc/client.h"
GtkWidget
 *image_user,
 *statu_user,
 *poisk,
 *serch_input,
 *setting_button,
 *setting_button,
 *bar,
 *user_repository,
 *main_repository,
 *main_box_mes,
 *statu_user,
 *setting_button,
 *setings_user,
 *about_us,
 *log_out,
 *temp_box;

GtkWidget *mes_user = NULL;

GtkWidget *send_button;
GtkWidget *ChatWindow = NULL;
GtkWidget *scroll_mes_window;

GtkWidget *mes_box;
GtkWidget *main_box_mes; // бокс с вводом сообщения, отображения сообщения и с кнопкой отправки
GtkWidget *main_box_chat;

GtkWidget *user_repository; // бокс со стаком и листом юзеров
GtkWidget *main_repository;


GtkWidget *box_thema;

GtkWidget *popover, *vbox;
GtkWidget *popover_poisk, *vbox_poisk, *serch_input;
GtkWidget *popover_statu, *vbox_statu;
char * text4;

int last_chat = 1;

const char *pages[666];

GtkWidget *widget;

void mx_swap_dark_mode()
{
 load_css();
 add_element_to_css(image_user, "dark");
 add_element_to_css(statu_user, "dark");
 add_element_to_css(poisk, "dark");
 add_element_to_css(serch_input, "dark");
 add_element_to_css(bar, "dark");

 add_element_to_css(user_repository, "text_dark_mode");
 add_element_to_css(main_repository, "dark");
 add_element_to_css(main_box_mes, "dark");
 add_element_to_css(statu_user, "dark");
 add_element_to_css(temp_box, "text_dark_mode");

 add_element_to_css(mes_user, "text_dark_mode");

 // settings
 add_element_to_css(popover, "dark_settings");
 add_element_to_css(vbox, "dark_settings");
 add_element_to_css(setings_user, "dark_settings");
 add_element_to_css(about_us, "dark_settings");
 add_element_to_css(log_out, "dark_settings_red");
}



void text_in_Chat(char * text, char * orientation)
{
	if (gtk_stack_get_visible_child_name(GTK_STACK(stack)))
	{
		int text_len = mx_strlen(text);
		if (text_len > 70)
		{
			char * modify_text = mx_strnew(text_len + (text_len/70));
			int j = 0;
			for (int i = 0; i < text_len; i++)
			{
				if (!(i % 70))
				{
					modify_text[i] = '\n';
				}
				else
				{
					modify_text[i] = text[j];
					j++;
				}
			}

			mes_user = gtk_label_new(modify_text);
			mx_strdel(&modify_text);
		}
		else
		{
			mes_user = gtk_label_new(text);
		}
		gtk_label_set_selectable(GTK_LABEL(mes_user), TRUE);

		GtkWidget * temp = gtk_stack_get_visible_child((GtkStack *)stack);
		GList *children_box = gtk_container_get_children(GTK_CONTAINER(temp));
		GList *children_scroll_box = gtk_container_get_children(GTK_CONTAINER(children_box->data));
		GList *children_scroll = gtk_container_get_children(GTK_CONTAINER(children_scroll_box->data));
		GList *children_scroll_mes_box = gtk_container_get_children(GTK_CONTAINER(children_scroll->data));

		GtkWidget * temp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		
		gtk_widget_set_halign(mes_user, GTK_ALIGN_START);
		gtk_box_pack_end(GTK_BOX(temp_box), mes_user, FALSE, FALSE, 0);
		if (!mx_strcmp(orientation, "temp_box-left"))
		{
			gtk_widget_set_halign(temp_box, GTK_ALIGN_START);
			add_element_to_css(temp_box, "temp_box-left");
		}
		else
		{
			gtk_widget_set_halign(temp_box, GTK_ALIGN_END);
			add_element_to_css(temp_box, "temp_box-right");

		}

		add_element_to_css(temp_box, "msg__style");

		gtk_box_pack_end(GTK_BOX(children_scroll_mes_box->data), temp_box, FALSE, FALSE, 0);
		gtk_box_reorder_child(GTK_BOX(children_scroll_mes_box->data), temp_box, 0);
		gtk_widget_show_all(temp_box);
		
		return;
	}
}


void mx_sticker_in_chat(char * sticker_path, char * orientation)
{
	if (gtk_stack_get_visible_child_name(GTK_STACK(stack)))
	{
		GtkWidget * sticker = gtk_button_new();
		GtkWidget * image = gtk_image_new_from_file(sticker_path);
		gtk_button_set_image(GTK_BUTTON(sticker), image);

		GtkWidget * temp = gtk_stack_get_visible_child((GtkStack *)stack);
		GList *children_box = gtk_container_get_children(GTK_CONTAINER(temp));
		GList *children_scroll_box = gtk_container_get_children(GTK_CONTAINER(children_box->data));
		GList *children_scroll = gtk_container_get_children(GTK_CONTAINER(children_scroll_box->data));
		GList *children_scroll_mes_box = gtk_container_get_children(GTK_CONTAINER(children_scroll->data));

		GtkWidget * temp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		
		gtk_widget_set_halign(sticker, GTK_ALIGN_START);
		gtk_box_pack_end(GTK_BOX(temp_box), sticker, FALSE, FALSE, 0);
		if (!mx_strcmp(orientation, "temp_box-left"))
		{
			gtk_widget_set_halign(temp_box, GTK_ALIGN_START);
			add_element_to_css(temp_box, "temp_box-left");
		}
		else
		{
			gtk_widget_set_halign(temp_box, GTK_ALIGN_END);
			add_element_to_css(temp_box, "temp_box-right");

		}

		// add_element_to_css(temp_box, "msg__style");

		gtk_box_pack_end(GTK_BOX(children_scroll_mes_box->data), temp_box, FALSE, FALSE, 0);
		gtk_box_reorder_child(GTK_BOX(children_scroll_mes_box->data), temp_box, 0);
		gtk_widget_show_all(temp_box);
		
		return;
	}
}


void change_current_chat_title(GtkButton *button_that_was_clicked, gchar * new_title)
{
	if (button_that_was_clicked) {}

	if (gtk_stack_get_visible_child_name(GTK_STACK(stack)))
	{
		mx_change_chat_title_button_clicked(new_title);

		gtk_stack_set_visible_child_name(GTK_STACK(stack), new_title);
		gtk_label_set_text(GTK_LABEL(statu_user_login), new_title);
	}
}


void convert_text()
{
	text4 = (char *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)serch_input));
	char * users_list = mx_search_button_clicked(text4);
	
	char ** users_list_arr = mx_strsplit(users_list, '|');
	int users_list_arr_size = mx_get_arr_size(users_list_arr);
	for (int i = 0; i < users_list_arr_size; i++)
	{
		char ** user_data = mx_strsplit(users_list_arr[i], ';');
		GtkWidget *button_user = gtk_button_new_with_label(user_data[1]);
		g_signal_connect(button_user, "clicked", G_CALLBACK(mx_request_chat_create), NULL);

		gtk_box_pack_start (GTK_BOX (vbox_poisk), button_user, FALSE, FALSE, 0);

		mx_del_strarr(&user_data);
	}

	gtk_widget_show_all (popover_poisk);
	mx_del_strarr(&users_list_arr);
}


void welcome()
{

	GtkWidget *azharkov, *tbortsov, *mdzyba, *hushkalo, *sklymenko, *khudzhuman, *information;

	real_quit = true;
    load_css();


    registr_window.registration_window = gtk_grid_new();
    gtk_widget_set_name(registr_window.registration_window, "myGrid");
    add_element_to_css(registr_window.registration_window, "registration_window");

    gtk_grid_set_column_spacing(GTK_GRID(registr_window.registration_window), 6);
    gtk_grid_set_row_spacing(GTK_GRID(registr_window.registration_window), 2);


    Window_Regist = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    add_element_to_css(Window_Regist, "window_Login");
    gtk_widget_set_size_request(Window_Regist, 600, 450);
    gtk_window_set_position(GTK_WINDOW(Window_Regist), GTK_WIN_POS_CENTER);



	information = gtk_label_new("Chat developers ");
	tbortsov = gtk_label_new("tbortsov ");
	mdzyba = gtk_label_new("mdzyba ");
	khudzhuman = gtk_label_new("khudzhuman ");
	azharkov = gtk_label_new("azharkov ");
	hushkalo = gtk_label_new("hushkalo ");
	sklymenko = gtk_label_new("sklymenko ");

	add_element_to_css(information, "text_welcome_main");
	add_element_to_css(tbortsov, "text_welcome");
	add_element_to_css(mdzyba, "text_welcome");
	add_element_to_css(khudzhuman, "text_welcome");
	add_element_to_css(azharkov, "text_welcome");
	add_element_to_css(hushkalo, "text_welcome");
	add_element_to_css(sklymenko, "text_welcome");

	gtk_grid_attach(GTK_GRID(registr_window.registration_window), khudzhuman,  1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), information,  1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), tbortsov,  1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), mdzyba,   1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), azharkov,  1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), hushkalo,  1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(registr_window.registration_window), sklymenko,  1, 6, 1, 1);


    gtk_container_add(GTK_CONTAINER(Window_Regist), registr_window.registration_window);
    gtk_window_set_modal(GTK_WINDOW(Window_Regist), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(Window_Regist), FALSE);

    gtk_widget_show_all(Window_Regist);

}


void display_setting()
{
	gtk_widget_show_all (popover);
}

void display_poisk()
{
	gtk_widget_show_all (popover_poisk);
}

void display_statu()
{
	gtk_widget_show_all (popover_statu);
}

GtkWidget *popover_stiker, *main_box_stiker, *box_stiker_veral, *box_stiker_veral2, *box_stiker_veral3;
GtkWidget *message_stiker_button;

void display_stiker()
{
	gtk_widget_show_all (popover_stiker);
}


void mx_create_chat_box(char * cur_user_name, char * chat_title)
{
	GtkWidget *message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);		 // бокс с сообщениями
	message_input = gtk_entry_new();	
	GtkWidget *message_send_button = gtk_button_new_with_label(""); // кнопка для сообщения
	add_element_to_css(message_send_button, "button__send");
	g_signal_connect(G_OBJECT(message_send_button), "clicked", G_CALLBACK(mx_send_message), message_input);

	GtkWidget * main_box_to_add = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // бокс для отображение всего вместе справа
	scroll_mes_window = gtk_scrolled_window_new(NULL, NULL); //скрол

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_mes_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS); //для свича
	gtk_container_add(GTK_CONTAINER(scroll_mes_window), message_box); // бокс где сообщение в скрол

	mes_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);						// бокс в которм лежит кнопка, ввод и бокс с сообщениями
	gtk_box_pack_start(GTK_BOX(mes_box), scroll_mes_window, TRUE, TRUE, 1); // свич  в бокс где наша кнопка и поле ввода

	gtk_box_pack_start(GTK_BOX(main_box_to_add), mes_box, TRUE, FALSE, 5); //  все ранее в бокс
	gtk_widget_set_size_request(mes_box, 400, 600);	// задаем размер боксу


	message_stiker_button = gtk_button_new(); 
	add_element_to_css(message_stiker_button, "message_stiker_button");

	g_signal_connect(G_OBJECT(message_stiker_button), "clicked", G_CALLBACK(display_stiker), NULL);
	popover_stiker = gtk_popover_new (message_stiker_button);
	main_box_stiker = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);

	box_stiker_veral = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);

	box_stiker_veral2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);

	box_stiker_veral3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);

	GtkWidget *stiker1 = gtk_button_new_with_label("");
	GtkWidget *stiker2= gtk_button_new_with_label("");
	GtkWidget *stiker3= gtk_button_new_with_label("");

	GtkWidget *stiker12 = gtk_button_new_with_label("");
	GtkWidget *stiker22= gtk_button_new_with_label("");
	GtkWidget *stiker32= gtk_button_new_with_label("");

	GtkWidget *stiker13 = gtk_button_new_with_label("");
	GtkWidget *stiker23= gtk_button_new_with_label("");
	GtkWidget *stiker33= gtk_button_new_with_label("");

	g_signal_connect(stiker1, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/1.png");
	g_signal_connect(stiker2, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/2.png");
	g_signal_connect(stiker3, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/3.png");

	g_signal_connect(stiker12, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/4.png");
	g_signal_connect(stiker22, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/5.png");
	g_signal_connect(stiker32, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/6.png");

	g_signal_connect(stiker13, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/7.png");
	g_signal_connect(stiker23, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/8.png");
	g_signal_connect(stiker33, "clicked", G_CALLBACK(mx_send_sticker), (gchar * )"client/img/stickers/9.png");

	gtk_box_pack_start (GTK_BOX (box_stiker_veral), stiker1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral), stiker2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral), stiker3, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (box_stiker_veral2), stiker12, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral2), stiker22, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral2), stiker32, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (box_stiker_veral3), stiker13, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral3), stiker23, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_stiker_veral3), stiker33, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(main_box_stiker), box_stiker_veral);
	gtk_container_add(GTK_CONTAINER(main_box_stiker), box_stiker_veral2);
	gtk_container_add(GTK_CONTAINER(main_box_stiker), box_stiker_veral3);

	add_element_to_css(stiker1, "stiker1");
	add_element_to_css(stiker2, "stiker2");
	add_element_to_css(stiker3, "stiker3");

	add_element_to_css(stiker12, "stiker12");
	add_element_to_css(stiker22, "stiker22");
	add_element_to_css(stiker32, "stiker32");

	add_element_to_css(stiker13, "stiker13");
	add_element_to_css(stiker23, "stiker23");
	add_element_to_css(stiker33, "stiker33");

	gtk_container_add (GTK_CONTAINER (popover_stiker), main_box_stiker);

	gtk_menu_button_set_popover (GTK_MENU_BUTTON (message_stiker_button), popover_stiker);
	// gtk_container_add (GTK_CONTAINER (message_stiker_button), gtk_label_new ("")); // этот лейбл помогает нам отображать картинку

	GtkWidget *box_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // нужен для ->ввод и отправка в одну строку 
	gtk_container_add(GTK_CONTAINER(box_time), message_input);
	gtk_widget_set_size_request(message_input, 900, 0);
	gtk_container_add(GTK_CONTAINER(box_time), message_stiker_button);
	gtk_container_add(GTK_CONTAINER(box_time), message_send_button);

	// тоже для отображение
	main_box_chat = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box_chat), box_time, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box_to_add), main_box_chat, TRUE, FALSE, 5);
	gtk_widget_set_size_request(main_box_chat, 500, 100);
	gtk_box_pack_start(GTK_BOX(ChatWindow), main_box_to_add, TRUE, FALSE, 5);

	// Chat could be named like this: Apach-Vilsan
	// If so, than it's a private chat and we need to display only the others user name

	char ** temp = mx_strsplit(chat_title, '-');
	if (temp[1] != NULL)
	{
		// If first element of private chat name is our cur_user_name then
		// we want to display the secon user name

		if (!mx_strcmp(temp[0], cur_user_name))
		{
			pages[last_chat] = mx_strdup(temp[1]);
		}
		else
		{
			pages[last_chat] = mx_strdup(temp[0]);
		}
	}
	else
	{
		pages[last_chat] = mx_strdup(chat_title);
	}

	widget = main_box_to_add;
	add_element_to_css(stack, "bolshoi_tekst_chat");
	gtk_stack_add_named(GTK_STACK(stack), widget, chat_title);
	gtk_container_child_set(GTK_CONTAINER(stack), widget, "title", pages[last_chat], NULL);
	last_chat++;

	gtk_widget_show_all(widget);

}


void mx_ChatWindow()
{
 real_quit = true;
 load_css();

 GtkWidget *statu_user_login = gtk_label_new("No Name");
 
 GtkWidget *image_user = gtk_button_new_with_label("");;
 
 serch_input = gtk_entry_new();

 GtkWidget *poisk = gtk_button_new_with_label("");

 pages[0] = NULL;

 ChatWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 gtk_window_set_resizable(GTK_WINDOW(ChatWindow), TRUE);
 gtk_window_set_title(GTK_WINDOW(ChatWindow), "Ucat Chat");
 gtk_widget_set_size_request(ChatWindow, 1240, 720);
 gtk_window_set_position(GTK_WINDOW(ChatWindow), GTK_WIN_POS_CENTER_ALWAYS);

 main_repository = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

 setting_button = gtk_menu_button_new();
 g_signal_connect(G_OBJECT(setting_button), "clicked", G_CALLBACK(display_setting), NULL);
 popover = gtk_popover_new(setting_button);
 vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

 setings_user = gtk_button_new_with_label("Your setings");
 g_signal_connect(G_OBJECT(setings_user), "clicked", G_CALLBACK(mx_setUser), NULL);
 about_us = gtk_button_new_with_label("About ASS");
 g_signal_connect(G_OBJECT(about_us), "clicked", G_CALLBACK(welcome), NULL);
 log_out = gtk_button_new_with_label("Log Out");
 g_signal_connect(G_OBJECT(log_out), "clicked", G_CALLBACK(mx_logout_button_clicked), NULL);

 gtk_box_pack_start(GTK_BOX(vbox), setings_user, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), about_us, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), log_out, FALSE, FALSE, 0);
 gtk_container_add(GTK_CONTAINER(popover), vbox);

 gtk_menu_button_set_popover(GTK_MENU_BUTTON(setting_button), popover);
 gtk_container_add(GTK_CONTAINER(setting_button), gtk_label_new(""));
 gtk_widget_show_all(setting_button);
 
 poisk = gtk_menu_button_new();

 g_signal_connect(G_OBJECT(poisk), "clicked", G_CALLBACK(convert_text), serch_input);
 g_signal_connect(G_OBJECT(poisk), "clicked", G_CALLBACK(display_poisk), NULL);

 popover_poisk = gtk_popover_new(poisk);
 vbox_poisk = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

 gtk_container_add(GTK_CONTAINER(popover_poisk), vbox_poisk);

 gtk_menu_button_set_popover(GTK_MENU_BUTTON(poisk), popover_poisk);
 gtk_container_add(GTK_CONTAINER(poisk), gtk_label_new(""));
 gtk_widget_show_all(poisk);


 GtkWidget *statu_user = gtk_button_new_with_label(""); // кнопка справа сверху
 gtk_button_set_relief(GTK_BUTTON(statu_user), GTK_RELIEF_NONE);

 g_signal_connect(G_OBJECT(statu_user), "clicked", G_CALLBACK(display_statu), NULL);
 popover_statu = gtk_popover_new(statu_user);
 add_element_to_css(popover_statu, "popover_statu");
 vbox_statu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

 GtkWidget *add_user = gtk_button_new_with_label("Add User"); // это неадо сделать красным
 g_signal_connect(G_OBJECT(add_user), "clicked", G_CALLBACK(mx_add_user_to_chat_button_clicked), serch_input);
 GtkWidget *change_chat_name = gtk_button_new_with_label("Chat name");
 g_signal_connect(G_OBJECT(change_chat_name), "clicked", G_CALLBACK(welcome), NULL);

 gtk_box_pack_start(GTK_BOX(vbox_statu), add_user, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(vbox_statu), change_chat_name, FALSE, FALSE, 0);
 gtk_container_add(GTK_CONTAINER(popover_statu), vbox_statu);

 gtk_menu_button_set_popover(GTK_MENU_BUTTON(statu_user), popover_statu);
 gtk_container_add(GTK_CONTAINER(statu_user), gtk_label_new(""));
 gtk_widget_show_all(statu_user);


 GtkWidget *status_user_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
 gtk_container_add(GTK_CONTAINER(status_user_box), image_user);
 gtk_container_add(GTK_CONTAINER(status_user_box), statu_user_login);
 gtk_container_add(GTK_CONTAINER(status_user_box), statu_user);


 bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
 gtk_container_add(GTK_CONTAINER(bar), setting_button);
 gtk_container_add(GTK_CONTAINER(bar), serch_input);
 gtk_container_add(GTK_CONTAINER(bar), poisk);
 gtk_container_add(GTK_CONTAINER(bar), status_user_box);

 // gtk_info_bar_set_message_type ((GtkInfoBar *)bar, GTK_MESSAGE_QUESTION);

 GtkWidget *list_user = gtk_stack_sidebar_new();
 add_element_to_css(list_user, "widget");
 gtk_widget_set_size_request(list_user, 305, 720); // тот самый бар слева

 stack = gtk_stack_new();
 gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_NONE); // Эффект переключения вверх
 gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(list_user), GTK_STACK(stack));

 user_repository = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
 gtk_box_pack_start(GTK_BOX(user_repository), list_user, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(user_repository), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(user_repository), stack, TRUE, TRUE, 0);

 // МОЕ ТУТ

 g_signal_connect(ChatWindow, "destroy", G_CALLBACK(mx_exit_button_clicked), NULL);

 timer = g_timeout_add_seconds(1.0, mx_check_new_message, ChatWindow);

 // бокс для светлой темной темы
 GtkWidget *hahah1 = gtk_label_new("Hi! Choose chat to start messaging :)");
 GtkWidget *switch_tema = gtk_switch_new();

 // g_signal_connect(switch_tema, "state-changed", G_CALLBACK (mx_exit_button_clicked), NULL);
 g_signal_connect(GTK_SWITCH(switch_tema), "notify::active", G_CALLBACK(mx_swap_dark_mode), NULL);

 gtk_widget_set_valign(switch_tema, GTK_ALIGN_CENTER);

 box_thema = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); // бокс для оснвого окна
 gtk_box_pack_start(GTK_BOX(box_thema), hahah1, TRUE, FALSE, 5);
 gtk_box_pack_start(GTK_BOX(box_thema), switch_tema, TRUE, FALSE, 5);
 gtk_widget_set_size_request(box_thema, 500, 100);

 widget = box_thema;
 // допустим тут ебать картинка ок да раземром 150 шоле
 gtk_stack_add_named(GTK_STACK(stack), widget, pages[0]);
 gtk_container_child_set(GTK_CONTAINER(stack), widget, "title", pages[0], NULL);

 // для отображение вообще всего в окне
 gtk_box_pack_start(GTK_BOX(main_repository), bar, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(main_repository), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(main_repository), user_repository, FALSE, FALSE, 0);
 gtk_container_add(GTK_CONTAINER(ChatWindow), main_repository);

 //Кнопки настройки
 gtk_button_set_relief(GTK_BUTTON(setings_user), GTK_RELIEF_NONE);
 gtk_button_set_relief(GTK_BUTTON(about_us), GTK_RELIEF_NONE);
 gtk_button_set_relief(GTK_BUTTON(log_out), GTK_RELIEF_NONE);
 add_element_to_css(setting_button, "setting_button");
 add_element_to_css(popover, "border");
 add_element_to_css(vbox, "box");
 add_element_to_css(setings_user, "setings_user");
 add_element_to_css(about_us, "about_us");
 add_element_to_css(log_out, "log_out");
  add_element_to_css(statu_user_login, "statu_user_login");
 //================

 //стили
 add_element_to_css(image_user, "image_user");
 add_element_to_css(statu_user, "status__user");
 add_element_to_css(poisk, "poisk");
 add_element_to_css(serch_input, "serch__input");
 add_element_to_css(setting_button, "ButtonStatus");
 add_element_to_css(setting_button, "setting__button");

 add_element_to_css(bar, "bar__style");
 add_element_to_css(user_repository, "main__background");
 add_element_to_css(main_repository, "user__list");
 add_element_to_css(main_box_mes, "user__list");
 add_element_to_css(statu_user, "status__button");

 gtk_widget_show_all(ChatWindow);
}

