#ifndef CLI_H
#define CLI_H

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

#include <stdio.h>
#include <stdlib.h>

void clearScr(void);

void clearTerminal(void);

void login_pageCLI(void);

void home_pageCLI(void);

void pressEnterToContinue(void);

#endif