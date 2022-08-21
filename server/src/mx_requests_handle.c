#include "../inc/server.h"


void mx_CreateUserHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    char **comand_arr = mx_strsplit(buf, ' '); // мать ебал
    int comand_arr_size = mx_get_arr_size(comand_arr);
    if (comand_arr_size != 4)
    {
        mx_printstr("INCORRECT ELEMENTS AMOUNT TO REGISTER USER\n");
        // send(cliProc, "RESPONSE: 0", strlen(msg), 0);
        SSL_write(ssl, "RESPONSE: 0", strlen("RESPONSE: 0")); /* send reply */
    }
    else
    {
        int result = mx_check_input(comand_arr[1], comand_arr[2], comand_arr[3]);
        if (result == 1)
        {
            mx_add_user(comand_arr[1], comand_arr[2], comand_arr[3]);
            // send(cliProc, "RESPONSE: 200 CREATED USER", strlen(msg), 0);
            SSL_write(ssl, "RESPONSE: 200 CREATED USER", strlen("RESPONSE: 200 CREATED USER"));
        }
        else if (result == 2)
        {
            mx_printstr("ERROR: FOUND SAME USER\n");
            // send(cliProc, "RESPONSE: 0 FOUND SAME USER\n", strlen("RESPONSE: 0 FOUND SAME USER\n"), 0);
            SSL_write(ssl, "RESPONSE: 0 FOUND SAME USER", strlen("RESPONSE: 0 FOUND SAME USER"));
        }
        else if (result == 3)
        {
            mx_printstr("ERROR: FOUND FORBIDDEN WORDS\n");
            // send(cliProc, "RESPONSE: 0 FOUND FORBIDDEN WORDS\n", strlen("RESPONSE: 0 FOUND FORBIDDEN WORDS\n"), 0);
            SSL_write(ssl, "RESPONSE: 0 FOUND FORBIDDEN WORDS", strlen("RESPONSE: 0 FOUND FORBIDDEN WORDS"));
        }
        else if (result == 4)
        {
            mx_printstr("ERROR: TO EASY PASS\n");
            // send(cliProc, "RESPONSE: 0 TO EASY PASS\n", strlen("RESPONSE: 0 TO EASY PASS\n"), 0);
            SSL_write(ssl, "RESPONSE: 0 TO EASY PASS", strlen("RESPONSE: 0 TO EASY PASS"));
        }
    }
}


bool mx_LoginUserHandle(char buf[MAXDATASIZE], SSL *ssl, int *user_id)
{
    char **comand_arr = mx_strsplit(buf, ' ');
    int comand_arr_size = mx_get_arr_size(comand_arr);
    if (comand_arr_size != 3)
    {
        mx_printstr("INCORRECT ELEMENTS AMOUNT TO LOGIN\n");
        // send(cliProc, "RESPONSE: 0 INCORRECT ELEMENTS AMOUNT TO LOGIN", strlen(msg), 0);
        SSL_write(ssl, "RESPONSE: 0 INCORRECT ELEMENTS AMOUNT TO LOGIN", strlen("RESPONSE: 0 INCORRECT ELEMENTS AMOUNT TO LOGIN"));
        return false;
    }
    else
    {
        char * cur_user_name = NULL;
        int result = mx_db_login(comand_arr[1], comand_arr[2], &cur_user_name);
        if (result == 1)
        {
            *user_id = mx_get_user_id(comand_arr[1]);

            char * response = mx_strnew(MAXDATASIZE);
            mx_strcat(response, "RESPONSE: 200 LOGGED SUCCESFULLY|");
            mx_strcat(response, mx_itoa(*user_id));
            mx_strcat(response, "|");
            mx_strcat(response, cur_user_name);
            SSL_write(ssl, response, strlen(response));
            
            return true;
         
        }
        else if (result == 2)
        {
            mx_printstr("ERROR: NO SUCH USER\n");
            // send(cliProc, "RESPONSE: 0 NO SUCH USER\n", strlen("RESPONSE: 0 NO SUCH USER\n"), 0);
            SSL_write(ssl, "RESPONSE: 0 NO SUCH USER", strlen("RESPONSE: 0 NO SUCH USER"));
            // loged_in = false;
            return false;
        }
        else if (result == 3)
        {
            mx_printstr("ERROR: INCORRECT PASSWORD\n");
            // send(cliProc, "RESPONSE: 0 INCORRECT PASSWORD\n", strlen("RESPONSE: 0 INCORRECT PASSWORD\n"), 0);
            SSL_write(ssl, "RESPONSE: 0 INCORRECT PASSWORD", strlen("RESPONSE: 0 INCORRECT PASSWORD"));
            // loged_in = false;
            return false;
        }
        return false;
    }
}


