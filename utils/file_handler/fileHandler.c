#include "./fileHandler.h"

int vaultExists(void){
    FILE *file = fopen("./data/vault.dat", "rb");

    if(file == NULL){
        return 0;
    }

    fclose(file);
    return 1;
}

int createVault(void) {
    unsigned char salt[SALT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char nonce[NONCE_SIZE];
    unsigned char cyphertext[MAX_CIPHERTEXT_SIZE];
    unsigned char tag[TAG_SIZE];

    char password[MAX_CIPHERTEXT_SIZE];

    unsigned char verification[] = "PwdGuardian_Vault_Verification";

    printf("Digite sua senha mestra: ");


    if(get_str(password, MAX_CIPHERTEXT_SIZE) != 0){
        return 1;
    }

    if (!generateSalt(salt)) {
        return 1;
    }

    if (!deriveKey(password, salt, key)) {
        return 1;
    }

    if (!generateNonce(nonce)) {
        return 1;
    }

    int ciphertext_len = encrypt(
        verification,
        strlen((char *)verification),
        key,
        nonce,
        cyphertext,
        tag
    );

    if (ciphertext_len <= 0) {
        return 1;
    }

    VAULT_HEADER vh;

    memcpy(vh.salt, salt, SALT_SIZE);
    memcpy(vh.nonce, nonce, NONCE_SIZE);
    memcpy(vh.tag, tag, TAG_SIZE);

    memcpy(vh.verification_data, cyphertext, ciphertext_len);
    vh.verification_len = ciphertext_len;

    FILE *file = fopen("./data/vault.dat", "wb");

    if(file == NULL){
        return 1;
    }

    if (fwrite(&vh, sizeof(VAULT_HEADER), 1, file) != 1) {
        fclose(file);
        return 1;
    }
    fclose(file);

    return 0;
}

int getVaultHeader(VAULT_HEADER *vh){

    FILE *file = fopen("./data/vault.dat", "rb");

    if (file == NULL) {
        return 1;
    }

    if (fread(vh, sizeof(VAULT_HEADER), 1, file) != 1) {
        fclose(file);
        return 1;
    }

    fclose(file);

    return 0;
}

int saveCredential(const char *site, const char *pwd) {
    CREDENTIAL credential = {0};

    if (encryptCredential(&credential, site, pwd) != 0) {
        OPENSSL_cleanse(&credential, sizeof(credential));
        return 1;
    }

    FILE *file = fopen("./data/vault.dat", "ab");

    if (file == NULL) {
        OPENSSL_cleanse(&credential, sizeof(credential));
        return 1;
    }

    if (fwrite(&credential, sizeof(credential), 1, file) != 1) {
        fclose(file);
        OPENSSL_cleanse(&credential, sizeof(credential));
        return 1;
    }

    fclose(file);
    OPENSSL_cleanse(&credential, sizeof(credential));

    return 0;
}

int listCredentials(void){
    FILE *file = fopen("./data/vault.dat", "rb");

    if (file == NULL) {
        return 1;
    }

    if (fseek(file, sizeof(VAULT_HEADER), SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }

    CREDENTIAL credential;
    int count = 0;

    while (fread(&credential, sizeof(CREDENTIAL), 1, file) == 1) {


        char site[SITE_SIZE + 1] = {0};

        if (decryptCredentialSite(&credential, site) == 0) {
            printf("\n[%d]- %s\n", (count+1),site);
        } else {
            puts("Error decrypting credential.");
        }
        ++count;

        OPENSSL_cleanse(site, sizeof(site));
        OPENSSL_cleanse(&credential, sizeof(credential));
    }

    fclose(file);
    return 0;
}

int findCredential(char *target, int *index){
    FILE *file = fopen("./data/vault.dat", "rb");

    if (file == NULL) {
        return 1;
    }

    if (fseek(file, sizeof(VAULT_HEADER), SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }

    CREDENTIAL credential;
    int count = 0;

    while (fread(&credential, sizeof(CREDENTIAL), 1, file) == 1) {

        char site[SITE_SIZE + 1] = {'\0'};

        if (decryptCredentialSite(&credential, site) == 0) {
            printf("\n%s\n", site);
        } else {
            puts("Error decrypting credential.");
        }

        if(strcmp(site, target) == 0){
            *index = count;
        }

        OPENSSL_cleanse(site, sizeof(site));
        OPENSSL_cleanse(&credential, sizeof(credential));
        ++count;
    }

    fclose(file);
    return 0;
}

int takeCredential(const int *index, CREDENTIAL *credential){
    FILE *file = fopen("./data/vault.dat", "rb");

    if (file == NULL) {
        return 1;
    }

    if (fseek(file, sizeof(VAULT_HEADER), SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }
    
    for(int i=0; i<=(*index); ++i){
        fread(credential, sizeof(CREDENTIAL), 1, file);
    }

    if(credential->ciphertext_pwd_len == 0){
        return 1;
    }

    fclose(file);
    return 0;
}

int countCredentials(int *var){
    FILE *file = fopen("./data/vault.dat", "rb");

    if (file == NULL) {
        return 1;
    }

    if (fseek(file, sizeof(VAULT_HEADER), SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }

    CREDENTIAL credential;



    while (fread(&credential, sizeof(CREDENTIAL), 1, file) == 1) {

        (*var)++;
        OPENSSL_cleanse(&credential, sizeof(credential));
    }
    

    fclose(file);
    return 0;
}
