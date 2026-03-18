#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "./queue-adt.h"

// FIFO Queue Element
typedef struct FIFOQueueElement{
    void *data;
    struct FIFOQueueElement *next;
} FIFOQueueElement;

FIFOQueueElement* construct_fifo_element(void*);
void destruct_fifo_element(FIFOQueueElement*);


// FIFO Queue
typedef struct {
    AbstractQueue queue;
    FIFOQueueElement *head;        
    FIFOQueueElement *tail;        
} FIFOQueue;

FIFOQueue* construct_fifo_queue();
void destruct_fifo_queue(FIFOQueue*);

#endif