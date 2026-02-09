#include "mysh.h"
#include "parser.c"

int main(){
    char input[MAX_INPUT_SIZE];
    char* token_arr[MAX_TOKEN_ARRAY_SIZE + 1];  // MAX_TOKEN_ARRAY_SIZE plus the NULL at the end.
    Command command;

    while (true) {
        // reset command struct
        command = (Command) {0};
        
        // getting and parsing user input 
        printf("mysh> ");
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

        // outputs the content of the command struct
        printf("command: %s\n", command.command);
        
        printf("args: ");
        int i = 0;
        while (command.args[i] != NULL) {
            printf("%s, ", command.args[i]);
            i++;
        }
        printf("\n");

        printf("input-file: %s\n", command.input_file);
        printf("output-file: %s\n", command.output_file);
        printf("append? : %d\n", command.append);
        printf("background? : %d\n", command.background);  
        printf("\n");  

        // possibly interpreter logic after this :>
    }

    return 0;
}
