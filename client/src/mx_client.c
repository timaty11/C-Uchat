#include "../inc/client.h"

char * message_text = NULL;

int cliSock, numbytes;
char buf[MAXDATASIZE], msg[MAXDATASIZE];
struct sockaddr_in dest_addr;

struct hostent *he;
pid_t pid;

SSL_CTX *ctx;
SSL *ssl;

int last_message_id = 0, cur_chat_id = 1;
int user_id;
char * cur_user_name = NULL;


gboolean mx_check_new_message(gpointer ptr)
{
    if (ptr){}

    if (!gtk_stack_get_visible_child_name(GTK_STACK(stack)))
    {
        return G_SOURCE_CONTINUE;
    }

    char * request = mx_strdup("DB_CHECK_LAST_MESSAGE ");
    char response[MAXDATASIZE];
    mx_strcat(request, mx_itoa(cur_chat_id));
    mx_strcat(request, " ");
    mx_strcat(request, mx_itoa(last_message_id));

    SSL_write(ssl, request, strlen(request));


    int bytes = SSL_read(ssl, response, sizeof(response));
    response[bytes] = '\0';
    printf("Received: \"%s\"\n", response);


    if (!strcmp(response, "RESPONSE: 0"))
    {
        printf("ERRORR. RETRYING\n");
        mx_check_new_message(ptr);
        return G_SOURCE_CONTINUE;
    }
    else if (!strcmp(response, "RESPONSE 0: NO MESSAGES FOUND"))
    {
        return G_SOURCE_CONTINUE;
    }
    else
    {
        mx_printstr("hyi2\n");

        char **response_arr = mx_strsplit(response, '|');
        int response_arr_size = mx_get_arr_size(response_arr);
        
        for (int i = response_arr_size - 1; i >= 0; i--)
        {
            mx_printstr("responza \n");
            mx_printstr(response_arr[i]);
            char ** message_data_arr = mx_strsplit(response_arr[i], ';');
            
            if (!mx_strcmp(message_data_arr[3], "TEXT"))
            {
                if (!mx_strcmp(message_data_arr[1], cur_user_name))
                {
                    text_in_Chat(message_data_arr[4], "temp_box-right");
                }
                else
                {
                    text_in_Chat(message_data_arr[4], "temp_box-left");
                }
            }
            else if (!mx_strcmp(message_data_arr[3], "STICKER"))
            {
                if (!mx_strcmp(message_data_arr[1], cur_user_name))
                {
                    mx_sticker_in_chat(message_data_arr[4], "temp_box-right");
                }
                else
                {
                    mx_sticker_in_chat(message_data_arr[4], "temp_box-left");
                }
            }

            last_message_id = mx_atoi(message_data_arr[0]);
            mx_printstr("pizdahyi\n");
        }


        return G_SOURCE_CONTINUE;
    }

    return G_SOURCE_CONTINUE;
}


void mx_get_chat_messages(GtkWidget *stack)
{
    // Get opened chat messages
    
    int bytes;
    char response[MAXDATASIZE];
    char ** response_arr;

	// LOAD LAST 50 OR LESS MESSAGES
    char *to_send1 = mx_strdup("DB_LOAD_50_MESS ");
    
    if (!gtk_stack_get_visible_child_name(GTK_STACK(stack)))
    {
        return;
    }

    gtk_label_set_text(GTK_LABEL(statu_user_login), gtk_stack_get_visible_child_name(GTK_STACK(stack)));

    char *to_send = mx_strnew(MAXDATASIZE);
    mx_strcat(to_send, "GET_CHAT_ID_BY_NAME ");
    mx_strcat(to_send, gtk_stack_get_visible_child_name(GTK_STACK(stack)));
    SSL_write(ssl, to_send, mx_strlen(to_send));
    mx_strdel(&to_send);

    bytes = SSL_read(ssl, response, sizeof(response));
    response[bytes] = '\0';
    printf("Received: \"%s\"\n", response);
    
    response_arr = mx_strsplit(response, '|');
    cur_chat_id = mx_atoi(response_arr[1]);

    mx_del_strarr(&response_arr);

    mx_strcat(to_send1, mx_itoa(cur_chat_id));

	printf("EBAT PIDOR - %s\nCUR CHAT_ID = %i\n", to_send1, cur_chat_id);


	SSL_write(ssl, to_send1, strlen(to_send1));

	bytes = SSL_read(ssl, response, sizeof(response));
	response[bytes] = '\0';
	printf("Received: \"%s\"\n", response);

	if (mx_strcmp(response, "RESPONSE 0: NO MESSAGES FOUND"))
	{
		mx_printstr("hyi3\n");
		response_arr = mx_strsplit(response, '|');
		int response_arr_size = mx_get_arr_size(response_arr);
		for (int i = response_arr_size - 1; i >= 0; i--)
		{
			char ** message_data_arr = mx_strsplit(response_arr[i], ';');
			
            if (!mx_strcmp(message_data_arr[3], "TEXT"))
            {
                if (!mx_strcmp(message_data_arr[1], cur_user_name))
                {
                    text_in_Chat(message_data_arr[4], "temp_box-right");
                }
                else
                {
                    text_in_Chat(message_data_arr[4], "temp_box-left");
                }
            }
            else if (!mx_strcmp(message_data_arr[3], "STICKER"))
            {
                if (!mx_strcmp(message_data_arr[1], cur_user_name))
                {
                    mx_sticker_in_chat(message_data_arr[4], "temp_box-right");
                }
                else
                {
                    mx_sticker_in_chat(message_data_arr[4], "temp_box-left");
                }
            }
			
			last_message_id = mx_atoi(message_data_arr[0]);
            mx_del_strarr(&message_data_arr);
		}
	}
}


