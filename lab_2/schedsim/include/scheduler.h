/**
 * This header file holds the SchedulerState structure which holds the state 
 * of the scheduler, including the list of processes and the current simulation time.
 * Morover, functions that set up the scheduler are also declared here.
 *  
 * Also, the header file declares the scheduling algorithm function prototypes.
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "./../include/process.h" 
#define MAX_PROCESSES 256

// Scheduler State
typedef struct {
    Process* processes[MAX_PROCESSES];   // Array of all processes
    int num_processes;                   // Number of processes
    int current_time;                    // Current simulation time
    Process processes[MAX_PROCESSES]; // Array of all processes
    int num_processes;      // Number of processes
    int current_time;       // Current simulation time
    // ... additional fields for metrics, Gantt chart, etc.
    // Recall: CMSC 141

    GanttEntry gantt[10000];   // or define a MAX_GANTT_ENTRIES
    int gantt_size;
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

