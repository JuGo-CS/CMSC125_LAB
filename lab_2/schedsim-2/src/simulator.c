#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../includes/simulator.h"
#include "../includes/objects/event.h"
#include "../includes/data-structures/event-queue.h"
#include "../includes/data-structures/rr-process-queue.h"
#include "../includes/data-structures/mlfq-process-queue.h"
#include "../includes/simulator.h"
#include "../includes/metrics.h"
#include "../includes/gantt.h"


EventQueue* event_queue;
void *scheduler;
int is_mlfq_scheduler = 0;
bool sort_by_burst_time = false;

void initialize_events(SchedulerState*);
void handle_arrival(SchedulerState*, Process*);
void handle_completion(SchedulerState*, Process*);
void handle_quantum_expire(SchedulerState*, Process*);
void handle_priority_boost(SchedulerState*);



void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    event_queue = construct_event_queue();
    scheduler = scheduling_algorithm;
    sort_by_burst_time = (scheduling_algorithm == schedule_sjf || scheduling_algorithm == schedule_stcf) ? 1 : 0;
    is_mlfq_scheduler = (scheduling_algorithm == schedule_mlfq) ? 1 : 0;
    initialize_events(state);

    while (event_queue->size > 0) {
        Event *current = dequeue_event(event_queue);
        state->current_time = current->time;
        
        switch (current->type) {
            case EVENT_ARRIVAL:
                handle_arrival(state, current->process);
                break;
            case EVENT_COMPLETION:
                handle_completion(state, current->process);
                break;
            case EVENT_QUANTUM_EXPIRE:
                handle_quantum_expire(state, current->process);
                break;
            case EVENT_PRIORITY_BOOST:
                handle_priority_boost(state);
                break;
        }
        destruct_event(current);
    }
    destruct_event_queue(event_queue);
    
    // SAFETY NET: Ensure any process still marked as running is marked complete
    if (state->running && state->running->finish_time == -1) {
        state->running->finish_time = state->current_time;
        state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
        state->running->waiting_time = state->running->turnaround_time - state->running->burst_time;
    }
    
    calculate_metrics(state, state->num_processes);
    print_gantt(state);
    print_metrics(state);

}

void initialize_events(SchedulerState* state) {
    for (int i = 0; i < state->num_processes; i++) {
        enqueue_event(event_queue, construct_event(
            state->processes[i]->arrival_time,
            EVENT_ARRIVAL,
            state->processes[i], 
            NULL
        ), sort_by_burst_time);
    }
    
    // For MLFQ, schedule priority boost events
    if (is_mlfq_scheduler) {
        MLFQProcessQueue* mq = (MLFQProcessQueue*) state->waiting;
        if (mq->config && mq->config->boost_period > 0) {
            // Calculate reasonable upper bound for simulation time
            // (sum of all burst times + max arrival time)
            int total_burst = 0;
            int max_arrival = 0;
            for (int i = 0; i < state->num_processes; i++) {
                total_burst += state->processes[i]->burst_time;
                if (state->processes[i]->arrival_time > max_arrival) {
                    max_arrival = state->processes[i]->arrival_time;
                }
            }
            int max_time = max_arrival + total_burst + 1000;  // Buffer for scheduling overhead
            
            int boost_time = mq->config->boost_period;
            while (boost_time <= max_time) {
                enqueue_event(event_queue, construct_event(
                    boost_time,
                    EVENT_PRIORITY_BOOST,
                    NULL,
                    NULL
                ), false);
                boost_time += mq->config->boost_period;
            }
        }
    }
}

void handle_arrival(SchedulerState* state, Process* process) {
    if (!state->running) {
        state->running = process;
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        int run_time = algo(state);

        // Collect gantt entry for this execution segment
        append_gantt_entry(state, state->running, state->current_time, run_time);

        // Update remaining time
        state->running->remaining_time -= run_time;

        // Determine if this was a completion or quantum expiration
        EventType event_type = (state->running->remaining_time == 0) ? 
                              EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;
        
        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            process, 
            NULL
        ), sort_by_burst_time);
    } 
    
    else if (scheduler == schedule_stcf && process->remaining_time < state->running->remaining_time) {
        
        //Cancel the running process's scheduled event
        remove_event_for_process(event_queue, state->running);
        state->waiting->enqueue(state->waiting, state->running);

        state->running = process;
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        int run_time = algo(state);

        append_gantt_entry(state, state->running, state->current_time, run_time);
        state->running->remaining_time -= run_time;

        EventType event_type = (state->running->remaining_time == 0) ?
                               EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;

        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            state->running,
            NULL
        ), sort_by_burst_time);

    }
    
    else {
        state->waiting->enqueue(state->waiting, process);
    }
}

