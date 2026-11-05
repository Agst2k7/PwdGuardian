#include "./CLI.h"

void clearScr(void){
    #ifdef _WIN32
        system("cls");
    #elif defined(__linux__)
        system("clear");
    #else
        printf("\033[2J\033[H");
    #endif
}


void login_pageCLI(void){
    clearScr();
    puts("==================================");
    puts("\n\nPwdGuardian\n\n");
    puts("==================================\n");

    puts("Enter the password: ");
};

void home_pageCLI(void){
    puts("==================================");
    puts("\n\nPwdGuardian / home_page\n\n");
    puts("==================================\n");
    puts("Choose an option:\n");
    puts("[1]-Show Credentials\n");
    puts("[2]-Add Credential\n");
    puts("[3]-Delete Credential\n");
    puts("[0]-Logout\n");
};

void clearTerminal(void){
    printf("\033[3J");  // Limpa o scrollback (quando suportado)
    printf("\033[2J");  // Limpa a tela
    printf("\033[H");   // Move o cursor para o início
    fflush(stdout); 
}

void pressEnterToContinue(void){
    printf("\nPress Enter to continue...");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}