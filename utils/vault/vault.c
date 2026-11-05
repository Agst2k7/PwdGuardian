#include "vault.h"

void logout(void){
    OPENSSL_cleanse(sessionKey, sizeof(sessionKey));
    logged = 0;
}



int openVault(void){
    unsigned char verification[] = "PwdGuardian_Vault_Verification";

    VAULT_HEADER vh;
    char input[MAX_CIPHERTEXT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char plaintext[MAX_CIPHERTEXT_SIZE];

    if(getVaultHeader(&vh) != 0){
        puts("Error to get VAULT_HEADER\n");
        pressEnterToContinue();
        return 1;
    }
    printf("\033[30;40m");
    if(get_pwd(input) != 0){
        printf("\033[0m");
        return 1;
    }
    printf("\033[0m");

    if (!deriveKey(input, vh.salt, key)) {
        OPENSSL_cleanse(key, sizeof(key));
        return 1;
    }

    

    int result = decrypt(
        vh.verification_data,
        vh.verification_len,
        key,
        vh.nonce,
        vh.tag,
        plaintext
    );

    if(result <= 0){
        OPENSSL_cleanse(key, sizeof(key));
        OPENSSL_cleanse(plaintext, sizeof(plaintext));
        return 1;
    }

    if(memcmp(verification, plaintext, sizeof(verification) - 1) != 0){
        OPENSSL_cleanse(key, sizeof(key));
        OPENSSL_cleanse(plaintext, sizeof(plaintext));
        return 1;
    }

    memcpy(sessionKey, key, KEY_SIZE); //copia key para sessionKey;

    OPENSSL_cleanse(key, sizeof(key));
    OPENSSL_cleanse(plaintext, sizeof(plaintext));
    OPENSSL_cleanse(input, sizeof(input));

    return 0;
}

int encryptCredential(CREDENTIAL *credential,
                      const char *site,
                      const char *password)
{

    if(strlen(site) > SITE_SIZE || strlen(password) > MAX_CIPHERTEXT_SIZE){
        return 1;
    }

    if(!generateNonce(credential->nonce_site)){
        memset(credential, 0, sizeof(*credential));
        return 1;
    }

    if(!generateNonce(credential->nonce_pwd)){
        memset(credential, 0, sizeof(*credential));
        return 1;
    }
    
    int site_len = encrypt((const unsigned char *)site,
            strlen(site),
            sessionKey,
            credential->nonce_site,
            credential->ciphertext_site,
            credential->tag_site);

    if(site_len <=0){
        memset(credential, 0, sizeof(*credential));
        return 1;
    }

    credential->ciphertext_site_len = site_len;

    int pwd_len = encrypt((const unsigned char *)password,
            strlen(password),
            sessionKey,
            credential->nonce_pwd,
            credential->ciphertext_pwd,
            credential->tag_pwd);

    if(pwd_len <=0){
        memset(credential, 0, sizeof(*credential));
        return 1;
    }

    credential->ciphertext_pwd_len = pwd_len;

    return 0;
};

int decryptCredential(CREDENTIAL *credential,
                      char *site,
                      char *password)
{

    int result_site = decrypt(
        credential->ciphertext_site,
        credential->ciphertext_site_len,
        sessionKey,
        credential->nonce_site,
        credential->tag_site,
        site
    );

    if(result_site <= 0){
        OPENSSL_cleanse(site, SITE_SIZE + 1);
        OPENSSL_cleanse(password, MAX_CIPHERTEXT_SIZE + 1);
        return 1;
    }

    int result_pwd = decrypt(
        credential->ciphertext_pwd,
        credential->ciphertext_pwd_len,
        sessionKey,
        credential->nonce_pwd,
        credential->tag_pwd,
        password
    );

    if(result_pwd <= 0){
        OPENSSL_cleanse(site, SITE_SIZE + 1);
        OPENSSL_cleanse(password, MAX_CIPHERTEXT_SIZE + 1);
        return 1;
    }

    if (result_site < 0 || result_site > SITE_SIZE) {
        return 1;
    }

    site[result_site] = '\0';
    password[result_pwd] = '\0';

    return 0;
}

int decryptCredentialSite(CREDENTIAL *credential, char* site){

    int result_site = decrypt(
        credential->ciphertext_site,
        credential->ciphertext_site_len,
        sessionKey,
        credential->nonce_site,
        credential->tag_site,
        site
    );

    if(result_site <= 0){
        OPENSSL_cleanse(site, SITE_SIZE + 1);
        return 1;
    }

    if (result_site < 0 || result_site > SITE_SIZE) {
        return 1;
    }

    site[result_site] = '\0';

    return 0;
};

void showCredential(CREDENTIAL *credential){
    char site[SITE_SIZE+1];
    char pwd[MAX_CIPHERTEXT_SIZE+1];

    if(decryptCredential(credential, site, pwd) != 0){
        puts("Error to decrypt the credential\n");
        pressEnterToContinue();
        return;
    }

    printf("\ntitle: %s\n", site);
    printf("\npassword: %s\n", pwd);
    pressEnterToContinue();
    clearTerminal();

    OPENSSL_cleanse(site, sizeof(site));
    OPENSSL_cleanse(pwd, sizeof(pwd));
}