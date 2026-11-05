#ifndef VAULT_H
#define VAULT_H

#include <unistd.h>

#include "../file_handler/fileHandler.h"
#include "./session.h"
#include "../structs/CREDENTIAL.h"
#include "../vault/vault.h"
#include "../CLI/inputs.h"

void logout(void);

int openVault(void);

int encryptCredential(CREDENTIAL *credential,
                      const char *site,
                      const char *password);

int decryptCredential(CREDENTIAL *credential,
                      char *site,
                      char *password);

int decryptCredentialSite(CREDENTIAL *credential, char *site);

void showCredential(CREDENTIAL *credential);

#endif