void handle_completion(SchedulerState* state, Process* process) {
    // CRITICAL: Ensure finish_time is ALWAYS set when process completes
    // This prevents negative metrics even if scheduler didn't set it
    if (state->running && state->running->finish_time == -1) {
        state->running->finish_time = state->current_time;
        state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
        state->running->waiting_time = state->running->turnaround_time - state->running->burst_time;
    }
    
    state->running = NULL;
    if (state->waiting->size > 0) {
        state->running = state->waiting->dequeue(state->waiting);
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        int run_time = algo(state);

        append_gantt_entry(state, state->running, state->current_time, run_time);
        
        // Update remaining time
        state->running->remaining_time -= run_time;
        
        // Determine if this was a completion or quantum expiration
        EventType event_type = (state->running->remaining_time == 0) ? 
                              EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;
        
        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            state->running, 
            NULL
        ), sort_by_burst_time);
    }
}

void handle_quantum_expire(SchedulerState* state, Process* process) {
    // Check for MLFQ demotion
    int demoted = 0;
    if (is_mlfq_scheduler && state->running) {
        demoted = mlfq_check_demotion(state);
    }
    
    // Preempted process should stay in the active RR rotation (batch semantics)
    if (state->running) {
        if(scheduler == schedule_rr) {
            RRProcessQueue* rrq = (RRProcessQueue*) state->waiting;
            rr_requeue(rrq, state->running);
        } else {
            // For MLFQ, demotion already moved it to the new queue
            if (!demoted) {
                state->waiting->enqueue(state->waiting, state->running);
            }
        }
    }

    // Schedule next process (if any)
    if (state->waiting->size > 0) {
        state->running = state->waiting->dequeue(state->waiting);
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        int run_time = algo(state);

        append_gantt_entry(state, state->running, state->current_time, run_time);

        // Update remaining time
        state->running->remaining_time -= run_time;
        
        // SAFETY CHECK: If remaining_time is 0 (process completes), ensure finish_time is set
        if (state->running->remaining_time == 0 && state->running->finish_time == -1) {
            state->running->finish_time = state->current_time + run_time;
            state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
            state->running->waiting_time = state->running->turnaround_time - state->running->burst_time;
        }
        
        // Determine if this was a completion or quantum expiration
        EventType event_type = (state->running->remaining_time == 0) ?
                              EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;

        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            state->running,
            NULL
        ), sort_by_burst_time);
    } else {
        state->running = NULL;
    }
}

void handle_priority_boost(SchedulerState* state) {
    if (!is_mlfq_scheduler) return;
    
    MLFQProcessQueue* mq = (MLFQProcessQueue*) state->waiting;
    
    // Check if running process is about to complete
    // If so, don't boost it - let it finish first
    if (state->running && state->running->remaining_time == 0) {
        return;  // Will be handled by next event (EVENT_COMPLETION)
    }
    
    // If a process is running, save it to re-enqueue after boost
    Process* running_process = state->running;
    state->running = NULL;
    
    // Boost all waiting processes to highest priority
    mlfq_priority_boost(mq);
    
    // Re-enqueue the previously running process at highest priority  
    if (running_process) {
        // IMPORTANT: Only reset priority and allotment if not completing
        if (running_process->remaining_time > 0) {
            running_process->priority = 0;
            running_process->time_in_queue = 0;
            state->waiting->enqueue(state->waiting, running_process);
        } else {
            // Process is complete, don't re-enqueue - let it be handled as completion
            state->running = running_process;
            return;
        }
    }
    
    // Re-schedule the next process
    if (state->waiting->size > 0) {
        state->running = state->waiting->dequeue(state->waiting);
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        int run_time = algo(state);

        append_gantt_entry(state, state->running, state->current_time, run_time);

        // Update remaining time
        state->running->remaining_time -= run_time;
        
        // Determine if this was a completion or quantum expiration
        EventType event_type = (state->running->remaining_time == 0) ?
                              EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;

        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            state->running,
            NULL
        ), 0);
    } else {
        state->running = NULL;
    }
}

