#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../includes/command.h"
#include "./../includes/scheduler.h"
#include "./../includes/simulator.h"
#include "./../includes/compare.h"
#include "./../includes/data-structures/fcfs-process-queue.h"
#include "./../includes/data-structures/sjf-process-queue.h"
#include "./../includes/data-structures/rr-process-queue.h"
#include "./../includes/data-structures/stcf-process-queue.h"
#include "./../includes/data-structures/mlfq-process-queue.h"
#include "./../includes/mlfq-config.h"

int main(int argc, char *argv[]) {
    CommandLineArguments* args = parse_command_line(argc, argv);   
    SchedulerState state = {0};

    if (args->input_file) {
        get_processes_from_file(&state, args->input_file);
    } else if (args->process_str) {
        get_processes_from_string(&state, args->process_str);
    } else {
        fprintf(stderr, "No input provided!\n");
        return 1;
    }
    
    // If comparison mode is requested, run all algorithms and exit
    if (args->compare_mode) {
        run_comparison(state, args->input_file ? args->input_file : "inline");
        free_scheduler(&state);
        free(args);
        return 0;
    }

    // Normal mode: run single algorithm
    void *algorithm = NULL;

    if(args->algorithm) {
        if(strcasecmp(args->algorithm, "fcfs") == 0) {
            state.waiting = (AbstractProcessQueue*) construct_fcfs_process_queue();
            algorithm = schedule_fcfs;
        }
        else if(strcasecmp(args->algorithm, "sjf") == 0) {
            state.waiting = (AbstractProcessQueue*) construct_sjf_process_queue();
            algorithm = schedule_sjf;
        }
        else if(strcasecmp(args->algorithm, "stcf") == 0) {
            state.waiting = (AbstractProcessQueue*) construct_stcf_process_queue();
            algorithm = schedule_stcf;
        }
        else if(strcasecmp(args->algorithm, "rr") == 0) {
            if (args->quantum_time <= 0) {
                fprintf(stderr, "Error: Quantum time must be positive for Round Robin scheduling.\n");
                exit(1);
            }
            state.quantum_time = args->quantum_time;
            state.waiting = (AbstractProcessQueue*) construct_rr_process_queue();
            algorithm = schedule_rr;
        }
        else if(strcasecmp(args->algorithm, "mlfq") == 0) {
            if (!args->mlfq_config_file) {
                fprintf(stderr, "Error: MLFQ requires --mlfq-config option\n");
                exit(1);
            }
            
            MLFQConfig* mlfq_config = load_mlfq_config(args->mlfq_config_file);
            if (!mlfq_config) {
                fprintf(stderr, "Error: Failed to load MLFQ configuration\n");
                exit(1);
            }
            
            state.waiting = (AbstractProcessQueue*) construct_mlfq_process_queue(mlfq_config);
            algorithm = schedule_mlfq;
        }
        else {
            fprintf(stderr, "Invalid Algorithm!\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "No Algorithm Found!\n");
        exit(1);
    }

    simulate_scheduler(&state, algorithm);
    free_scheduler(&state);
    free(args);

    return 0;
}



