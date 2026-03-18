#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/command-line-parser.h"
#include "./../include/discrete-event-simulator.h"
#include "./../include/scheduler.h"

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

    simulate_scheduler(&state);
    free_scheduler_processes(&state);
    free(args);

    // if(args->algorithm) {
    //     if(strcasecmp(args->algorithm, "fcfs") == 0) {
    //         schedule_fcfs(&state);
    //     }
    //     else if(strcasecmp(args->algorithm, "rr") == 0) {
    //         schedule_rr(&state, args->quantum_time);
    //     }
    //     else if(strcasecmp(args->algorithm, "sjf") == 0) {
    //         schedule_sjf(&state);
    //     }
    //     else if(strcasecmp(args->algorithm, "stcf") == 0) {
    //         schedule_stcf(&state);
    //     }
    //     else {
    //         fprintf(stderr, "Invalid Algorithm!\n");
    //         exit(1);
    //     }
    // } else {
    //     fprintf(stderr, "No Algorithm Found!\n");
    //     exit(1);
    // }

    // calculate_metrics(&state, state.num_processes);
    // print_metrics(&state);
    return 0;
}



