#include "mysh.h"

int main(){

    char inputs[MAX_NUM];
    char *argument[MAX_NUM];

    while(1){
        printf("mysh> ");
        fgets(inputs, 128, stdin);
        inputs[strcspn(inputs, "\n")] = 0;

        int index = 0;
        char *token = strtok(inputs, " ");
        while (token != NULL) {
            argument[index] = token;
            token = strtok(NULL, " ");
            index++;
        }
        argument[index] = NULL;

        if(strcmp(argument[0], "exit") == 0){
            printf("\nmysh> exiting mysh...\n\n");
            break;
        }

        if (strcmp(argument[0], "pwd") == 0) {
            char path_name[MAX_NUM];
            getcwd(path_name, sizeof(path_name));
            printf("    > %s\n", path_name);
            
            continue;
        }

        if(strcmp(argument[0], "cd") == 0){
            if(argument[1] == NULL){
                printf("    > cd: Missing Input Argument!\n");
            }
            else {
                if (chdir(argument[1]) != 0) {
                    printf("    > mysh> cd: Invalid Directory!\n");
                }
            }
        }
    }

    return 0;
}