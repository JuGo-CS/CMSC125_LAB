#include "mysh.h"

/**
 * gets user input from terminal.
 * @param input takes in a character array to store the input.
 * @return returns 0 if SUCCESS; returns 1 if an ERROR has occured.
 */
int get_input(char* input) {
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) { 
        fprintf(stderr, "ERROR: Maximum input size (%d)) is reached.\n", MAX_INPUT_SIZE);
        return 1;       
    }
    input[strcspn(input, "\n")] = '\0';     
    return 0;    
}

/**
 * turns the input string to a list of tokens.
 * @param dest takes in an array of strings to store all the tokens there. 
 * @param input string input of user.
 * @return returns 0 if SUCCESS; returns 1 if an ERROR has occured.
 */
int get_tokens(char** dest, char* input) {
    char* next_token;    
    char* token = strtok_r(input, " \t", &next_token);
    
    int i = 0;
    while (token && (i < MAX_TOKEN_ARRAY_SIZE - 1)) {
        dest[i] = token;
        token = strtok_r(NULL, " \t", &next_token);
        i++;
    }
    if (i == MAX_TOKEN_ARRAY_SIZE) {
        fprintf(stderr, "Error: Maximum tokens (%d) reached.\n", MAX_TOKEN_ARRAY_SIZE);
        return 1;
    } else {
        dest[i] = NULL;
    }

    return 0;
}