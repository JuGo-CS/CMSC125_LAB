#ifndef fcfs_QUEUE_H
#define fcfs_QUEUE_H

#include "./process-queue-adt.h"

// FCFS Queue Element
typedef struct FCFSQueueElement{
    Process *process;
    struct FCFSQueueElement *next;
} FCFSQueueElement;

FCFSQueueElement* construct_fcfs_element(Process*);
void destruct_fcfs_element(FCFSQueueElement*);


// FCFS Queue
typedef struct {
    AbstractProcessQueue queue;
    FCFSQueueElement *head;        
    FCFSQueueElement *tail;        
} FCFSProcessQueue;

FCFSProcessQueue* construct_fcfs_process_queue();
#endif