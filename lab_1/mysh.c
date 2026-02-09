#include "mysh.h"

int main(){

    char inputs[MAX_NUM];
    char *argument[MAX_NUM];

    while(1){
        printf("mysh> ");
        fgets(inputs, MAX_NUM, stdin);
        inputs[strcspn(inputs, "\n")] = 0;


        //currently using this parser for the mean time
        //will be remove once the parser.c is done
        int index = 0;
        char *token = strtok(inputs, " ");
        while (token != NULL) {
            argument[index] = token;
            token = strtok(NULL, " ");
            index++;
        }
        argument[index] = NULL;


        interpreter(argument);
    }

    return 0;
}