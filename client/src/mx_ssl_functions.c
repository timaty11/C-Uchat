#include "../inc/client.h"

int padding = RSA_PKCS1_OAEP_PADDING;

bool generate_keys()
{
	int				ret = 0;
	RSA				*r = NULL;
	BIGNUM			*bne = NULL;
	BIO				*bp_public = NULL, *bp_private = NULL;

	int				bits = 2048;
	unsigned long	e = RSA_F4;

	// 1. generate rsa key
	bne = BN_new();
	ret = BN_set_word(bne,e);
	if(ret != 1){
		goto free_all;
	}

	r = RSA_new();
	ret = RSA_generate_key_ex(r, bits, bne, NULL);
	if(ret != 1){
		goto free_all;
	}

	// 2. save public key
	bp_public = BIO_new_file("client/public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if(ret != 1){
		goto free_all;
	}

	// 3. save private key
	bp_private = BIO_new_file("client/private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

	// 4. free
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bne);

	return (ret == 1);
}

char * descrypted(char * encrypted_message) {
    RSA *rsa_private_key = RSA_new();
    RSA *private;
    FILE *f;

    if ((f = fopen("client/private.pem", "r")) == NULL) {
        printf("can't find file\n");
        exit(1);
    }
    
    private = PEM_read_RSAPrivateKey(f, &rsa_private_key, NULL, NULL);

    if (private == NULL) {
        printf("Error rsa size\n");
        exit(1);
    }
    
    int len = RSA_size(private); //was public

    
    char *descrypted_message = malloc(len + 1);
    
    if (RSA_private_decrypt(len, (const unsigned char *)encrypted_message, (unsigned char *)descrypted_message, rsa_private_key, RSA_NO_PADDING) == -1)
    {
        ERR_print_errors_fp(stderr);
    }
    fclose(f);
    return descrypted_message;
}


char * encrypted(char *message, char *key) {
    RSA *rsa_public_key = RSA_new();
    printf("HYE - %s\n", key);

    FILE * f = fopen("client/server_pubkey.pem", "w+");
    
    int i = 0;
    for (; key[i] != '\0'; i++)
    {
        fwrite(&key[i], sizeof(char), 1, f);
    }
    fclose(f);

    mx_printstr("2\n");
    RSA *public;
    FILE *f1;
    if ((f1 = fopen("client/server_pubkey.pem", "r")) == NULL) {
        printf("can't find file\n");
        exit(1);
    }
    public = PEM_read_RSAPublicKey(f1, &rsa_public_key, NULL, NULL);
    if (public == NULL) {
        printf("Error rsa size\n");
        exit(1);
    }
    int len_pub = RSA_size(rsa_public_key);
    mx_printstr("3\n");
    char * encrypted_message = malloc(len_pub + 1);
    if (RSA_public_encrypt(len_pub, (const unsigned char *)message, (unsigned char *)encrypted_message, rsa_public_key, RSA_NO_PADDING) == -1)
    {
        ERR_print_errors_fp(stderr);
    }
    mx_printstr("4\n");
    fclose(f1);
    return encrypted_message;
}


RSA * mx_convert_string_to_RSA(unsigned char *key)
{
    mx_printstr("5\n");
    RSA *rsa = NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    mx_printstr("6\n");
    if (keybio == NULL)
    {
        printf("Failed to create key BIO");
        return 0;
    }
    mx_printstr("7\n");

    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    if (rsa == NULL)
    {
        mx_printstr("HYETA\n");
        ERR_print_errors_fp(stderr);
    }
    mx_printstr("8\n");

    return rsa;
}

