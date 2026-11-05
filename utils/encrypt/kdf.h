#ifndef KDF_H
#define KDF_H

#include <stddef.h>

#define SALT_SIZE 16
#define KEY_SIZE 32

int generateSalt(unsigned char *salt);

int deriveKey(
    const char *password,
    const unsigned char *salt,
    unsigned char *key
);

#endif