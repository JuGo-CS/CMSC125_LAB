/*
 * Design decisions:
 * - Processes enter at highest priority (interactive assumption)
 * - Demoted when time allotment exhausted (revealed to be compute-heavy)
 * - Bottom level is often FCFS (no quantum/allotment limits)
 * - Boost period prevents starvation of long-running jobs
 */

#include <stdio.h>
#include <stdlib.h>
#include "./../includes/scheduler.h"
#include "./../includes/data-structures/mlfq-process-queue.h"

int schedule_mlfq(SchedulerState *state) {
    if (!state->running) {
        return 0;
    }
    
    MLFQProcessQueue* mq = (MLFQProcessQueue*) state->waiting;
    int level = state->running->priority;
    int quantum = mlfq_get_quantum(mq, level);
    int allotment = mlfq_get_allotment(mq, level);
    
    // Calculate run time for this quantum
    int run_time = 0;
    
    // Determine how long this process can run
    if (quantum == -1) {
        // FCFS level (no quantum limit)
        run_time = state->running->remaining_time;
    } else {
        // Time-quantum level
        run_time = (state->running->remaining_time <= quantum) ? 
                   state->running->remaining_time : quantum;
    }
    
    // Set start time if this is first execution (and haven't set it yet)
    if (state->running->start_time == -1) {
        state->running->start_time = state->current_time;
    }
    
    // Update allotment tracking (for demotion logic)
    state->running->time_in_queue += run_time;
    
    // Set finish_time ALWAYS if this quantum will complete the process
    // This ensures that even if boost happens, we've recorded the completion time
    if (run_time == state->running->remaining_time) {
        state->running->finish_time = state->current_time + run_time;
        state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
        state->running->waiting_time = state->running->turnaround_time - state->running->burst_time;
    }
    
    return run_time;
}

/**
 * Called by simulator to check if process should be demoted
 * Returns 1 if demoted, 0 otherwise
 */
int mlfq_check_demotion(SchedulerState *state) {
    if (!state->running) return 0;
    
    MLFQProcessQueue* mq = (MLFQProcessQueue*) state->waiting;
    int level = state->running->priority;
    int allotment = mlfq_get_allotment(mq, level);
    
    // If no allotment limit, no demotion from this level
    if (allotment == -1) return 0;
    
    // If process has exceeded allotment, demote it
    if (state->running->time_in_queue >= allotment) {
        int next_level = level + 1;
        
        // Don't go beyond bottom level
        if (next_level >= mq->num_levels) {
            next_level = mq->num_levels - 1;
        }
        
        if (next_level != level) {
            // Demote the process
            mlfq_demote_process(mq, state->running, next_level);
            state->running->priority = next_level;
            return 1;
        }
    }
    
    return 0;
}
