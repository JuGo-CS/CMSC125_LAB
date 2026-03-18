#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/discrete-event-simulator.h"
#include "./../include/process.h"
#include "./../include/queue.h"


Event* event_queue_head = NULL;

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
    if (event) {
        free(event);
    }
}

void insert_event(Event* new_event) {
    if (!new_event) return;
    
    if (!event_queue_head || new_event->time < event_queue_head->time) {
        new_event->next = event_queue_head;
        event_queue_head = new_event;
        return;
    }
    
    Event* current = event_queue_head;
    while (current->next && current->next->time <= new_event->time) {
        current = current->next;
    }
    
    new_event->next = current->next;
    current->next = new_event;
}

Event* pop_event() {
    if (!event_queue_head) return NULL;
    
    Event* event = event_queue_head;
    event_queue_head = event_queue_head->next;
    event->next = NULL;
    return event;
}

void free_event_queue() {
    while (event_queue_head) {
        Event* temp = event_queue_head;
        event_queue_head = event_queue_head->next;
        free(temp);
    }
}

void initialize_arrival_events(SchedulerState* state) {
    for (int i = 0; i < state->num_processes; i++) {
        Event* arrival_event = construct_event(
            state->processes[i]->arrival_time,
            EVENT_ARRIVAL,
            state->processes[i],
            NULL
        );
        insert_event(arrival_event);
    }
}


/**
 * Main discrete event simulation loop
 * Processes events chronologically and simulates scheduling
 */
void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    Event* running = NULL;
    Event* waiting[256];
    int head = 0;
    int tail = -1;
    int waiting_size = 0;
    int time = 0;

    initialize_arrival_events(state);
    
    Event *current_event;
    while (event_queue_head != NULL) {
        current_event = pop_event();
        time = current_event->time;

        if (current_event->type == EVENT_ARRIVAL) {
            printf("Process %s arrived at time %d\n", current_event->process->pid, time);
            if (!running) {
                running = current_event;
                current_event->type = EVENT_COMPLETION;
                current_event->process->start_time = time;
                current_event->process->finish_time = current_event->process->start_time + current_event->process->burst_time;
                current_event->time = current_event->process->finish_time;
                insert_event(current_event);
            } else {
                tail++;
                waiting[tail] = current_event;
                waiting_size++;
            }
        } else if (current_event->type == EVENT_COMPLETION) {
            printf("Process %s has been completed at time %d\n", current_event->process->pid, time);
            running = NULL;
            if (waiting_size > 0) {
                running = waiting[head];
                head++;
                waiting_size--;
                
                running->type = EVENT_COMPLETION;
                running->process->start_time = time;
                running->process->finish_time = running->process->start_time + running->process->burst_time;
                running->time = running->process->finish_time;
                insert_event(running);
            }
        }
    }
    
    // Clean up event queue
    free_event_queue();
}

