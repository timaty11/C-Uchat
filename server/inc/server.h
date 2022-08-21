#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <malloc/malloc.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <sys/wait.h>
#include <netdb.h>
#include "../../framework/libmx/inc/libmx.h"
#include <sqlite3.h>
#include <signal.h>

#include "openssl/ssl.h"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/x509.h"


#define BACKLOG 5
#define MAXDATASIZE 2048

#ifndef SERVER_H
#define SERVER_H


// DB functions
void mx_sql_table_create(void);
void mx_add_user(char *user, char *name, char *pass);
int mx_check_input(char *user, char *name, char *pass);
int mx_db_login(char *user, char *pass, char ** cur_user_name);
char * mx_change_user_pass(char * user_id, char * old_pass, char * new_pass);
int mx_db_add_message(char * chat_id, char * user_id, char * mes_type, char * text, char * pic_path);
int mx_get_user_id(char *user);
char * mx_get_user_by_id(int user_id);
char * mx_get_user_by_name(char * user_name);
char * mx_get_last_message(char * chat_id, char * last_message_id);
char * mx_get_last_50_messages(int chat_id);
int mx_get_id_by_ChatTitle(char * ChatTitle);
char * mx_get_ChatData_by_id(char * chat_id);
int mx_db_create_chat(char * title, char * members);
void mx_update_user_status(char * user_id, char * status);
char * mx_db_get_all_chats_for_user(char * user_id);
char * mx_delete_message_by_id(int mes_id);
char * mx_add_user_to_chat_by_name(char * chat_id, char * user_name);
void mx_change_chat_title(char * chat_id, char * new_title);

char * mx_hande_single_quotes(char * original);

// Certeficate functions
SSL_CTX* mx_init_server_CTX(void);
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
void ShowCerts(SSL* ssl);

// Requests handle functions
void mx_CreateUserHandle(char buf[MAXDATASIZE], SSL *ssl);
bool mx_LoginUserHandle(char buf[MAXDATASIZE], SSL *ssl, int *user_id);
void mx_UserChangeNameHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_UserChangePassHandle(char buf[MAXDATASIZE], SSL *ssl, int user_id);
void mx_DisconnectHandle(SSL *ssl, int cliProc, int servSock);
void mx_MessageSendHandle(char buf[MAXDATASIZE], SSL *ssl, int user_id, char * data_type);
void mx_FileSendHandle(SSL *ssl);
void mx_FindUserHandler(char buf[MAXDATASIZE], SSL *ssl);
void mx_ChatCreateHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_ChatsIDLoad(char buf[MAXDATASIZE], SSL *ssl);
void mx_ChatsLoad(char buf[MAXDATASIZE], SSL *ssl);
void mx_ChatChangeNameHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_CheckLastMessageHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_AddUserToChatHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_LoadLast50MessagesHandle(char buf[MAXDATASIZE], SSL *ssl);
void mx_DeleteMessage(char buf[MAXDATASIZE], SSL *ssl);

// Cipher algorithms
bool generate_keys();
char * descrypted(char *encrypted_message);
char * encrypted(char *message);

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65]);
char *  sha256_string(char *string);
int sha256_file(char *path, char outputBuffer[65]);

// File exxchange
void mx_receive_file(SSL * ssl);



#endif
