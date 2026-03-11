#include <stdio.h>
#include <stdlib.h>
#include "./../include/process.h"
#include "./../include/scheduler.h"

void calculate_metrics(SchedulerState *state) {
    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];
        
        // Turnaround time = Finish time - Arrival time
        p->turnaround_time =  p->finish_time - p->arrival_time;
        
        // Waiting time = Turnaround time - Burst time
        p->waiting_time =  p->turnaround_time - p->burst_time;
        
        // Response time = Start time - Arrival time
        p->response_time =  p->start_time - p->arrival_time;
    }
}

double calculate_average_turnaround(SchedulerState *state) {
    double sum = 0.0;
    for (int i = 0; i < state->num_processes; i++) {
        sum +=  state->processes[i].turnaround_time;
    }
    return sum / state->num_processes;
}

void print_metrics(SchedulerState *state) {
    int total_tt = 0, total_wt = 0, total_rt = 0;
    printf("\n\n=== Metrics ===\n");
    // Use %-7s to left-align the string in a 7-character space
    printf("%-8s|  AT  |  BT  |  FT  |   TT   |   WT   |   RT   \n", "Process");
    printf("--------|------|------|------|--------|--------|--------\n");

    for (int i = 0; i < state->num_processes; i++) {
        Process *p = &state->processes[i];
        
        // Calculate totals for the average later
        int tt = p->finish_time - p->arrival_time;
        int wt = tt - p->burst_time;
        int rt = p->start_time - p->arrival_time;
        
        total_tt += tt;
        total_wt += wt;
        total_rt += rt;

        printf("%-8s| %4d | %4d | %4d | %6d | %6d | %6d\n", 
               p->pid, p->arrival_time, p->burst_time, 
               p->finish_time, tt, wt, rt);
    }
    printf("--------|------|------|------|--------|--------|-------\n");
    printf("%-8s|      |      |      | %6.2f | %6.2f | %6.2f\n", 
        "Average", 
        (float)total_tt / state->num_processes, 
        (float)total_wt / state->num_processes, 
        (float)total_rt / state->num_processes);
}
