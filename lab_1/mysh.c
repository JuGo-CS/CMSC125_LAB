#include "mysh.h"
#include "parser.c"

int main(){
    char input[MAX_INPUT_SIZE];
    char* token_arr[MAX_TOKEN_ARRAY_SIZE + 1];  // MAX_TOKEN_ARRAY_SIZE plus the NULL at the end.
    while (true) {
        printf("mysh> ");
        get_input(input);
        get_tokens(token_arr, input);

        int i = 0;
        while (token_arr[i] != NULL) {
            printf("%s ", token_arr[i]);
            i++;
        }
        printf("\n");

    }

    return 0;
}
