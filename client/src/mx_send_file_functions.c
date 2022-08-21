#include "../inc/client.h"

int get_buff_size(char *filepath)
{
    FILE *f = fopen(filepath, "rb");
    int bytes;
    for(bytes = 0; getc(f) != EOF; ++bytes);
    fclose(f);
    return bytes;
}

void mx_send_file(SSL *ssl)
{
    char filename[] = "client/img/pictures/jpeg_43.jpeg";

    printf("[sending file's name... %s]\n", filename);
    int size_file = get_buff_size((char *)filename);
    char sendbuffer[size_file];
    int b;

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        perror("File");
        return;
    }

    int count_send_amounts = size_file/1024;
    SSL_write(ssl, mx_itoa(count_send_amounts), 256);

    printf("[send file...]\n");
    while((b = fread(sendbuffer, 1, sizeof(sendbuffer), fp)) > 0)
    {
        SSL_write(ssl, sendbuffer, b);
    }
    printf("end send file\n");

    fclose(fp);
}

