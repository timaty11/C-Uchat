#include "../inc/server.h"


int main(int argc, char *argv[])
{
    int servSock, cliProc;
    socklen_t sin_size;
    char buf[MAXDATASIZE];


    struct sockaddr_in my_addr, income_addr;

    pid_t pid;

    if (argc != 2)
    {
        printf("ERROR: port reqiured\n");
        exit(0);
    }

    mx_sql_table_create();
    
    int port = atoi(argv[1]);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((servSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket Error: %d\n", errno);
    }
    else
    {
        printf("Server Socket %d created\n", servSock);
    }

    if (bind(servSock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("Bind Error: %d\n", errno);
    }
    else
    {
        printf("Server Bind created\n");
    }

    listen(servSock, BACKLOG);
    printf("Server is waitting for connection...\n");
    sin_size = sizeof(struct sockaddr_in);

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((cliProc = accept(servSock, (struct sockaddr *)&income_addr, &sin_size)) == -1)
        {
            perror("accept");
        }
        else
        {
            printf("Server accepted connection from %s\n", inet_ntoa(income_addr.sin_addr));
        }


        SSL *ssl;
        SSL_CTX *ctx;

        SSL_library_init();
        ctx = mx_init_server_CTX();



        LoadCertificates(ctx, "server/mycert.pem", "server/mycert.pem");
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, cliProc);

        pid = fork();        
        if (pid == 0) //// CHILD
        {
            char client_request[1024];
            char ClientPubKey[1024];
            int bytes;

            char * ServerResponse = mx_file_to_str("server/public.pem");
        
            if (SSL_accept(ssl) == -1)
            {
                mx_printstr("CANNOT ESTABILISH SAFE CONNECTION\n");
                ERR_print_errors_fp(stderr);
            }
            else
            {
                // Get request
                bytes = SSL_read(ssl, client_request, sizeof(client_request));
                client_request[bytes] = '\0';
                
                if (!mx_strcmp(client_request, "GET SERVER_PUBKEY"))
                {
                    SSL_write(ssl, ServerResponse, strlen(ServerResponse)); // Send server pubkey

                    bytes = SSL_read(ssl, ClientPubKey, sizeof(ClientPubKey)); // Get clients pub_key
                    ClientPubKey[bytes] = '\0';
                    printf("\nClientPubKey: \n%s\n", ClientPubKey);
                }
            }

            bool loged_in = false;
            int user_id;

            //// WE WORK IN THIS CYCLE
            while (1)
            {
                mx_printstr("WAITING FOR REQUEST\n");
                if (!loged_in)
                {
               
                        mx_printstr("not loged\n");
                        for (int i = 0; i < MAXDATASIZE; i++)
                        {
                            buf[i] = '\0';
                        }
                        int bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */

                        if (bytes == -1)
                        {
                            printf("Error reading ssl\n");
                        }
                        else
                        {
                            buf[bytes] = '\0';
                            printf("Client msg: \"%s\"\n", buf);
                        }

                        if (buf[0] == 'C' && buf[1] == 'R' && buf[2] == 'E' && buf[3] == 'A') // CREATE
                        {
                            mx_CreateUserHandle(buf, ssl);
                        }
                        else if (buf[0] == 'L' && buf[1] == 'O' && buf[2] == 'G' && buf[3] == 'I') // LOGIN
                        {
                            loged_in = mx_LoginUserHandle(buf, ssl, &user_id);
                            if (loged_in)
                            {
                                mx_update_user_status(mx_itoa(user_id), "online");
                            }
                        }
                        if (!mx_strcmp(buf, "DISCONNECT"))
                        {
                            mx_DisconnectHandle(ssl, cliProc, servSock);
                        }
       
                }
                else
                {
                    for (int i = 0; i < MAXDATASIZE; i++)
                    {
                        buf[i] = '\0';
                    }
                    int bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */

                    if (bytes == -1)
                    {
                        perror("recv");
                       
                    }
                    else
                    {
                        buf[bytes] = '\0';
                        printf("Client msg: \"%s\"\n", buf);
                    }

                    // Handle comands
                    if (!mx_strcmp(buf, "DISCONNECT"))
                    {
                        // TO DO
                        // Add online status change for user_id
                        mx_update_user_status(mx_itoa(user_id), "offline");
                        mx_DisconnectHandle(ssl, cliProc, servSock);
                    }
                    else if (!strncmp(buf, "LOGOUT", 6))
                    {
                        // TO DO
                        // Add online status change for user_id
                        loged_in = false;
                        SSL_write(ssl, "RESPONSE: 200 LOGGED OUT SUCCESSFULLY", MAXDATASIZE);
                        mx_update_user_status(mx_itoa(user_id), "offline");
                    }
                    else if (!strncmp(buf, "SEND_FILE", 9)) // SEND_FILE
                    {
                        mx_FileSendHandle(ssl);
                    }
                    else if (!strncmp(buf, "SEND_STICKER", 12)) // SEND_STICKER 1 path
                    {
                        mx_MessageSendHandle(buf, ssl, user_id, "STICKER");
                    }
                    else if (!strncmp(buf, "SEND", 4)) // SEND 1 hyi
                    {
                        mx_MessageSendHandle(buf, ssl, user_id, "TEXT");
                    }
                    else if (!strncmp(buf, "LOAD_USERS", 10)) // LOAD_USERS
                    {
                        mx_FindUserHandler(buf, ssl);
                    }
                    else if (!strncmp(buf, "CHAT_CREATE", 11)) // CHAT_CREATE
                    {
                        mx_ChatCreateHandle(buf, ssl);
                    }
                    else if (!strncmp(buf, "GET_CHAT_ID_BY_NAME", 19)) // GET_CHAT_ID_BY_NAME
                    {
                        mx_ChatsIDLoad(buf, ssl);
                    }
                    else if (!strncmp(buf, "LOAD_CHATS", 10)) // LOAD_CHATS
                    {
                        printf("buf is: %s\n", buf);
                        mx_ChatsLoad(buf, ssl);
                    }
                    else if (!strncmp(buf, "DB_CHECK_LAST_MESSAGE", 21)) // DB_CHECK_LAST_MESSAGE
                    {
                        mx_CheckLastMessageHandle(buf, ssl);
                    }
                    else if (!strncmp(buf, "DB_LOAD_50_MESS", 15)) // DB_LOAD_50_MESS
                    {
                        mx_LoadLast50MessagesHandle(buf, ssl);
                    }
                    else if (!strncmp(buf, "DELETE_MESSAGE", 14)) // DELETE_MESSAGE
                    {
                        mx_DeleteMessage(buf, ssl);
                    }
                    else if (!strncmp(buf, "ADD_USER_TO_CHAT", 16)) // ADD_USER_TO_CHAT
                    {
                        mx_AddUserToChatHandle(buf, ssl);
                    }
                    else if (!strncmp(buf, "CHANGE_CHAT_TITLE", 17)) // CHANGE_CHAT_TITLE
                    {
                        mx_ChatChangeNameHandle(buf, ssl);
                    }
                    else if (!strncmp(buf, "CHANGE_PASS", 11)) // CHANGE_PASS
                    {
                        mx_UserChangePassHandle(buf, ssl, user_id);
                    }
                }
            }
        }
        else //// PARENT
        {
            continue;
        }

    }

    return 0;
}

