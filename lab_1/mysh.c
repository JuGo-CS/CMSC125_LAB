#include "mysh.h"
#include "parser.c"
#include "interpreter.c"

int main(){
    while (true) {      
        cleanup_background_jobs();
          
        printf("mysh> ");
        Command command = get_command();
        if (command.command) {
            interpreter(&command);
        }
        // interpreter(command); // make deep copies of commands sa sulod sang interpreter if istore siya sa bg jobs
    
        // free_command(&command);
    }

    return 0;
}
