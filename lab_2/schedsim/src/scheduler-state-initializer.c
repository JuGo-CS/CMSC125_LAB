#include "./../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_processes_from_file(SchedulerState *state, char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (!fptr) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }

    char line[256];
    char pid[PID_LENGTH];
    int arrival_time, burst_time;
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0; 
        
        char first_char;
        if (sscanf(line, " %c", &first_char) != 1 || first_char == '#') {
            continue;
        }
    
        sscanf(line, "%15[^ ] %d %d", pid, &arrival_time, &burst_time);
        Process* p = construct_process(pid, arrival_time, burst_time);
        if (p) {
            state->processes[state->num_processes++] = p;
        }
    }
    fclose(fptr);
}

void get_processes_from_string(SchedulerState *state, char *process_str) {
    char *token = strtok(process_str, ";");
    while (token && state->num_processes < MAX_PROCESSES) {
        char pid[PID_LENGTH];
        int arrival_time, burst_time;
        sscanf(token, "%15[^:]:%d:%d", pid, &arrival_time, &burst_time);
        
        Process* p = construct_process(pid, arrival_time, burst_time);
        if (p) {
            state->processes[state->num_processes++] = p;
        }
        token = strtok(NULL, ";");
    }
}
 
void free_scheduler_processes(SchedulerState *state) {
    for (int i = 0; i < state->num_processes; i++) {
        free(state->processes[i]);
    }
}