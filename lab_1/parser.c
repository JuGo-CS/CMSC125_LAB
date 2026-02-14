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
 * Gets user input from terminal.
 * NOTE: 
 *  [1] MAX_INPUT_SIZE + 2 is used for pointer size and fgets size as fgets reads 
 *   MAX_INPUT_SIZE characters plus the newline character (\n) and the NULL terminator (\0).
 * 
 * @return returns a pointer to the input string if SUCCESS; returns NULL if an ERROR has occured.
 */
char* get_input() {
    char* input = malloc((MAX_INPUT_SIZE + 2) * sizeof(char));
    if (input == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for input.\n");
        exit(1);
    }
    
    fgets(input, MAX_INPUT_SIZE + 2, stdin);
    if (strchr(input, '\n') == NULL) { 
        fprintf(stderr, "ERROR: Maximum input size (%d) is reached.\n", MAX_INPUT_SIZE);
        while (getchar() != '\n');
        free(input);
        return NULL;       
    } else {
        input[strcspn(input, "\n")] = '\0';     
        return input;    
    }
}

/**
 * Turns the input string to a list of tokens.
 * NOTE:
 * [1] Tokens are separated by spaces or tabs.
 * [2] MAX_TOKEN_ARRAY_SIZE + 1 is for the MAX_TOKEN_ARRAY_SIZE tokens and the NULL terminator (\0) at the end of the token array.
 * 
 * @param input string input of user.
 * @return returns a pointer to the array of tokens if SUCCESS; returns NULL if an ERROR has occured.
 */
char** get_tokens(char* input) {
    char** token_arr = malloc((MAX_TOKEN_ARRAY_SIZE + 1) * sizeof(char*));
    if (token_arr == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for token array.\n");
        exit(1);
    }

    char* next_token;    
    char* token = strtok_r(input, " \t", &next_token);
    int i = 0;
    while (token && (i < MAX_TOKEN_ARRAY_SIZE + 1)) {
        token_arr[i] = token;
        token = strtok_r(NULL, " \t", &next_token);
        i++;
    }
    if (i == MAX_TOKEN_ARRAY_SIZE + 1) {
        fprintf(stderr, "Error: Maximum tokens (%d) reached.\n", MAX_TOKEN_ARRAY_SIZE);
        free(token_arr);
        return NULL;
    } else {
        token_arr[i] = NULL;
        return token_arr;
    }
}


/**
 * Sets the command struct to be executed given a list of tokens. 
 * 
 * Note: 
 *  All commands follow the following format for parsing with an example:
 *      command_name    command_args    redirect_operations(<, >, >>)   background_operation(&)
 *      ls              -la             > file.txt                      &
 *  
 * @param command takes in the address of the Command struct and stores values there.   
 * @param tokens takes in an array of tokens.
 * @return returns 0 if SUCCESS; returns 1 if an ERROR has occured.
 */
int get_command(Command* command, char** tokens) {        
    
    /* These are special symbols for redirection and 
        for declaring background processes */
    char* INPUT_REDIRECTION_SYMBOL = "<";
    char* OUTPUT_REDIRECTION_SYMBOL = ">";
    char* APPEND_REDIRECTION_SYMBOL = ">>";
    char* BACKGROUND_INDICATOR_SYMBOL = "&";
    char* special_symbols[] = {
        INPUT_REDIRECTION_SYMBOL, 
        OUTPUT_REDIRECTION_SYMBOL, 
        APPEND_REDIRECTION_SYMBOL, 
        BACKGROUND_INDICATOR_SYMBOL
    };
    

    int i = 0;
    
    /* Handles finding all command arguments until we find NULL 
        or a special symbol (>, >>, <, or &). */
    while (tokens[i] != NULL && !contains_string(special_symbols, sizeof(special_symbols)/sizeof(char*), tokens[i])) {
        command->args[i] = tokens[i];
        i++;
    }
    command->command = command->args[0];
    command->args[i] = NULL;
    

    /* Handles redirection and background processes. 
        Note:
        [1] For redirection, there shall be only one input and 
            output redirection (you cannot have two or more input and
            output/append redirections.), or it will lead to an ERROR. 
            
        [2] Extra arguments or NULL found after redirections will lead to an ERROR.
            Example of extra args in input: 
                echo "hello" > output.txt EXTRA_ARGS

            Redirection symbols (<, >, >>) must always be followed by only one file.
        
        [3] Background operation is optional, however, it must be the last thing in the 
            user input. If not, then it will lead to an ERROR.
            Example:
                sleep 20 &

        [3] Order of input and output redirection does not matter.
    */
    while (tokens[i] != NULL) {
        if (strcmp(tokens[i], INPUT_REDIRECTION_SYMBOL) == 0) {
            if (command->input_file != NULL) {
                fprintf(stderr, "Error: Input redirection (<) has already been declared.\n");
                return 1;
            } else if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: Input file for redirection is not found.\n");
                return 1;
            } else {
                command->input_file = tokens[i + 1];
                i += 2;
            }
        } else if (strcmp(tokens[i], OUTPUT_REDIRECTION_SYMBOL) == 0) {
            if (command->output_file != NULL) {
                fprintf(stderr, "Error: Output/Append redirection (>, >>) has already been declared.\n");
                return 1;
            } else if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: Output file for redirection is not found.\n");
                return 1;
            } else {
                command->output_file = tokens[i + 1];
                i += 2;
            }
        } else if (strcmp(tokens[i], APPEND_REDIRECTION_SYMBOL) == 0) {
            if (command->output_file != NULL) {
                fprintf(stderr, "Error: Output/Append redirection (>, >>) has already been declared.\n");
                return 1;
            } else if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: Append file for redirection is not found.\n");
                return 1;
            } else {
                command->output_file = tokens[i + 1];
                command->append = true;
                i += 2;
            }
        } else if (strcmp(tokens[i], BACKGROUND_INDICATOR_SYMBOL) == 0) {
            command->background = true;
            i++;
        } else {
            fprintf(stderr, "Error: Other arguments found after redirections (<, >, >>) or background process symbol (&).\n");
            return 1;
        }
    }

    return 0;
}

