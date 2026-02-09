#ifndef MYSH_H
#define MYSH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/wait.h>
#define MAX_TOKEN_ARRAY_SIZE 256
#define MAX_INPUT_SIZE 1024

typedef struct {
    char *command;       
    char *args[MAX_TOKEN_ARRAY_SIZE];     
    char *input_file;     
    char *output_file;   
    bool append;         
    bool background;     
} Command;
#endif