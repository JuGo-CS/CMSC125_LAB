#include "mysh.h"

int get_input(char*);

int main(){
    char input[MAX_INPUT_SIZE];

    while (true) {
        printf("mysh> ");
        if (get_input(input) != 0) return 1;
        printf("%s\n", input);
    }

    return 0;
}


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