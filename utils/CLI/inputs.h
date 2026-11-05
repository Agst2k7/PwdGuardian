#ifndef INPUTS_H
#define INPUTS_H

#include "./CLI.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include "../structs/CREDENTIAL.h"

int get_int(int *ptr);

int get_menu_inp(int *op, int *options_arr, int arr_size);

int get_str(char *arr, size_t max_size);

int get_pwd(char *arr);

#endif