void mx_request_chat_create(GtkButton *button_that_was_clicked, gchar * user_to_add)
{
    if (!user_to_add)
    {
        user_to_add = (gchar *) gtk_button_get_label(button_that_was_clicked);
    }

    char *to_send1 = mx_strdup("CHAT_CREATE ");
	mx_strcat(to_send1, cur_user_name);
    mx_strcat(to_send1, "-");
    mx_strcat(to_send1, user_to_add);
    
    mx_strcat(to_send1, ";");
    mx_strcat(to_send1, cur_user_name);
    mx_strcat(to_send1, ",");
    mx_strcat(to_send1, user_to_add);

	printf("CLIENT SEND - %s\n", to_send1);

	SSL_write(ssl, to_send1, strlen(to_send1));

    char response[MAXDATASIZE];
    int bytes = SSL_read(ssl, response, sizeof(response));
	response[bytes] = '\0';
	printf("Received: \"%s\"\n", response);

    if (!strncmp(response, "RESPONSE: 200", 13))
    {
        mx_printstr("hyi1\n");
        char ** response_arr = mx_strsplit(response, '|');
        mx_printstr("hyi2\n");
        cur_chat_id = mx_atoi(response_arr[1]);
        mx_printstr("hyi3\n");
        
        mx_create_chat_box(cur_user_name, user_to_add);
    }
}


