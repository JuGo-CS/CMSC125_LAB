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

    static struct option long_options[] = {
        {"algorithm", required_argument, 0, 'a'},
        {"processes", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "a:p:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                algorithm = optarg;
                break;  
            case 'p':
                process_str = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s --algorithm=TYPE --processes=\"...\"\n", argv[0]);
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
        for(int i = 0; i < state.num_processes; i++) {
            printf("Loaded Process: PID=%s, Arrival=%d, Burst=%d, Remaining=%d, Start=%d, Finish=%d, Waiting=%d, Priority=%d, QueueTime=%d\n", 
                state.processes[i].pid, 
                state.processes[i].arrival_time, 
                state.processes[i].burst_time, 
                state.processes[i].remaining_time,
                state.processes[i].start_time,
                state.processes[i].finish_time,
                state.processes[i].waiting_time,
                state.processes[i].priority,
                state.processes[i].time_in_queue
            );
        }
        printf("Scheduling algorithm: %s\n", algorithm);
    } else {
        fprintf(stderr, "No processes provided. Use --processes=\"pid,arrival,burst;...\"\n");
        exit(1);    
    }
    return 0;
}



