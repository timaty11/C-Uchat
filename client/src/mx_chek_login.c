
#include "../inc/client.h"




int mx_chek_login()
{
    char * login = (char *)gtk_entry_get_text(GTK_ENTRY(grid_widget.login_entry));
    if(mx_strlen(login) < 8)
    {
        return 0;
    }
    return 1;
}

int mx_chek_password()
{
    char * pass = (char *)gtk_entry_get_text(GTK_ENTRY(grid_widget.password_entry));
    
    if (mx_strlen(pass) < 8)
    {
        return 0;
    }
    return 1;
}

int mx_chek_login_registr()
{
    char * login = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.login_entry));
    if(mx_strlen(login) < 8)
    {
        return 0;
    }
     return 1;
}


int mx_chek_password_registr()
{
    char * pass = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.pass_entry));
    if(mx_strlen(pass) < 8)
    {
        return 0;
    }
    return 1;
}


int mx_chek_name_entry()
{
    char * name = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.name_entry));
    if(mx_strlen(name) < 1)
    {
        return 0;
    }
    return 1;
}


int mx_chek_pass_true_entry()
{
  char * pass_confirm = (char *)gtk_entry_get_text(GTK_ENTRY(registr_window.pass_true_entry));
  
    if(mx_strlen(pass_confirm) < 1)
    {
        return 0;
    }
    return 1;
}
