#include "mysh.h"

/*
   This keeps track of how many background jobs we've started.
   Crucial for printing how many background job already runs
   Also for keeping track of the current running background jobs.
   
 */

int total_bg_job;
int active_bg_job;
pid_t pids_bg_job[MAX_BG_JOBS];



/*
    cleanup_background_jobs()

    This function checks all active background jobs to see
    if they have finished execution.

    It uses waitpid() with WNOHANG so that:
        - It does NOT block the shell
        - It only reaps processes that already finished

    If a background job has finished:
        - Print a completion message
        - Remove its PID from the tracking array
        - Shift remaining PIDs left
*/

void cleanup_background_jobs() {

    int status;

    for (int i = 0; i < active_bg_job; i++) {

        pid_t result = waitpid(pids_bg_job[i], &status, WNOHANG);

        if (result > 0) {

            printf("    > Background job (PID: %d) finished.\n", pids_bg_job[i]);

            // for (int j = i; j < active_bg_job - 1; j++) {
            //     pids_bg_job[j] = pids_bg_job[j + 1];
            // }

            pids_bg_job[i] = pids_bg_job[--active_bg_job];

            // active_bg_job--;
            i--; 
        }
    }
}

int handles_builtin(Command *command, int *builtin_status) {
    /*
        Buuilt-in commands such as pwd, cd, and 
        exit are located outside the forking part because
        these commands must be  handled by the parent process.
        No forking needed on these commands.
    */


    /*
        BUILT-IN: exit
        If the user types "exit",
        will result to end the whole shell.
     */
    if (strcmp(command->command, "exit") == 0) {
        printf("\nmysh> exiting shell...\n\n");
        exit(0);
    }


    /*
        BUILT-IN: cd
        CD navigate between folders (directories) within a file system
     */
    if (strcmp(command->command, "cd") == 0) {

        if (command->args[1] == NULL) {
            printf("    > cd: Missing Input Argument!\n");
            *builtin_status = UNSUCCESSFUL;
        }

        else {
            if (chdir(command->args[1]) != 0) {
                printf("    > cd: Invalid Directory!\n");
                *builtin_status = UNSUCCESSFUL;
            }

            else {
                printf("    > cd: Directory was changed successfully!\n");
                *builtin_status = SUCCESSFUL;
            }
        }

        return A_BUILTIN;
    }



    /*
        BUILT-IN: pwd
     
        This just prints where we currently are in the file system.
        If the current directory is "mnt/cd/cmsc125", shell will 
        just print this.
     */
    if (strcmp(command->command, "pwd") == 0) {

        char path_name[MAX_INPUT_SIZE];
        if (getcwd(path_name, sizeof(path_name)) == NULL) {
            perror("    > Pwd Failed!");
            *builtin_status = UNSUCCESSFUL;
        } 
        else {
            printf("    > %s\n", path_name);
            *builtin_status = SUCCESSFUL;
        }

        return A_BUILTIN;
    }

    return NOT_BUILTIN;
}

int interpreter(Command *command) {

    // if there's no input, will return to the mysh.c to ask for the input
    if (command == NULL || command->command == NULL) {
        return 0;
    } 

    int builtin_status = 0;
    if (handles_builtin(command, &builtin_status)){
        return builtin_status;
    }

    // will fork the parent
    pid_t pid = fork();


    // if the forking failed, it will be catched by this if statement
    if (pid < 0) {
        perror("    > Fork Failed!");
        return 1;
    }

    /*
       CHILD PROCESS
     
       pid == 0 means we are now inside the child.
       The child is responsible for:
            - setting up i/o redirection
            - executing the actual command
     */

    if (pid == 0) {

        /*
          INPUT REDIRECTION
          
          If user typed something like:
            command < input.txt
          
          We open that file and connect it to STDIN.
         */
        if (command->input_file != NULL) {

            int fd = open(command->input_file, O_RDONLY);
            if (fd < 0) {
                perror("    > Input Redirection Failed!");
                exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }


        /*
           OUTPUT REDIRECTION
         
           Handles:
             >  (overwrite)
             >> (append)
         
         */
        if (command->output_file != NULL) {

            int flags = O_WRONLY | O_CREAT;

            if (command->append)
                flags |= O_APPEND;  // >>
            else
                flags |= O_TRUNC;   // >

            int fd = open(command->output_file, flags, 0644);

            if (fd < 0) {
                perror("    > Output Redirection Failed");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        /*
           Now this part replaces the child process
           with the actual program the user requested.
        */
        execvp(command->command, command->args);
        perror("    > Execvp Failed!");
        exit(127);
    } 

    /*
       PARENT PROCESS
     
       This is still our shell.
       The shell decides:
         - Do we wait?
         - Or do we run it in background?
     */
    else {

        /*
           FOREGROUND PROCESS
          
           Normal commands.
           The shell waits until the child finishes.
         */
        if (!command->background) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    printf("    > Command exited with code: %d!\n", exit_code);
                }
            }
        } 


        /*
           BACKGROUND PROCESS
          
           If the user added "&" at the end,
           the shell DO NOT wait.
          
           The shell stays responsive.
           The child runs on its own.
         */
        else {

            if (active_bg_job < MAX_BG_JOBS) {
                // total_bg_job++;      // Increase background job number
            
            pids_bg_job[active_bg_job++] = pid;
            total_bg_job++;


            /*
               Rebuild the full command string
               just so we can print it nicely.
             */
            char full_command[1024] = "";
            int i = 0;

            while (command->args[i] != NULL) {
                strcat(full_command, command->args[i]);
                strcat(full_command, " ");
                i++;
            }

            printf("    > [%d] Started background job: %s(PID: %d)\n", total_bg_job, full_command, pid);

            }

            else {
                printf("    > Too many background jobs at the moment!\n");
            }
        }
    }
    
    return 0;
}
