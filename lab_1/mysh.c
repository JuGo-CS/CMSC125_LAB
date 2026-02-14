#include "mysh.h"
#include "parser.c"
#include "interpreter.c"

int main(){
    char* input;
    // char** token_arr;
    // Command* command;

    while (true) {        
        printf("mysh> ");
        input = get_input();
        if (input == NULL) {
            continue;
        }
        printf("Input: %s\n", input);

        // get_tokens(token_arr, input);
        // if (get_command(&command, token_arr) != 0) {
        //     continue;
        // };
                
        // // possibly interpreter logic after this :>
        // interpreter(command->args);
    
        free(input);
        // free(token_arr);
        // free(command);
    }

    return 0;
}
