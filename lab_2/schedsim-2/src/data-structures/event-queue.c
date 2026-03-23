#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/event-queue.h"
#include "./../../includes/objects/event.h"

void enqueue_event(EventQueue* q, Event* event) {
    if (q->head == NULL) {
        q->head = event;     
    } else {
        Event* curr = q->head;
        Event* prev = NULL;

        while (curr != NULL && event->time >= curr->time) {

            if((event->time < curr->time) || (event->time == curr->time && event->type == EVENT_ARRIVAL)){
                break;
            }

            prev = curr;
            curr = curr->next;
        }
        
        event->next = curr;

        if (prev == NULL) {
            q->head = event;
        } else {
            prev->next = event;
        }
    } 
    q->size++;
}

Event* dequeue_event(EventQueue* q) {
    if (q->head == NULL) return NULL;

    Event* temp = q->head;
    q->head = q->head->next;
    q->size--;
    return temp;
}

EventQueue* construct_event_queue() {
    EventQueue* q = calloc(1, sizeof(EventQueue));
    q->head = NULL;
    return q;
}
void destruct_event_queue(EventQueue* q) {
    while (q->head != NULL) {
        free(dequeue_event(q));
    }
    free(q);
}

Event* peek_next_arrival(EventQueue* q) {
    Event* curr = q->head;
    while (curr != NULL) {
        if (curr->type == EVENT_ARRIVAL) return curr;
        curr = curr->next;
    }
    return NULL;  // no pending arrivals
}

void remove_event_for_process(EventQueue* q, Process* process) {
    Event* curr = q->head;
    Event* prev = NULL;

    while (curr != NULL) {
        if (curr->process == process) {
            if (prev == NULL) {
                q->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            q->size--;
            destruct_event(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}