void mx_UserChangeNameHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ':');
    char **pass_data = mx_strsplit(request_arr[1], ';');

    char * db_response = mx_change_user_pass(pass_data[0], pass_data[1], pass_data[1]);

    SSL_write(ssl, db_response, MAXDATASIZE);
}


void mx_UserChangePassHandle(char buf[MAXDATASIZE], SSL *ssl, int user_id)
{
    char **request_arr = mx_strsplit(buf, ':');
    char **pass_data = mx_strsplit(request_arr[1], ';');

    char * db_response = mx_change_user_pass(mx_itoa(user_id), pass_data[0], pass_data[1]);

    SSL_write(ssl, db_response, MAXDATASIZE);
}


void mx_DisconnectHandle(SSL *ssl, int cliProc, int servSock)
{
    SSL_write(ssl, "RESPONSE: 0", strlen("RESPONSE: 0"));
    close(cliProc);
    close(servSock);
    printf("Server Sockets closed\n");
    
    kill(getpid(), SIGKILL);
    exit(0);
}


void mx_FindUserHandler(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ' ');


    char * response = mx_get_user_by_name(request_arr[1]);

    SSL_write(ssl, response, MAXDATASIZE);
}


void mx_MessageSendHandle(char buf[MAXDATASIZE], SSL *ssl, int user_id, char * data_type)
{
    char ** sticker_data = mx_strsplit(buf, ' ');

    if (!mx_strcmp(data_type, "TEXT"))
    {
        mx_db_add_message(sticker_data[1], mx_itoa(user_id), "TEXT", &buf[7], NULL); // return 1 on success
    }
    else if (!mx_strcmp(data_type, "STICKER"))
    {
        mx_db_add_message(sticker_data[1], mx_itoa(user_id), "STICKER", NULL, sticker_data[2]); // return 1 on success
    }
    

    //// IF CORRECT GO SEND RESPONSE
    SSL_write(ssl, "RESPONSE: 200", MAXDATASIZE);
}


void mx_FileSendHandle(SSL *ssl)
{
    mx_receive_file(ssl);

    // mx_db_add_message(chat_id_temp, mx_itoa(user_id), &buf[7]);

    //// IF CORRECT GO SEND RESPONSE
    // SSL_write(ssl, "RESPONSE: 200", MAXDATASIZE);
}


void mx_ChatCreateHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ' ');
    char **chat_data = mx_strsplit(request_arr[1], ';');

    // mx_db_create_chat("Chat1", "admin,user");
    int created_chat_id = mx_db_create_chat(chat_data[0], chat_data[1]);

    char * response = mx_strnew(256);
    mx_strcat(response, "RESPONSE: 200|");
    mx_strcat(response, mx_itoa(created_chat_id));

    printf("%s\n", response);

    SSL_write(ssl, response, MAXDATASIZE);
}


void mx_ChatsIDLoad(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ' ');

    printf("zalypa konya %s\n", request_arr[1]);
    int chat_id = mx_get_id_by_ChatTitle(request_arr[1]);
    char * response = mx_strnew(MAXDATASIZE);

    mx_strcat(response, "RESPONSE: 200|");
    mx_strcat(response, mx_itoa(chat_id));

    SSL_write(ssl, response, MAXDATASIZE);
}


void mx_ChatsLoad(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ' ');

    char * response = mx_strdup(mx_db_get_all_chats_for_user(request_arr[1]));

    printf("sending: %s\n", response);
    SSL_write(ssl, response, MAXDATASIZE);
}


