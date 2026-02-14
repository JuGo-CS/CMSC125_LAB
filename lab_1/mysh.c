#include "mysh.h"
#include "parser.c"
#include "interpreter.c"

void print_command(Command* command) {
    printf("----- Command Struct -----\n");
    printf("Command: %s\n", command->command);
    printf("Arguments: ");
    for (int i = 0; command->args[i] != NULL; i++) {
        printf("%s ", command->args[i]);
    }
    printf("\n");
    printf("Input File: %s\n", command->input_file);
    printf("Output File: %s\n", command->output_file);
    printf("Append: %s\n", command->append ? "true" : "false");
    printf("Background: %s\n", command->background ? "true" : "false");
    printf("--------------------------\n\n");
}

int main(){
    char* input;
    char** token_arr;
    Command* command;

    while (true) {        
        printf("mysh> ");
        input = get_input();
        if (input == NULL) {
            continue;
        }

        token_arr = get_tokens(input);  
        if (token_arr == NULL) {
            continue;
        }
   
        command = get_command(token_arr);
        if (command == NULL) {
            continue;
        }

        // command output for testing purposes
        // print_command(command);

        // intereter
        interpreter(command);
    
        // free allocated memory
        free(input);
        free(token_arr);
        free(command);
    }

    return 0;
}
