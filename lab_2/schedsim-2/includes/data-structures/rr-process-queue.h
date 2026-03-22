#ifndef RR_QUEUE_H
#define RR_QUEUE_H

#include "./process-queue-adt.h"

typedef struct RRQueueElement {
    Process* process;
    struct RRQueueElement* next;
} RRQueueElement;

typedef struct {
    AbstractProcessQueue queue;
    RRQueueElement* active_head;
    RRQueueElement* active_tail;
    RRQueueElement* round_head;
    RRQueueElement* round_tail;
    RRQueueElement* pending_head;
    RRQueueElement* pending_tail;
} RRProcessQueue;

RRProcessQueue* construct_rr_process_queue();
void rr_requeue(RRProcessQueue* rq, Process* process);

#endif