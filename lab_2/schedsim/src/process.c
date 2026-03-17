#include "../include/gantt.h"
#include "../include/metrics.h"
#include "../include/process.h"
#include "../include/scheduler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum {
    EVENT_ARRIVAL,
    EVENT_COMPLETION,
    EVENT_QUANTUM_EXPIRE,
    EVENT_PRIORITY_BOOST
} EventType;

typedef struct Event {
    int time;
    EventType type;
    Process *process;
    struct Event *next;
} Event;

static Event *initialize_events(SchedulerState *state) {
    (void)state;
    return NULL;
}

static Event *pop_event(Event **queue) {
    (void)queue;
    return NULL;
}

static void handle_arrival(SchedulerState *state, Process *p) {
    (void)state;
    (void)p;
}

static void handle_completion(SchedulerState *state, Process *p) {
    (void)state;
    (void)p;
}

void simulate_scheduler(SchedulerState *state) { //void simulate_scheduler(SchedulerState *state, SchedulingAlgorithm algorithm) {
    Event *event_queue = initialize_events(state);
    
    while (event_queue != NULL) {
        Event *current = pop_event(&event_queue);
        state->current_time = current->time;
        
        switch (current->type) {
            case EVENT_ARRIVAL:
                handle_arrival(state, current->process);
                break;
            case EVENT_COMPLETION:
                handle_completion(state, current->process);
                break;
            // ... handle other events
        }
        
        free(current);
    }
        
    calculate_metrics(state, state->num_processes);
    print_metrics(state);
}
