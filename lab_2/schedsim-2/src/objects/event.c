#include <stdlib.h>
#include "./../../includes/objects/event.h" 

Event* construct_event(int time, EventType type, Process* process, Event* next_event) {
    Event* event = (Event *) calloc(1, sizeof(Event));
    if (event) {
        event->time = time;
        event->type = type;
        event->process = process;
        event->next = next_event;
    }
    return event;
}

void destruct_event(Event* event) {
    free(event);
}