void login_button_clicked()
{
    char * login = (char *)gtk_entry_get_text(GTK_ENTRY(grid_widget.login_entry));
    char * pass = (char *)gtk_entry_get_text(GTK_ENTRY(grid_widget.password_entry));


    if(mx_chek_login() == 0)
    {

        add_element_to_css(grid_widget.login_entry, "erore");      
    }
    else if(mx_chek_login() == 1)
    {
        add_element_to_css(grid_widget.login_entry, "correct");
    }
    
    if(mx_chek_password() == 0)
    {
        add_element_to_css(grid_widget.password_entry, "erore"); 
    }
    else
    {
        add_element_to_css(grid_widget.password_entry, "correct");
    }


    if (!logged_in)
    {
        real_quit = false;

        char *to_send = mx_strdup("LOGIN ");
        mx_strcat(to_send, login);
        mx_strcat(to_send, " ");
        mx_strcat(to_send, pass);

        SSL_write(ssl, to_send, strlen(to_send));

        printf("Client sent %s to %s\n", to_send, inet_ntoa(dest_addr.sin_addr));

        for (int i = 0; i < MAXDATASIZE; i++)
        {
            buf[i] = '\0';
        }

        int bytes = SSL_read(ssl, buf, sizeof(buf));
        buf[bytes] = '\0';
        printf("Received: \"%s\"\n", buf);

        if (!strncmp(buf, "RESPONSE: 200 LOGGED SUCCESFULLY|", 33))
        {


            logged_in = true;
            real_quit = false;
            gtk_widget_destroy(LoginWindow);
            mx_ChatWindow();
            

            char **response_arr = mx_strsplit(buf, '|');
            user_id = mx_atoi(response_arr[1]);
            cur_user_name = mx_strdup(response_arr[2]);
            mx_del_strarr(&response_arr);

            char *to_send1 = mx_strdup("LOAD_CHATS ");
            mx_strcat(to_send1, mx_itoa(user_id));

            SSL_write(ssl, to_send1, strlen(to_send1));

            printf("Client sent %s to %s\n", to_send1, inet_ntoa(dest_addr.sin_addr));

            for (int i = 0; i < MAXDATASIZE; i++)
            {
                buf[i] = '\0';
            }

            int bytes = SSL_read(ssl, buf, sizeof(buf));
            buf[bytes] = '\0';
            printf("Received: \"%s\"\n", buf);

            // TO DO: CREATE ALL RECEIVED CHATS AND DISPLAY THEM ON THE SCREEN
            
            if (!mx_strcmp(buf, "RESPONSE 0: NO MESSAGES FOUND"))
            {
                return;
            }

            response_arr = mx_strsplit(buf, '|');
            int response_arr_size = mx_get_arr_size(response_arr);
            for (int i = 0; i < response_arr_size; i++)
            {
                char ** chat_data = mx_strsplit(response_arr[i], ';');
                mx_create_chat_box(cur_user_name, chat_data[1]);
                mx_del_strarr(&chat_data);
            }
        }
        else
        {
            real_quit = false;

            if (mx_strcmp(buf,"RESPONSE: 0 NO SUCH USER") == 0)
            {
                add_element_to_css(grid_widget.login_entry, "erore_login");
            }
            if (mx_strcmp(buf,"RESPONSE: 0 INCORRECT PASSWORD") == 0)
            {
                add_element_to_css(grid_widget.password_entry, "erore_login"); 
            }

            return;
        }
    }
}


void register_button_clicked()
{
    char * login = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.login_entry));
    char * name = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.name_entry));
    char * pass = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.pass_entry));
    char * pass_confirm = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.pass_true_entry));

    if(mx_chek_login_registr() == 0)
    {
        add_element_to_css(registr_window.login_entry, "erore");
    }
    else
    {
        add_element_to_css(registr_window.login_entry, "correct");
    }
    
    if(mx_chek_password_registr() == 0)
    {
        add_element_to_css(registr_window.pass_entry, "erore");
    }
    else
    {
        add_element_to_css(registr_window.pass_entry, "correct");
    }

    if(mx_chek_name_entry() == 0)
    {
        add_element_to_css(registr_window.name_entry, "erore");
    }
    else
    {
        add_element_to_css(registr_window.name_entry, "correct");
    }
    
    if(mx_chek_pass_true_entry() == 0)
    {
        add_element_to_css(registr_window.pass_true_entry, "erore");
    }
    else
    {
        add_element_to_css(registr_window.pass_true_entry, "correct");
    }

    if (mx_strcmp(pass, pass_confirm))
    {
        mx_printstr("PASS AND PASS CONFIRM MUST BE IDENTICAL\n");
        return;
    }
    
    char *to_send = mx_strdup("CREATE ");
    mx_strcat(to_send, login);
    mx_strcat(to_send, " ");
    mx_strcat(to_send, name);
    mx_strcat(to_send, " ");
    mx_strcat(to_send, pass);

    SSL_write(ssl, to_send, strlen(to_send));
    printf("Client sent %s to %s\n", to_send, inet_ntoa(dest_addr.sin_addr));

    for (int i = 0; i < MAXDATASIZE; i++)
    {
        buf[i] = '\0';
    }

    
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    

    if (!strcmp(buf, "RESPONSE: 200 CREATED USER"))
    {
        real_quit = false;
        gtk_widget_destroy(Window_Regist);
        mx_LoginWindow();
    }
    else
    {
        real_quit = false;
        return;
    }

    return;
}


char * mx_search_button_clicked(char * user_name)
{
    char *to_send = mx_strdup("LOAD_USERS ");
    mx_strcat(to_send, user_name);

    SSL_write(ssl, to_send, strlen(to_send));

    printf("Client sent %s to %s\n", to_send, inet_ntoa(dest_addr.sin_addr));

    char * response = mx_strnew(MAXDATASIZE);
    int bytes = SSL_read(ssl, response, MAXDATASIZE);
    response[bytes] = '\0';
    printf("Received: \"%s\"\n", response);

    return response;
}


