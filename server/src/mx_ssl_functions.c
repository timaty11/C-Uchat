#include "../inc/server.h"

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
	bp_public = BIO_new_file("server/public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if(ret != 1){
		goto free_all;
	}

	// 3. save private key
	bp_private = BIO_new_file("server/private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

	// 4. free
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bne);

	return (ret == 1);
}


char * descrypted(char *encrypted_message) {
    RSA *rsa_private_key = RSA_new();
    RSA *private;
    FILE *f;

    if ((f = fopen("server/private.pem", "r")) == NULL) {
        printf("can't find file\n");
        exit(1);
    }
    
    private = PEM_read_RSAPrivateKey(f, &rsa_private_key, NULL, NULL);

    if (private == NULL) {
        printf("Error rsa size\n");
        exit(1);
    }
    
    int len = RSA_size(private); //was public

    char * descrypted_message = malloc(len + 1);
    
    if (RSA_private_decrypt(len, (const unsigned char *)encrypted_message, (unsigned char *)descrypted_message, rsa_private_key, RSA_NO_PADDING) == -1)
    {
        ERR_print_errors_fp(stderr);
    }
    fclose(f);
    return descrypted_message;
}


char * encrypted(char *message) {
    RSA *rsa_public_key = RSA_new();
    RSA *public;
    FILE *f1;
    if ((f1 = fopen("server/public.pem", "r")) == NULL) {
        printf("can't find file\n");
        exit(1);
    }
    public = PEM_read_RSAPublicKey(f1, &rsa_public_key, NULL, NULL);
    if (public == NULL) {
        printf("Error rsa size\n");
        exit(1);
    }
    int len_pub = RSA_size(public);
    char * encrypted_message = malloc(len_pub + 1);
    if (RSA_public_encrypt(len_pub, (const unsigned char *)message, (unsigned char *)encrypted_message, rsa_public_key, RSA_NO_PADDING) == -1)
    {
        ERR_print_errors_fp(stderr);
    }

    fclose(f1);
    return encrypted_message;
}


void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}


char * sha256_string(char *string)
{
    char * outputBuffer = mx_strnew(65);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;

    return outputBuffer;
}


int sha256_file(char *path, char outputBuffer[65])
{
    FILE *file = fopen(path, "rb");
    if(!file) return -534;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return ENOMEM;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return 0;
}

