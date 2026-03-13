#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/metrics.h"
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
    char *holder_qtime = NULL;
    int quantum_time = 0;
    
    static struct option long_options[] = {
        {"algorithm", required_argument, 0, 'a'},
        {"processes", required_argument, 0, 'p'},
        {"input", required_argument, 0, 'i'},
        {"quantum", required_argument, 0, 'q'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "a:p:i:q:", long_options, NULL)) != -1) {
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
            case 'q': {
                char *endptr;
                long val = strtol(optarg, &endptr, 10);

                if (*endptr != '\0' || val <= 0) {
                    fprintf(stderr, "Error: Quantum time '%s' must be a positive integer.\n", optarg);
                    exit(1);
                }
                
                quantum_time = (int)val;
                break;
            }

            default:
                fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
                exit(1);
        }
    }    

    // get process from --processes argument
    if (process_str) {
        char *token = strtok(process_str, ",");
        while (token && state.num_processes < MAX_PROCESSES) {
            Process p = {0};          
            sscanf(token, "%15[^:]:%d:%d", p.pid, &p.arrival_time, &p.burst_time);
            p.remaining_time = p.burst_time; 
            p.start_time = -1;
            state.processes[state.num_processes++] = p;
            token = strtok(NULL, ",");  
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
            p.start_time = -1;
            state.processes[state.num_processes++] = p;
        }
    
        fclose(fptr);
    }

    if(algorithm) {
        if(strcasecmp(algorithm, "fcfs") == 0) {
            schedule_fcfs(&state);
        }

        else if(strcasecmp(algorithm, "rr") == 0) {
            // initialize_rr(&state);
            // printf("HAHAHA\n");
            schedule_rr(&state, quantum_time);
        }

        else if(strcasecmp(algorithm, "sjf") == 0) {
            schedule_sjf(&state);
        }

        else if(strcasecmp(algorithm, "stcf") == 0) {
            schedule_stcf(&state);
        }

        // else if(strcasecmp(algorithm, "mlfq") == 0) {
            
        // }

        else {
            fprintf(stderr, "Invalid Algorithm!\n");
            exit(1);
        }

        calculate_metrics(&state, state.num_processes);
    }

    else {
        fprintf(stderr, "No Algorithm Found!\n");
        exit(1);
    }

    print_metrics(&state);

    printf("\n\nScheduling algorithm: %s\n", algorithm);
    for(int i = 0; i < state.num_processes; i++) {
        printf("%d Process: %s\n", i + 1, state.processes[i].pid);
    }

    return 0;
}



