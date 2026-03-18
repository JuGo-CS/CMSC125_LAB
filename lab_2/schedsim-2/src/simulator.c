#include <stdio.h>
#include "../includes/simulator.h"
#include "../includes/objects/event.h"
#include "../includes/data-structures/event-queue.h"
#include "../includes/simulator.h"
#include "../includes/metrics.h"


EventQueue* event_queue;
void *scheduler;

void initialize_events(SchedulerState*);
void handle_arrival(SchedulerState*, Process*);
void handle_completion(SchedulerState*, Process*);

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
        }
        destruct_event(current);
    }
    destruct_event_queue(event_queue);
    calculate_metrics(state, state->num_processes);
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
        algo(state);

        enqueue_event(event_queue, construct_event(
            process->finish_time,
            EVENT_COMPLETION,
            process, 
            NULL
        ));
    } else {
        state->waiting->enqueue(state->waiting, process);
    }
}

void handle_completion(SchedulerState* state, Process* process) {
    state->running = NULL;
    if (state->waiting->size > 0) {
        state->running = state->waiting->dequeue(state->waiting);
        int (*algo)(SchedulerState*) = (int (*)(SchedulerState*))scheduler;
        algo(state);
        enqueue_event(event_queue, construct_event(
            state->running->finish_time,
            EVENT_COMPLETION,
            state->running, 
            NULL
        ));
    } 
}
