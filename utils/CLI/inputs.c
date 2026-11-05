#include "./inputs.h"

int get_int(int *var){
    char input[100];
    char *end;
    long value;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    if (strchr(input, '\n') == NULL) { // clea  n buffer
        int c;

        while ((c = getchar()) != '\n' && c != EOF);

        return -1;
    }

    errno = 0;
    value = strtol(input, &end, 10);

    // Verifica se nenhum número foi encontrado
    if (end == input) {
        return -1;
    }

    // Ignora espaços após o número
    while (isspace((unsigned char)*end)) {
        end++;
    }

    // Verifica caracteres adicionais e overflow
    if (*end != '\0' || errno == ERANGE ||
        value < INT_MIN || value > INT_MAX) {
        return -1;
    }

    *var = (int)value;

    return 0;
};

int get_menu_inp(int *op, int *options_arr, int size_arr){
    int temp;
    short int ok=0;
    while(1){
        if(get_int(&temp) == 0){
            for(int i=0; i<size_arr; ++i){
                if(temp == *(options_arr+i)){
                    memset(options_arr, 0, size_arr * sizeof(int));
                    ok = 1;
                    break;
                }
            }
        };
        if(ok){
            *op = temp;
            return 0;
        }

        puts("Invalid input.");
        sleep(2);
        clearScr();
        return 1;
    }
};

int get_str(char *arr, size_t max_size) {
    char input[max_size];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    char *newline = strchr(input, '\n');

    if (newline == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }

    *newline = '\0';

    memcpy(arr, input, strlen(input)+1);

    return 0;
}

int get_pwd(char *arr){
    char input[MAX_CIPHERTEXT_SIZE];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    char *newline = strchr(input, '\n');

    if (newline == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }

    *newline = '\0';

    memcpy(arr, input, strlen(input)+1);

    return 0;
}