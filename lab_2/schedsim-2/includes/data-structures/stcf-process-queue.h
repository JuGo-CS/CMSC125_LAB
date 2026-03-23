#ifndef STCF_PROCESS_QUEUE_H
#define STCF_PROCESS_QUEUE_H

#include "./process-queue-adt.h"
#include "../objects/process.h"

typedef struct STCFQueueElement {
    Process* process;
    struct STCFQueueElement* next;
} STCFQueueElement;

typedef struct {
    AbstractProcessQueue queue;
    STCFQueueElement* head;
} STCFProcessQueue;

STCFProcessQueue* construct_stcf_process_queue();

#endif