#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../includes/command.h"
#include "./../includes/scheduler.h"
#include "./../includes/simulator.h"
#include "./../includes/data-structures/fcfs-process-queue.h"
#include "./../includes/data-structures/sjf-process-queue.h"
#include "./../includes/data-structures/rr-process-queue.h"
#include "./../includes/data-structures/stcf-process-queue.h"

int main(int argc, char *argv[]) {
    CommandLineArguments* args = parse_command_line(argc, argv);   
    SchedulerState state = {0};
    void *algorithm = NULL;

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
            state.waiting = (AbstractProcessQueue*) construct_fcfs_process_queue();
            algorithm = schedule_fcfs;
        }
        else if(strcasecmp(args->algorithm, "sjf") == 0) {
            state.waiting = (AbstractProcessQueue*) construct_sjf_process_queue();
            algorithm = schedule_sjf;
        }
        else if(strcasecmp(args->algorithm, "stcf") == 0) {
            state.waiting = (AbstractProcessQueue*) construct_stcf_process_queue(); // add this
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
        } else {
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



