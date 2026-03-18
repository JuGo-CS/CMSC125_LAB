#include <stdio.h>
#include "../includes/simulator.h"
#include "../includes/objects/event.h"
#include "../includes/data-structures/event-queue.h"

void initialize_events(SchedulerState* state, EventQueue* q) {
    for (int i = 0; i < state->num_processes; i++) {
        enqueue_event(q, construct_event(
            state->processes[i]->arrival_time,
            EVENT_ARRIVAL,
            state->processes[i], 
            NULL
        ));
    } 
}

void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    EventQueue* q = construct_event_queue();
    initialize_events(state, q);

    Event* curr = q->head;
    while (curr != NULL) {
        printf("Process %s\n", curr->process->pid);
        curr = curr->next;
    }
    destruct_event_queue(q);
    scheduling_algorithm(state);
}