void mx_ChatChangeNameHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    char **request_arr = mx_strsplit(buf, ':');
    char **chat_data = mx_strsplit(request_arr[1], ';');

    mx_change_chat_title(chat_data[0], chat_data[1]);

    SSL_write(ssl, "RESPONSE: 200", MAXDATASIZE);
}


void mx_CheckLastMessageHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    //// TODO - CORRECT THE COMMENTS
    // Делаем запрос на бд - выгружаем послденне добавленное сообщение в chat_id (по идее максимальный айди у него будет)
    // Разбиваем полученную строку на элементы:
    // Username, message_id, time, text
    // Справниваем с элементами полученными от клиента
    // Если не совпали - отправляем элементы клиенту

    char **request_arr = mx_strsplit(buf, ' ');
    char *result = mx_get_last_message(request_arr[1], request_arr[2]);
    // mx_printstr("hyi1\n");
    if (!mx_strcmp(result, "RESPONSE 0: NO MESSAGES FOUND"))
    {
        printf("result = %s\n", result);
        SSL_write(ssl, result, strlen(result));
        // continue;
    }

    // char **db_result_arr = mx_strsplit(result, ';');
    // mx_printstr("hyi2\n");

    // printf("%s / %s\n", db_result_arr[0], request_arr[2]);
    // if (!mx_strcmp(db_result_arr[0], request_arr[2]))
    // {
    //     // send(cliProc, "RESPONSE: 200: LAST MESSAGE IS UP TO DATE", MAXDATASIZE, 0);
    //     SSL_write(ssl, "RESPONSE: 200: LAST MESSAGE IS UP TO DATE", strlen("RESPONSE: 200: LAST MESSAGE IS UP TO DATE"));
    // }
    else
    {
        // send(cliProc, result, strlen(result), 0);
        SSL_write(ssl, result, strlen(result));
        mx_strdel(&result);
    }
}


void mx_LoadLast50MessagesHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    // Делаем запрос на БД и выгружаем последние 50 сообщений по chat_id
    // Если у нас не chat_id (первый символ не число) то сначала делаем запрос в БД
    // на получение айди чата по его названию и передает этот айди в mx_get_last_50_messages


    char **request_arr = mx_strsplit(buf, ' ');

    int chat_id = 0;
    if (!mx_isdigit(request_arr[1][0]))
    {
        chat_id = mx_get_id_by_ChatTitle(request_arr[1]);
    }
    else
    {
        chat_id = mx_atoi(request_arr[1]);
    }

    char *result = mx_get_last_50_messages(chat_id);
    if (!mx_strcmp(result, "RESPONSE 0: NO MESSAGES FOUND"))
    {
        printf("result = %s\n", result);
        SSL_write(ssl, result, strlen(result));
    }
    else
    {
        printf("result = %s\n", result);
        // send(cliProc, result, strlen(result), 0);
        SSL_write(ssl, result, strlen(result));
    }
}


void mx_DeleteMessage(char buf[MAXDATASIZE], SSL *ssl)
{
    // Делаем запрос на БД и удаляем сообщение по его ID

    char **request_arr = mx_strsplit(buf, ' ');

    char *result = mx_delete_message_by_id(mx_atoi(request_arr[1]));

    printf("result = %s\n", result);
    SSL_write(ssl, result, strlen(result));
}


void mx_AddUserToChatHandle(char buf[MAXDATASIZE], SSL *ssl)
{
    // Делаем запрос на БД и удаляем сообщение по его ID
    // ADD_USER_TO_CHAT cur_chat_id user_name

    char **request_arr = mx_strsplit(buf, ' ');

    char * added_user_name = mx_add_user_to_chat_by_name(request_arr[1], request_arr[2]);

    char * result = mx_strnew(MAXDATASIZE);
    mx_strcat(result, "RESPONSE: 200 ADDED USER SUCCESSFULLY|");
    mx_strcat(result, added_user_name);

    printf("result = %s\n", result);
    SSL_write(ssl, result, MAXDATASIZE);
}

