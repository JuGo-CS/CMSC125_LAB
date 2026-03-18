#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../includes/command.h"

/**
 * This function gets command-line arguments.
 * Refer to command-line-parser.h to add new command-line arguments.
 */
CommandLineArguments* parse_command_line(int argc, char *argv[]) { 
    CommandLineArguments* args = malloc(sizeof(CommandLineArguments));
    if (!args) {
        fprintf(stderr, "Error: Failed to allocate memory for command-line arguments.\n");
        exit(1);
    }
    memset(args, 0, sizeof(CommandLineArguments));

    int opt;
    while ((opt = getopt_long(argc, argv, "a:p:i:q:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                args->algorithm = optarg;
                break;  
            
            case 'p':
                args->process_str = optarg;
                break;
            
            case 'i':
                args->input_file = optarg;
                break;
            
            case 'q': 
                char *endptr;
                long val = strtol(optarg, &endptr, 10);
                args->quantum_time = (int)val;
                break;
            
            default:
                fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
                exit(1);
        }
    }        
    return args;    
}

