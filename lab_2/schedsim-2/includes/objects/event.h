#ifndef EVENT_H
#define EVENT_H

#include "./process.h"

typedef enum {
    EVENT_ARRIVAL,              
    EVENT_COMPLETION,            
    EVENT_QUANTUM_EXPIRE,
    EVENT_PRIORITY_BOOST
} EventType;

typedef struct Event {
    int time;
    EventType type;
    Process* process;
    struct Event *next;
} Event;

Event* construct_event(int, EventType, Process*, Event*);
void destruct_event(Event*);

#endif