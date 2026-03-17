/**
 * This header file defines the Process structure and related functions for a CPU scheduling 
 * simulator. The Process structure contains fields for process identification, timing information, 
 * and scheduling parameters. 
 * 
 * The construct_process function initializes a Process instance. 
 * The destruct_process function handles Process memory cleanup.
 */
#ifndef PROCESS_H
#define PROCESS_H

#define PID_LENGTH 16
typedef struct {
    char pid[PID_LENGTH];   // Process identifier
    int arrival_time;       // When process arrives
    int burst_time;         // Total CPU time needed
    int remaining_time;     // For preemptive algorithms
    int start_time;         // When first executed (for RT)
    int finish_time;        // When completed (for TT)
    int waiting_time;       // Time spent waiting
    int priority;           // For MLFQ
    int time_in_queue;      // For MLFQ allotment tracking
    int turnaround_time;
    int response_time;
    // int last_update;       // For calculating waiting_time
} Process;
Process* construct_process(char *, int, int);
void destruct_process(Process*);

#endif
