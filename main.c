#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./utils/file_handler/fileHandler.h"
#include "./utils/vault/vault.h"
#include "./utils/CLI/CLI.h"
#include "./utils/vault/session.h"
#include "./utils/CLI/inputs.h"

typedef enum{
    LOGIN_PAGE,
    HOME_PAGE,
    ADD_CREDENTIAL,
    DEL_CREDENTIAL,
    SEE_AVALIABLE_CREDENTIALS,
    SEE_CREDENTIAL,
    EXIT
}STEP;

int main(void) {

    clearScr();

    STEP step = LOGIN_PAGE;
    int op;
    
    if(vaultExists() == 0){
        if(createVault() != 0){
            puts("Erro ao iniciar.\n");
            return 1;
        }
    }

    
    
    int options1[] = {1, 0};
    do{
        puts("==================================");
        puts("\n\nPwdGuardian\n\n");
        puts("==================================\n");
        puts("[1]-Enter\n");
        puts("[0]-Exit\n");
    } while(get_menu_inp(&op , options1, 2) != 0);


    if(op == 0){ return 0; }
    

    while(step != EXIT){
        if(logged == 0){ step = LOGIN_PAGE; }

        switch (step)
        {
        case LOGIN_PAGE: {
            login_pageCLI();
            if(openVault() == 1){
                OPENSSL_cleanse(sessionKey, sizeof(sessionKey));
                clearTerminal();
                puts(RED "Access denied.\n" RESET);
                sleep(3);
                clearTerminal();
            }
            else{
                clearTerminal();
                puts(GREEN "Access granted.\n" RESET);
                sleep(3);
                logged = 1;
                step = HOME_PAGE;
            }
            break;
        }

        case HOME_PAGE: {
            clearTerminal();
            home_pageCLI();
            int options[] = {1 ,2, 3, 0};
            while(get_menu_inp(&op, options, sizeof(options)) != 0){
                home_pageCLI();
            }
            if(op == 1){ step = SEE_AVALIABLE_CREDENTIALS; }
            else if(op == 2){ step = ADD_CREDENTIAL; }
            else if(op == 3){ step = DEL_CREDENTIAL; }
            else if(op == 0){ step = EXIT; }
            break;
        }
            
        case ADD_CREDENTIAL: {
            char site_name[SITE_SIZE];
            char pwd_text[MAX_CIPHERTEXT_SIZE];
            do{
                puts("Insert the site name:\n");
            }while(get_str(site_name, SITE_SIZE) != 0);
            
            do{
                puts("Insert the password to save:\n");
            }while(get_str(pwd_text, MAX_CIPHERTEXT_SIZE) != 0);

            if(saveCredential(site_name, pwd_text) != 0){
                OPENSSL_cleanse(&site_name, sizeof(site_name));
                OPENSSL_cleanse(&pwd_text, sizeof(pwd_text));
                puts("Erro to save.");
                sleep(2);
                break;
            }

            OPENSSL_cleanse(&site_name, sizeof(site_name));
            OPENSSL_cleanse(&pwd_text, sizeof(pwd_text));
            puts("Credential successfully saved!");

            step = HOME_PAGE;
            break;
        }

        case SEE_AVALIABLE_CREDENTIALS: {
            int credential_number=0;
            clearScr();
            listCredentials();
            countCredentials(&credential_number);
            do{
                do{
                    puts("\nEnter the credential index to see / 0 to return:\n");
                }while(get_int(&op) != 0);
            }while(op < 0 || op > credential_number);
            if(op == 0){ step = HOME_PAGE; break; }
            step = SEE_CREDENTIAL;
            break;
        }

        case SEE_CREDENTIAL: {
            clearScr();
            CREDENTIAL credential = {0};

            int index = op-1;
            
            takeCredential(&index, &credential);
            showCredential(&credential);
            clearTerminal();
            OPENSSL_cleanse(&credential, sizeof(credential));
            step = HOME_PAGE;
            break;
        }
        
        case EXIT: {
            logout();
            clearTerminal();
            return 0;
        }
        default:
            puts("Invalid option.\n");
            logout();
            break;
        }
    }

    logout();
    clearTerminal();

    return 0;
}