void mx_send_file_button_clicked()
{
    SSL_write(ssl, "SEND_FILE", strlen("SEND_FILE"));
    mx_send_file(ssl);

}


void mx_change_chat_title_button_clicked(char * new_title)
{
    char * request = mx_strnew(MAXDATASIZE);
    mx_strcat(request, "CHANGE_CHAT_TITLE:");
    mx_strcat(request, mx_itoa(cur_chat_id));
    mx_strcat(request, ";");
    mx_strcat(request, new_title);

    printf("sendin: %s\n", request);
    SSL_write(ssl, request, MAXDATASIZE);

    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!mx_strcmp(buf, "RESPONSE: 200"))
    {

    }
}


void change_something()
{
    char * new_name = (char *)gtk_entry_get_text(GTK_ENTRY(entry_login));
    char * old_pass = (char *)gtk_entry_get_text(GTK_ENTRY(entry_pass));
    char * new_pass = (char *)gtk_entry_get_text(GTK_ENTRY(entry_pass_true));

    if (new_name && new_name[0] != '\0')
    {
        change_current_user_pass(new_name, NULL);
    }

    if (old_pass && old_pass[0] != '\0')
    {
        change_current_user_pass(old_pass, new_pass);
    }
}


void change_current_user_pass(char * old_pass, char * new_pass)
{
	char * request = mx_strnew(MAXDATASIZE);
    mx_strcat(request, "CHANGE_PASS:");
    mx_strcat(request, old_pass);
    mx_strcat(request, ";");
    mx_strcat(request, new_pass);

    printf("sendin: %s\n", request);
    SSL_write(ssl, request, MAXDATASIZE);

    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!mx_strcmp(buf, "RESPONSE: 200"))
    {

    }
    else
    {
        printf("hyita1234567890\n");
    }
}


void mx_exit_button_clicked()
{
    if (real_quit)
    {
        
        SSL_write(ssl, "DISCONNECT", strlen("DISCONNECT"));
        printf("Client sent DISCONNECT to %s\n", inet_ntoa(dest_addr.sin_addr));

        int sd = SSL_get_fd(ssl);   /* get socket connection */
        SSL_free(ssl);  /* release SSL state */
        close(sd);  /* close connection */

        gtk_main_quit();
        exit(1);
    }
}


void mx_logout_button_clicked()
{
    g_source_remove(timer);
    SSL_write(ssl, "LOGOUT", 8);
    printf("Client sent LOGOUT to %s\n", inet_ntoa(dest_addr.sin_addr));

    for (int i = 0; i < MAXDATASIZE; i++)
    {
        buf[i] = '\0';
    }
    
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!strcmp(buf, "RESPONSE: 200 LOGGED OUT SUCCESSFULLY"))
    {
        real_quit = false;
        logged_in = false;
        gtk_widget_destroy(ChatWindow);
        mx_LoginWindow();
    }
}


void mx_add_user_to_chat_button_clicked(GtkButton *button_that_was_clicked, GtkWidget * input)
{
    if (button_that_was_clicked) {}

    char * user_name = (char *)gtk_entry_get_text(GTK_ENTRY(input));
    if (!user_name || user_name[0] == '\0')
    {
        return;
    }

    char * request = mx_strnew(MAXDATASIZE);
    mx_strcat(request, "ADD_USER_TO_CHAT ");
    mx_strcat(request, mx_itoa(cur_chat_id));
    mx_strcat(request, " ");
    mx_strcat(request, user_name);

    SSL_write(ssl, request, mx_strlen(request));
    printf("Client sent %s to %s\n", request, inet_ntoa(dest_addr.sin_addr));

    for (int i = 0; i < MAXDATASIZE; i++)
    {
        buf[i] = '\0';
    }
    
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!strncmp(buf, "RESPONSE: 200 ADDED USER SUCCESSFULLY", 37))
    {
        char ** response_data_arr = mx_strsplit(buf, '|');
        char * message_to_display = mx_strnew(256);
        mx_strcat(message_to_display, cur_user_name);
        mx_strcat(message_to_display, " has added ");
        mx_strcat(message_to_display, response_data_arr[1]);
        mx_strcat(message_to_display, "to chat!");

        text_in_Chat(message_to_display, "temp_box-right");
    }
}


