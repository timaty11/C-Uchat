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
#include <gtk/gtk.h>
#include <glib.h>

#include "openssl/ssl.h"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/x509.h"

#define MAXDATASIZE 2048

#ifndef CLIENT_H
#define CLIENT_H

struct s_grid_widget
{
    GtkWidget *label_window;
    GtkWidget *vgrid;

    GtkWidget *label_login;
    GtkWidget *login_entry;

    GtkWidget *label_password;
    GtkWidget *password_entry;

    GtkWidget *name_label;
    GtkWidget *name_entry;

    GtkWidget *pass_label;
    GtkWidget *pass_entry;

    GtkWidget *pass_true_label;
    GtkWidget *pass_true_entry;

    GtkWidget *hbox1;
    GtkWidget *hbox2;
    GtkWidget *hbox3;

    GtkWidget *login_button;
    GtkWidget *create_button;
} grid_widget;

struct s_registr_window
{
    GtkWidget *registration_window;

    GtkWidget *Text_registration;

    GtkWidget *login_label;
    GtkWidget *login_entry; 

    GtkWidget *name_label;
    GtkWidget *name_entry;

    GtkWidget *pass_label;
    GtkWidget *pass_entry;

    GtkWidget *pass_true_label;
    GtkWidget *pass_true_entry;

    GtkWidget *hbox1;
    GtkWidget *hbox2;
    GtkWidget *hbox3;
    GtkWidget *hbox4;
    GtkWidget *hbox5;

    GtkWidget *registr_button;
    GtkWidget *login_button;

    GtkWidget *empty_box;

}registr_window;

int main(int argc, char *argv[]);

int mx_chek_name_entry();
int mx_chek_pass_true_entry();
int mx_chek_login_registr();
int mx_chek_password_registr();
int mx_chek_login();
int mx_chek_password();

void First_Winsow();
void Window_Login();
void mx_server_seans();
void mx_setUser();

void login_button_clicked();
void register_button_clicked();
void mx_exit_button_clicked();
void mx_logout_button_clicked();
void mx_send_file_button_clicked();
void mx_send_message(GtkWidget * widget, GtkWidget * input);
void mx_send_sticker(GtkWidget * widget, gchar * sticker_path);
char * mx_search_button_clicked(char * user_name);
void mx_delete_message_button_clicked(GtkButton *button_that_was_clicked, gchar * mes_to_delete);
void mx_add_user_to_chat_button_clicked(GtkButton *button_that_was_clicked, GtkWidget * input);
void mx_change_chat_title_button_clicked(char * new_title); // Server request
void change_current_chat_title(GtkButton *button_that_was_clicked, gchar * new_title);
void change_current_user_pass(char * old_pass, char * new_pass);
void change_something();

gboolean mx_check_new_message(gpointer ptr);

void Window_Chat();

bool logged_in;
bool real_quit;

GtkWidget *login_label, *login_entry;
GtkWidget *password_label, *password_entry;
GtkWidget *LoginWindow;


GtkWidget *pass_confirm_label, *pass_confirm_entry;
GtkWidget *name_label, *name_entry;
GtkWidget *Window_Regist;


GtkWidget *ChatWindow;
GtkWidget *mes_input; //принимаем сообщение в формате виджет 
GtkWidget *statu_user_login;

GtkWidget *message_input;

GtkWidget * temp_box;

GtkWidget * ErrorWindow;

GtkWidget *stack;

GtkWidget *entry_login, *entry_pass, *entry_pass_true;

guint timer;

void close_window2(GtkWidget *widget, gpointer window);
void welcome();
void mx_LoginWindow();
void mx_Window_Regist();
void mx_ChatWindow();
void mx_sett_chat();
void mx_ErrorWindow(char * error_text);

void mx_create_chat_box(char * cur_user_name, char * chat_title);
void text_in_Chat(char *text, char * orientation);
void mx_sticker_in_chat(char * sticker_path, char * orientation);


void load_css(void);
void add_element_to_css(GtkWidget *element, char *name);

// Certeficate functions
SSL_CTX* mx_init_CTX(void);
void ShowCerts(SSL* ssl);

// Cipher algorithms
bool generate_keys();
char * descrypted(char * encrypted_message);
char * encrypted(char *message, char *key);
RSA * mx_convert_string_to_RSA(unsigned char *key);

void mx_get_chat_messages(GtkWidget *stack);
void mx_request_chat_create(GtkButton *button_that_was_clicked, gchar * user_to_add);

// File exxchange
void mx_send_file(SSL *ssl);
int get_buff_size(char *filepath);


#endif
