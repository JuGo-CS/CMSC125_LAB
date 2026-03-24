#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/compare.h"
#include "../includes/scheduler.h"
#include "../includes/simulator.h"
#include "../includes/data-structures/fcfs-process-queue.h"
#include "../includes/data-structures/sjf-process-queue.h"
#include "../includes/data-structures/rr-process-queue.h"
#include "../includes/data-structures/stcf-process-queue.h"
#include "../includes/data-structures/mlfq-process-queue.h"
#include "../includes/mlfq-config.h"

/**
 * Helper: Copy process array
 */
static void copy_processes(SchedulerState* dest, SchedulerState* src) {
    dest->num_processes = src->num_processes;
    for (int i = 0; i < src->num_processes; i++) {
        dest->processes[i] = construct_process(
            src->processes[i]->pid,
            src->processes[i]->arrival_time,
            src->processes[i]->burst_time
        );
        dest->processes[i]->remaining_time = dest->processes[i]->burst_time;
    }
}

/**
 * Helper: Calculate average metrics
 */
static void calculate_averages(SchedulerState* state, double* avg_tt, double* avg_wt, double* avg_rt) {
    *avg_tt = 0.0;
    *avg_wt = 0.0;
    *avg_rt = 0.0;
    
    for (int i = 0; i < state->num_processes; i++) {
        Process* p = state->processes[i];
        *avg_tt += p->turnaround_time;
        *avg_wt += p->waiting_time;
        *avg_rt += p->response_time;
    }
    
    *avg_tt /= state->num_processes;
    *avg_wt /= state->num_processes;
    *avg_rt /= state->num_processes;
}

/**
 * Helper: Count context switches from Gantt chart
 */
static int count_context_switches(SchedulerState* state) {
    if (state->gantt_size <= 1) return 0;
    
    int switches = 0;
    for (int i = 1; i < state->gantt_size; i++) {
        if (state->gantt[i].name[0] != state->gantt[i-1].name[0]) {
            switches++;
        }
    }
    return switches;
}

/**
 * Helper: Free scheduler state (but keep processes)
 */
static void free_scheduler_state(SchedulerState* state) {
    if (state->waiting) {
        state->waiting->destruct_queue(state->waiting);
        state->waiting = NULL;
    }
    state->gantt_size = 0;
}

/**
 * Run comparison of all algorithms
 */
void run_comparison(SchedulerState base_state, char* input_description) {
    AlgorithmResult results[5];
    memset(results, 0, sizeof(results));
    
    printf("\n");
    printf("=== Algorithm Comparison for %s ===\n\n", input_description);
    
    // Test 1: FCFS
    {
        printf("\n*First Come, First Serve Scheduler*");
        printf("\n--------------------------------------------------");
        SchedulerState state = {0};
        copy_processes(&state, &base_state);
        state.waiting = (AbstractProcessQueue*) construct_fcfs_process_queue();
        
        simulate_scheduler(&state, schedule_fcfs);
        
        strcpy(results[0].algorithm_name, "FCFS");
        calculate_averages(&state, &results[0].avg_turnaround, &results[0].avg_waiting, &results[0].avg_response);
        results[0].context_switches = count_context_switches(&state);
        
        free_scheduler_state(&state);
    }
    
    // Test 2: SJF
    {
        printf("\n\n*Shortest Job First Scheduler*");
        printf("\n--------------------------------------------------");
        SchedulerState state = {0};
        copy_processes(&state, &base_state);
        state.waiting = (AbstractProcessQueue*) construct_sjf_process_queue();
        
        simulate_scheduler(&state, schedule_sjf);
        
        strcpy(results[1].algorithm_name, "SJF");
        calculate_averages(&state, &results[1].avg_turnaround, &results[1].avg_waiting, &results[1].avg_response);
        results[1].context_switches = count_context_switches(&state);
        
        free_scheduler_state(&state);
    }
    
    // Test 3: STCF
    {
        printf("\n\n*Shortest-Time to Completion First Scheduler*");
        printf("\n--------------------------------------------------");
        SchedulerState state = {0};
        copy_processes(&state, &base_state);
        state.waiting = (AbstractProcessQueue*) construct_stcf_process_queue();
        
        simulate_scheduler(&state, schedule_stcf);
        
        strcpy(results[2].algorithm_name, "STCF");
        calculate_averages(&state, &results[2].avg_turnaround, &results[2].avg_waiting, &results[2].avg_response);
        results[2].context_switches = count_context_switches(&state);
        
        free_scheduler_state(&state);
    }
    
    // Test 4: Round Robin (q=10)
    {
        printf("\n\n*Round Robin Scheduler*");
        printf("\n--------------------------------------------------");
        SchedulerState state = {0};
        copy_processes(&state, &base_state);
        state.quantum_time = 10;
        state.waiting = (AbstractProcessQueue*) construct_rr_process_queue();
        
        simulate_scheduler(&state, schedule_rr);
        
        strcpy(results[3].algorithm_name, "RR (q=10)");
        calculate_averages(&state, &results[3].avg_turnaround, &results[3].avg_waiting, &results[3].avg_response);
        results[3].context_switches = count_context_switches(&state);
        
        free_scheduler_state(&state);
    }
    
    // Test 5: MLFQ
    {
        printf("\n\n*Multilevel Feedback Queue Scheduler*");
        printf("\n--------------------------------------------------");
        SchedulerState state = {0};
        copy_processes(&state, &base_state);
        
        // Load default MLFQ config
        MLFQConfig* config = load_mlfq_config("workloads/mlfq_config.txt");
        if (!config) {
            fprintf(stderr, "Warning: Could not load default MLFQ config\n");
            // Use fallback config
            config = malloc(sizeof(MLFQConfig));
            memset(config, 0, sizeof(MLFQConfig));
            config->num_levels = 3;
            config->levels[0].time_quantum = 10;
            config->levels[0].allotment = 50;
            config->levels[1].time_quantum = 30;
            config->levels[1].allotment = 150;
            config->levels[2].time_quantum = -1;
            config->levels[2].allotment = -1;
            config->boost_period = 200;
        }
        
        state.waiting = (AbstractProcessQueue*) construct_mlfq_process_queue(config);
        
        simulate_scheduler(&state, schedule_mlfq);
        
        strcpy(results[4].algorithm_name, "MLFQ");
        calculate_averages(&state, &results[4].avg_turnaround, &results[4].avg_waiting, &results[4].avg_response);
        results[4].context_switches = count_context_switches(&state);
        
        free_scheduler_state(&state);
        free_mlfq_config(config);
    }
    
    // Print comparison table
    printf("\n\n=== COMPARISON ===\n");
    printf("%-15s | %-8s | %-8s | %-8s | %-15s\n", "Algorithm", "Avg TT", "Avg WT", "Avg RT", "Context Switches");
    printf("----------------+----------+----------+----------+----------------\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-15s | %8.1f | %8.1f | %8.1f | %15d\n",
               results[i].algorithm_name,
               results[i].avg_turnaround,
               results[i].avg_waiting,
               results[i].avg_response,
               results[i].context_switches);
    }
    
    printf("\n");
}
