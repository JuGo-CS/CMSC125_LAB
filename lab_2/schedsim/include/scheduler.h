#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_PROCESSES 100
#include "./../include/process.h"

typedef struct {
    Process processes[MAX_PROCESSES]; // Array of all processes
    int num_processes;      // Number of processes
    int current_time;       // Current simulation time
    // ... additional fields for metrics, Gantt chart, etc.
    // Recall: CMSC 141
} SchedulerState;

// Return 0 on success, -1 on error (command line etiquette)
int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state, int quantum);
void initialize_rr(SchedulerState *state);
// int schedule_mlfq(SchedulerState *state, MLFQConfig *config);

#endif

