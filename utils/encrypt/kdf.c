#include "kdf.h"

#include <argon2.h>
#include <openssl/rand.h>
#include <string.h>

int generateSalt(unsigned char *salt) {
    return RAND_bytes(salt, SALT_SIZE) == 1;
}

int deriveKey(
    const char *password,
    const unsigned char *salt,
    unsigned char *key
) {
    int result = argon2id_hash_raw(
        3,                      // Iterações
        65536,                  // Memória em KiB
        1,                      // Paralelismo
        password,
        strlen(password),
        salt,
        SALT_SIZE,
        key,
        KEY_SIZE
    );

    return result == ARGON2_OK;
}