void mx_delete_message_button_clicked(GtkButton *button_that_was_clicked, gchar * mes_to_delete)
{
    if (!button_that_was_clicked) {}
    if (!mes_to_delete) {}

    char *to_send = mx_strdup("DELETE_MESSAGE ");
    mx_strcat(to_send, mx_itoa(last_message_id));

    SSL_write(ssl, to_send, strlen(to_send));

    printf("Client sent %s to %s\n", to_send, inet_ntoa(dest_addr.sin_addr));

    char * response = mx_strnew(MAXDATASIZE);
    int bytes = SSL_read(ssl, response, MAXDATASIZE);
    response[bytes] = '\0';
    printf("Received: \"%s\"\n", response);

    if (!mx_strcmp(response, "RESPONSE 200: DELETED SUCCESSFULY"))
    {
        gtk_widget_destroy(temp_box);
        last_message_id--;
    }
}


int main(int argc, char *argv[])
{
    logged_in = false;
    real_quit = true;

    gtk_init(&argc, &argv);
    SSL_library_init();
    First_Winsow();
    // mx_LoginWindow();
    if (argc != 3)
    {
        printf("ERROR: hostname + port required\n");
        exit(0);
    }

    if ((he = gethostbyname(argv[1])) == NULL)
    {
        printf("Couldn't get hostname\n");
        exit(1);
    }

    if ((cliSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket Error: %d\n", errno);
    }
    else
    {
        printf("Client Socket %d created\n", cliSock);
        dest_addr.sin_family = PF_INET;
        dest_addr.sin_port = htons(atoi(argv[2]));
        dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
    }

    if (connect(cliSock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) != 0)
    {
        printf("Connect Error: %d\n", errno);
    }
    else
    {
        printf("Client Connection created\n");

        char ServerResponse[1024];
        int bytes;
        
        ctx = mx_init_CTX();
        ssl = SSL_new(ctx);

        generate_keys();

        if (SSL_set_fd(ssl, cliSock) != 1)
        {
            ERR_print_errors_fp(stderr);
        }
        
        if (SSL_connect(ssl) != 1)  // perform the connection
        {
            ERR_print_errors_fp(stderr);
        }
        else
        {
            //// SEND SERVER CLIENTS PUBLIC CODE
            char * RequestMessage = "GET SERVER_PUBKEY";
            
            printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
            // ShowCerts(ssl);
            SSL_write(ssl, RequestMessage, strlen(RequestMessage)); // Encrypt and send message

            bytes = SSL_read(ssl, ServerResponse, sizeof(ServerResponse));  /* Get reply & decrypt */
            ServerResponse[bytes] = '\0';
            printf("Received: \"%s\"\n", ServerResponse);

            char * MyPubKey = mx_file_to_str("client/public.pem"); // Prepare Clients pubkey to send to server
            SSL_write(ssl, MyPubKey, strlen(MyPubKey));

        }
    }

    gtk_main ();

    return 0;
}


void mx_send_message(GtkWidget * widget, GtkWidget * input)
{
    if (widget) {}
    
    message_text = mx_strdup((char *)gtk_entry_get_text(GTK_ENTRY(input))); 
    
    gtk_entry_set_text(GTK_ENTRY(input), "");
    if (!message_text || message_text[0] == '\0')
    {

        return;
    }

    char *request = mx_strdup("SEND ");
    mx_strcat(request, mx_itoa(cur_chat_id));
    mx_strcat(request, " ");
    mx_strcat(request, message_text);

    SSL_write(ssl, request, strlen(request));
    printf("Client sent %s to %s\n", request, inet_ntoa(dest_addr.sin_addr));

    for (int i = 0; i < MAXDATASIZE; i++)
    {
        buf[i] = '\0';
    }

    
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!strcmp(buf, "RESPONSE: 200"))
    {
        return;
    }
    else
    {
        return;
    }

    return;
}


void mx_send_sticker(GtkWidget * widget, gchar * sticker_path)
{
    if (widget) {}

    char *request = mx_strdup("SEND_STICKER ");
    mx_strcat(request, mx_itoa(cur_chat_id));
    mx_strcat(request, " ");
    mx_strcat(request, sticker_path);

    SSL_write(ssl, request, strlen(request));
    printf("Client sent %s to %s\n", request, inet_ntoa(dest_addr.sin_addr));

    for (int i = 0; i < MAXDATASIZE; i++)
    {
        buf[i] = '\0';
    }
    
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    printf("Received: \"%s\"\n", buf);

    if (!strcmp(buf, "RESPONSE: 200"))
    {
        return;
    }
    else
    {
        return;
    }

    return;
}

