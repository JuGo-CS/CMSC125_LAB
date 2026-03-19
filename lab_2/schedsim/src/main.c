<<<<<<< HEAD
=======
#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/metrics.h"
#include "./../include/gantt.h"
#include <getopt.h>
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/command-line-parser.h"
#include "./../include/discrete-event-simulator.h"
#include "./../include/scheduler.h"
#include "./../include/metrics.h"

// Parse args, load processes, run the chosen scheduling algo, print results.
int main(int argc, char *argv[]) {
    CommandLineArguments* args = parse_command_line(argc, argv);   
    SchedulerState state = {0};
    void* algorithm = NULL;

<<<<<<< HEAD
    if (args->input_file) {
        get_processes_from_file(&state, args->input_file);
    } else if (args->process_str) {
        get_processes_from_string(&state, args->process_str);
    } else {
        fprintf(stderr, "No input provided!\n");
        return 1;
    } 

    if(args->algorithm) {
        if(strcasecmp(args->algorithm, "fcfs") == 0) {
            algorithm = schedule_fcfs;
        }
        else if(strcasecmp(args->algorithm, "rr") == 0) {
            algorithm = schedule_rr;
=======
    int opt;
    char *algorithm = NULL;
    char *process_str = NULL;
    char *input_file = NULL;
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
                    fprintf(stderr, "Error: Invalid Quantum time '%s' input!\n", optarg);
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
            state.gantt_size = 0;
            token = strtok(NULL, ",");  
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
        }
        else if(strcasecmp(args->algorithm, "sjf") == 0) {
            algorithm = schedule_sjf;
        }
        else if(strcasecmp(args->algorithm, "stcf") == 0) {
            algorithm = schedule_stcf;
        }
        else {
            fprintf(stderr, "Invalid Algorithm!\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "No Algorithm Found!\n");
        exit(1);
    }
<<<<<<< HEAD

    simulate_scheduler(&state, algorithm);
    calculate_metrics(&state, state.num_processes);
    print_metrics(&state);

    free_scheduler_processes(&state);
    free(args);
=======
    print_gantt(&state);
    print_metrics(&state);

    // printf("\n\nScheduling algorithm: %s\n", algorithm);
    // for(int i = 0; i < state.num_processes; i++) {
    //     printf("%d Process: %s\n", i + 1, state.processes[i].pid);
    // }
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b

    return 0;
}



