#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/discrete-event-simulator.h"
#include "./../include/process.h"

Event* construct_event(int time, EventType type, Process* process, Event* next_event) {
    Event* event = (Event *) malloc(sizeof(Event));
    if (event) {
        memset(event, 0, sizeof(Event));
        event->time = time;
        event->type = type;
        event->process = process;
        event->next = next_event;
    }
    return event;
}

void destruct_event(Event* event) {
    free(event->process);
    free(event);
}

void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    scheduling_algorithm(state);
}

