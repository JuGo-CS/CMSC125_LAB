#ifndef SCHEDULER_STATE_H

#include "./objects/process.h" 
#define MAX_PROCESSES 256

// Scheduler State
typedef struct {
    Process* processes[MAX_PROCESSES];   // Array of all processes
    int num_processes;                   // Number of processes
    int current_time;                    // Current simulation time
} SchedulerState;
void get_processes_from_file(SchedulerState*, char*);
void get_processes_from_string(SchedulerState*, char*);
void free_scheduler_processes(SchedulerState*);

// Scheduling algorithms
// Return 0 on success, -1 on error (command line etiquette)
int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state, int quantum);
void initialize_rr(SchedulerState *state);
// int schedule_mlfq(SchedulerState *state, MLFQConfig *config);



#endif