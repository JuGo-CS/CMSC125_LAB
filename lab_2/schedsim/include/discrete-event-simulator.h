/**
 * This header file manages the scheduling process using a discrete event simulator. 
 * An event is a moment of time where a change of state occurs in a system.  
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "./process.h"
#include "./scheduler.h"

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
void simulate_scheduler(SchedulerState*);

#endif