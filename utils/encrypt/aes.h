#ifndef AES_H
#define AES_h

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

#define KEY_SIZE 32
#define NONCE_SIZE 12

int generateKey(unsigned char *key);
int generateNonce(unsigned char *nonce);

int encrypt(
    const unsigned char *plaintext, // original text
    int plaintext_len,  // text length
    unsigned char *key, // chave AES
    unsigned char *nonce,   // Nonce
    unsigned char *ciphertext,  // encrypted text
    unsigned char *tag  // auth tag
); // return 0 if error

int decrypt(
    unsigned char *ciphertext, // encrypted text
    int ciphertext_len, // encrypted text length
    unsigned char *key, // chave AES
    unsigned char *nonce,   // Nonce
    unsigned char *tag, // auth tag
    unsigned char *plaintext // original text
);

#endif