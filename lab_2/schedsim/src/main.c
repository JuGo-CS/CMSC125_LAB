#include "./../include/process.h"
#include "./../include/scheduler.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // initialize scheduler state
    SchedulerState state = {0}; 

    // get flags and arguments
    int opt;
    char *algorithm = NULL;
    char *process_str = NULL;
    char *input_file = NULL;
    
    static struct option long_options[] = {
        {"algorithm", required_argument, 0, 'a'},
        {"processes", required_argument, 0, 'p'},
        {"input", required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "a:p:i", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                algorithm = optarg;
                break;  
            case 'p':
                process_str = optarg;
                break;
            case 'i':
                input_file = optarg;
                break;
            default:
                fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
                exit(1);
        }
    }    

    // get process from --processes argument
    if (process_str) {
        char *token = strtok(process_str, ";");
        while (token && state.num_processes < MAX_PROCESSES) {
            Process p = {0};          
            sscanf(token, "%15[^:]:%d:%d", p.pid, &p.arrival_time, &p.burst_time);
            p.remaining_time = p.burst_time; 
            state.processes[state.num_processes++] = p;
            token = strtok(NULL, ";");  
        }
    } else if (input_file) {
        FILE *fptr = fopen(input_file, "r");
        if (!fptr) {
            fprintf(stderr, "Error opening file: %s\n", input_file);
            exit(1);
        }
        
        char line[256];
        while (fgets(line, sizeof(line), fptr)) {
            line[strcspn(line, "\n")] = 0; 
            Process p = {0};          
            sscanf(line, "%15[^ ] %d %d", p.pid, &p.arrival_time, &p.burst_time);
            p.remaining_time = p.burst_time; 
            state.processes[state.num_processes++] = p;
        }
    
        fclose(fptr);
    }

    printf("Scheduling algorithm: %s\n", algorithm);
    return 0;
}



