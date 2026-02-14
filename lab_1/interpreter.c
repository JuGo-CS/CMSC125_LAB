#include "mysh.h"

int interpreter(Command *command) {

    //if there's no input, will return to the mysh.c to ask for the input
    if (command == NULL || command->command == NULL) {
        return 0;
    }
    
    // if (strcmp(argument[0], "exit") == 0) {
    //     printf("\nmysh> exiting mysh...\n\n");
    //     exit(0);
    // }

    if (strcmp(command->command, "exit") == 0) {
        printf("\nmysh> exiting shell...\n\n");
        exit(0);
    }

    // 
    // if(strcmp(argument[0], "cd") == 0){
    //     //if there's no directory input, will raise issue
    //     if(argument[1] == NULL){
    //         printf("    > cd: Missing Input Argument!\n");
    //     }
    //     else {
    //         //check if the directory input is valid or not
    //         if (chdir(argument[1]) != 0) {
    //             printf("    > cd: Invalid Directory!\n");
    //         }
    //     }
    //     return;
    // }

    if (strcmp(command->command, "cd") == 0) {

        if (command->args[1] == NULL) {
            printf("    > cd: Missing Input Argument!\n");
            return 1;
        }
        if (chdir(command->args[1]) != 0) {
            printf("    > cd: Invalid Directory!\n");
            return 1;
        }

        return 0;
    }

    // shows current directory
    // if (strcmp(argument[0], "pwd") == 0) {
    //     char path_name[MAX_INPUT_SIZE];
    //     getcwd(path_name, sizeof(path_name));
    //     printf("    > %s\n", path_name);
        
    //     return;
    // }

    if (strcmp(command->command, "pwd") == 0) {

        char path_name[MAX_INPUT_SIZE];
        if (getcwd(path_name, sizeof(path_name)) == NULL) {
            perror("    > pwd failed!");
            return 1;
        }

        printf("    > %s\n", path_name);
        return 0;
    }

    //will fork the parent ~ to execute the given program
    pid_t pid = fork();

    if (pid < 0) {
        perror("    > fork failed!");
        return 1;
    }

    if (pid == 0) {
        //will pass the argument to the 'execvp'
        execvp(command->command, command->args);
        perror("    > execvp failed!");
        exit(127);
    } 
    
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return 0;
}
