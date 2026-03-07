#include "../include/main.h"
#include "../include/gantt.h"
#include "../include/metrics.h"
#include "../include/process.h"
#include "../include/scheduler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    
    while (true) {
        printf("mysh> ");

        // get user input
        if (!fgets(line, MAX_LINE, stdin)) break;
        line[strcspn(line, "\n")] = '\0';

        // get command arguments. currently does not handle quotes or escapes (not priority for this lab).
        char *token = strtok(line, " ");
        int i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;
        if (args[0] == NULL) continue; 

        // builtin commands
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("cd failed: Missing argument\n");
            } else if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }
        if (strcmp(args[0], "exit") == 0) break;

        // execute command
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            printf("mysh: command not found\n");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) != 0) {
                    printf("mysh exited with code %d\n", WEXITSTATUS(status));
                }
            }
        }

    }
    return 0;
}

