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
    EventQueue* q = malloc(sizeof(EventQueue));
    q->head = NULL;
    return q;
}
void destruct_event_queue(EventQueue* q) {
    while (q->head != NULL) {
        free(dequeue_event(q));
    }
    free(q);
}



