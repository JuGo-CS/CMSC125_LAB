#include "mysh.h"

/**
 * checks if a string is in an array.
 * @param array takes in a string array to check.
 * @param size takes in the size of the array to check.
 * @param string takes in the string to find in the array.
 * @return returns true if it FOUND the string in the array; returns false if NOT FOUND.
 */
bool contains_string(char* array[], int size, char* string) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i], string) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * frees all pointers found inside the struct
 * @param cmd takes in a command struct address.
 */

void free_command(Command *cmd) {
    if (!cmd) {
        return;
    }
    
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
        cmd->args[i] = NULL;
    }
    if (cmd->input_file) {
        free(cmd->input_file);
    }
    if (cmd->output_file) {
        free(cmd->output_file);
    }
    
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->command = NULL;
}

/**
 * gets user input and get a Command from it .
 * @return returns a Command Struct on SUCCESS; returns 0 if if not.
 */

Command get_command() {
    char input[MAX_INPUT_SIZE + 2];
    Command command = {0}; 

    if (fgets(input, sizeof(input), stdin) == NULL) return command;

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] != '\n') {
        fprintf(stderr, "ERROR: Maximum input size (%d) reached.\n", MAX_INPUT_SIZE);
        int ch;
        while (((ch = getchar()) != '\n') && (ch != EOF)); 
        return command;
    } else if (len > 0) {
        input[len - 1] = '\0';
    }


    char* token_arr[MAX_TOKEN_ARRAY_SIZE + 1];
    char* next_token;
    char* token = strtok_r(input, " \t", &next_token);
    int i = 0;

    while (token && (i < MAX_TOKEN_ARRAY_SIZE)) {
        token_arr[i++] = token;
        token = strtok_r(NULL, " \t", &next_token);
    }
    token_arr[i] = NULL;

    if (token != NULL) {
        fprintf(stderr, "ERROR: Too many arguments (max %d).\n", MAX_TOKEN_ARRAY_SIZE);
        return command;
    }


    int j = 0;
    while (token_arr[j] != NULL && !contains_string(special_symbols, 4, token_arr[j])) {
        command.args[j] = strdup(token_arr[j]);
        if (!command.args[j]) break; 
        j++;
    }
    command.args[j] = NULL;
    command.command = command.args[0];

    bool error_flag = false;
    while (token_arr[j] != NULL) {
        if (strcmp(token_arr[j], INPUT_REDIRECTION_SYMBOL) == 0) {
            if (command.input_file != NULL) {
                fprintf(stderr, "Error: Input redirection (<) has already been declared.\n");
                error_flag = true;
            } else if (token_arr[j + 1] == NULL) {
                fprintf(stderr, "Error: Input file for redirection is not found.\n");
                error_flag = true;
            } else {
                command.input_file = strdup(token_arr[j + 1]);
                j += 2;
            }

        } else if (strcmp(token_arr[j], OUTPUT_REDIRECTION_SYMBOL) == 0) {
            if (command.output_file != NULL) {
                fprintf(stderr, "Error: Output/Append redirection (>, >>) has already been declared.\n");
                error_flag = true;
            } else if (token_arr[j + 1] == NULL) {
                fprintf(stderr, "Error: Output file for redirection is not found.\n");
                error_flag = true;
            } else {
                command.output_file = strdup(token_arr[j + 1]);
                j += 2;
            }
        } else if (strcmp(token_arr[j], APPEND_REDIRECTION_SYMBOL) == 0) {
            if (command.output_file != NULL) {
                fprintf(stderr, "Error: Output/Append redirection (>, >>) has already been declared.\n");
                error_flag = true;
            } else if (token_arr[j + 1] == NULL) {
                fprintf(stderr, "Error: Append file for redirection is not found.\n");
                error_flag = true;
            } else {
                command.output_file = strdup(token_arr[j + 1]);
                command.append = true;
                j += 2;
            }
        } else if (strcmp(token_arr[j], BACKGROUND_INDICATOR_SYMBOL) == 0) {
            command.background = true;
            j++;
        } else {
            fprintf(stderr, "Error: Other arguments found after redirections (<, >, >>) or background process symbol (&).\n");
            error_flag = true;
        }

        if (error_flag) {
            free_command(&command);
            return (Command) {0};
        }
    }
    return command;
}

