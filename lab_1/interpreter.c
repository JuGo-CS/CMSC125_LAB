#include "mysh.h"

void interpreter(char **argument) {

    //if there's no input, will return to the mysh.c to ask for the input
    if (argument[0] == NULL) {
        return;
    }

    
    if (strcmp(argument[0], "exit") == 0) {
        printf("\nmysh> exiting mysh...\n\n");
        exit(0);
    }

    // 
    if(strcmp(argument[0], "cd") == 0){
        //if there's no directory input, will raise issue
        if(argument[1] == NULL){
            printf("    > cd: Missing Input Argument!\n");
        }
        else {
            //check if the directory input is valid or not
            if (chdir(argument[1]) != 0) {
                printf("    > cd: Invalid Directory!\n");
            }
        }
        return;
    }

    // shows current directory
    if (strcmp(argument[0], "pwd") == 0) {
        char path_name[MAX_INPUT_SIZE];
        getcwd(path_name, sizeof(path_name));
        printf("    > %s\n", path_name);
        
        return;
    }

    //will fork the parent ~ to execute the given program
    pid_t pid = fork();

    if (pid == 0) {
        //will pass the argument to the 'execvp'
        execvp(argument[0], argument);
        perror("execvp failed");
        exit(1);
    } else {
        wait(NULL);
    }
}
