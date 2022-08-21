#include "../inc/server.h"


void mx_receive_file(SSL * ssl)
{
    char buff[1025];
    char *name_of_newfile = "kukukuku.jpeg";

    char * count_recv_amounts = mx_strnew(256);
    
    SSL_read(ssl, count_recv_amounts, 256);

    printf("[new file is %s]\n", name_of_newfile);

    FILE* fp = fopen( name_of_newfile, "wb");
    int tot = 0;
    int b;
    if(fp != NULL)
    {
        printf("[receive file...]\n");
        for (int i = 0; i <= mx_atoi(count_recv_amounts); i++)
        {
           
            b = SSL_read(ssl, buff, 1024);
            tot += b;
            fwrite(buff, 1, b, fp);
        }

        printf("[received byte: %d]\n",tot);
        if (b<0)
            perror("Receiving");

        fclose(fp);
    }
    else
    {
        perror("File");
    }
}

