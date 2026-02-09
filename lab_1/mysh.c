#include "mysh.h"
#include "parser.c"

int main(){
    char input[MAX_INPUT_SIZE];
    char* token_arr[MAX_TOKEN_ARRAY_SIZE + 1];  // MAX_TOKEN_ARRAY_SIZE plus the NULL at the end.
    Command command;

    while (true) {
        // reset command struct
        command = (Command) {0};
        get_input(input);
        get_tokens(token_arr, input);
        if (get_command(&command, token_arr) != 0) {
            continue;
        };
        
        // exits if command is exit
        if (strcmp(command.command, "exit") == 0) {
            printf("Exiting mysh...");
            break;
        }
        
        // possibly interpreter logic after this :>
        interpreter(command.args);

    }

    return 0;
}
