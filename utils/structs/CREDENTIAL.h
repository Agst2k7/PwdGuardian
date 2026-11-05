#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#define SITE_SIZE 256
#define MAX_CIPHERTEXT_SIZE 1000
#define NONCE_SIZE 12
#define TAG_SIZE 16

#define SALT_SIZE 16


typedef struct {
    unsigned char ciphertext_site[SITE_SIZE];
    unsigned char nonce_site[NONCE_SIZE];
    unsigned char tag_site[TAG_SIZE];

    unsigned char ciphertext_pwd[MAX_CIPHERTEXT_SIZE];
    unsigned char nonce_pwd[NONCE_SIZE];
    unsigned char tag_pwd[TAG_SIZE];

    int ciphertext_site_len;
    int ciphertext_pwd_len;
} CREDENTIAL;

typedef struct {
    unsigned char salt[SALT_SIZE];

    unsigned char nonce[NONCE_SIZE];
    unsigned char tag[TAG_SIZE];

    unsigned char verification_data[MAX_CIPHERTEXT_SIZE];
    int verification_len;
} VAULT_HEADER;

#endif