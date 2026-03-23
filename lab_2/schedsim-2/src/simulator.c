#include <stdio.h>
#include <string.h>
#include "../includes/simulator.h"
#include "../includes/objects/event.h"
#include "../includes/data-structures/event-queue.h"
#include "../includes/data-structures/rr-process-queue.h"
#include "../includes/simulator.h"
#include "../includes/metrics.h"
#include "../includes/gantt.h"


EventQueue* event_queue;
void *scheduler;

void initialize_events(SchedulerState*);
void handle_arrival(SchedulerState*, Process*);
void handle_completion(SchedulerState*, Process*);
void handle_quantum_expire(SchedulerState*, Process*);



void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    event_queue = construct_event_queue();
    scheduler = scheduling_algorithm;
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
        }
        destruct_event(current);
    }
    destruct_event_queue(event_queue);
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
        ));
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
        ));
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
        ));

    }
    
    else {
        state->waiting->enqueue(state->waiting, process);
    }
}

void handle_completion(SchedulerState* state, Process* process) {
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
        ));
    }
}

void handle_quantum_expire(SchedulerState* state, Process* process) {
    // Preempted process should stay in the active RR rotation (batch semantics)
    if (state->running) {
        if(scheduler == schedule_rr) {
            RRProcessQueue* rrq = (RRProcessQueue*) state->waiting;
            rr_requeue(rrq, state->running);
        } else {
            state->waiting->enqueue(state->waiting, state->running);
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
        
        // Determine if this was a completion or quantum expiration
        EventType event_type = (state->running->remaining_time == 0) ?
                              EVENT_COMPLETION : EVENT_QUANTUM_EXPIRE;

        enqueue_event(event_queue, construct_event(
            state->current_time + run_time,
            event_type,
            state->running,
            NULL
        ));
    } else {
        state->running = NULL;
    }
}

