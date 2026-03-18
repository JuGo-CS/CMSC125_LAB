#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "./process-queue-adt.h"
#include "./../objects/process.h"


// SJF Queue Element
typedef struct SJFQueueElement{
    Process *process;
    struct SJFQueueElement *next;
} SJFQueueElement;

SJFQueueElement* construct_sjf_element(Process*);
void destruct_sjf_element(SJFQueueElement *);


// SJF Queue
typedef struct {
    AbstractProcessQueue queue;
    SJFQueueElement *head;        
} SJFProcessQueue;

SJFProcessQueue* construct_sjf_process_queue();

#endif