#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "./../objects/event.h"

typedef struct {
    Event *head;    
    int size;
} EventQueue;

EventQueue* construct_event_queue();
void destruct_event_queue(EventQueue*);
void enqueue_event(EventQueue*, Event*);
Event* dequeue_event(EventQueue*);

#endif