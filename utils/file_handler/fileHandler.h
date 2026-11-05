#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../structs/CREDENTIAL.h"
#include "../encrypt/aes.h"
#include "../encrypt/kdf.h"
#include "../vault/vault.h"

int vaultExists(void);

int createVault(void);

int getVaultHeader(VAULT_HEADER *vh);

int saveCredential(const char *site, const char *pwd);

int listCredentials(void);

int findCredential(char *title, int *index);

int takeCredential(const int *index, CREDENTIAL *credential);

int countCredentials(int *var);

#endif