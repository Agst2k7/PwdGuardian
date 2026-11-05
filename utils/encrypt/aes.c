#include "./aes.h"

int generateKey(unsigned char *key) {

    if (RAND_bytes(key, KEY_SIZE) != 1) {
        printf("Erro ao gerar chave!\n");
        return 0;
    }

    return 1;
}

int generateNonce(unsigned char *nonce) {

    if (RAND_bytes(nonce, NONCE_SIZE) != 1) {
        printf("Erro ao gerar nonce!\n");
        return 0;
    }

    return 1;
}

int encrypt(
    const unsigned char *plaintext,
    int plaintext_len,
    unsigned char *key,
    unsigned char *nonce,
    unsigned char *ciphertext,
    unsigned char *tag
) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (ctx == NULL)
        return 0;

    int len;
    int ciphertext_len;

    // Inicializa o algoritmo
    if (EVP_EncryptInit_ex(
        ctx,
        EVP_aes_256_gcm(),
        NULL,
        key,
        nonce
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    // Criptografa a mensagem
    if (EVP_EncryptUpdate(
        ctx,
        ciphertext,
        &len,
        plaintext,
        plaintext_len
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    ciphertext_len = len;

    // Finaliza a criptografia
    if (EVP_EncryptFinal_ex(
        ctx,
        ciphertext + ciphertext_len,
        &len
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    ciphertext_len += len;

    // Obtém a tag de autenticação
    if (EVP_CIPHER_CTX_ctrl(
        ctx,
        EVP_CTRL_GCM_GET_TAG,
        16,
        tag
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(
    unsigned char *ciphertext,
    int ciphertext_len,
    unsigned char *key,
    unsigned char *nonce,
    unsigned char *tag,
    unsigned char *plaintext
) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (ctx == NULL)
        return 0;

    int len;
    int plaintext_len;

    // Inicializa AES-256-GCM
    if (EVP_DecryptInit_ex(
        ctx,
        EVP_aes_256_gcm(),
        NULL,
        key,
        nonce
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    // Descriptografa os dados
    if (EVP_DecryptUpdate(
        ctx,
        plaintext,
        &len,
        ciphertext,
        ciphertext_len
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    plaintext_len = len;

    // Configura a tag recebida
    if (EVP_CIPHER_CTX_ctrl(
        ctx,
        EVP_CTRL_GCM_SET_TAG,
        16,
        tag
    ) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    // Verifica a autenticidade e finaliza
    int result = EVP_DecryptFinal_ex(
        ctx,
        plaintext + plaintext_len,
        &len
    );

    EVP_CIPHER_CTX_free(ctx);

    if (result != 1) {
        OPENSSL_cleanse(plaintext, plaintext_len);
        return -1;
    }

    plaintext_len += len;

    return plaintext_len;
}