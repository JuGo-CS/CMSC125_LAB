#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/process-queue-adt.h"
#include "./../../includes/data-structures/rr-process-queue.h"

// RR Queue helper
RRQueueElement* construct_rr_element(Process* process) {
    RRQueueElement* element = malloc(sizeof(RRQueueElement));
    if (element) {
        element->process = process;
        element->next = NULL;
    }
    return element;
}

void destruct_rr_element(RRQueueElement* el) {
    free(el);
}

void rr_enqueue(AbstractProcessQueue* self, Process* process) {
    RRProcessQueue* rq = (RRProcessQueue*) self;

    RRQueueElement* node = construct_rr_element(process);
    if (rq->tail == NULL) {
        rq->head = rq->tail = node;
    } else {
        rq->tail->next = node;
        rq->tail = node;
    }

    self->size++;
}

// Reinsert preempted running process at queue tail
void rr_requeue(RRProcessQueue* rq, Process* process) {
    RRQueueElement* node = construct_rr_element(process);
    if (rq->tail == NULL) {
        rq->head = rq->tail = node;
    } else {
        rq->tail->next = node;
        rq->tail = node;
    }
    rq->queue.size++;
}

Process* rr_dequeue(AbstractProcessQueue* self) {
    RRProcessQueue* rq = (RRProcessQueue*) self;
    if (rq->head == NULL) return NULL;

    RRQueueElement* temp = rq->head;
    Process* process = temp->process;
    rq->head = rq->head->next;
    if (rq->head == NULL) rq->tail = NULL;

    destruct_rr_element(temp);
    self->size--;
    return process;
}

void destruct_rr_process_queue(AbstractProcessQueue* self) {
    RRProcessQueue* rq = (RRProcessQueue*) self;
    while (rq->head != NULL) {
        RRQueueElement* temp = rq->head;
        rq->head = rq->head->next;
        destruct_rr_element(temp);
    }
    free(rq);
}

RRProcessQueue* construct_rr_process_queue() {
    RRProcessQueue* rq = malloc(sizeof(RRProcessQueue));
    rq->queue.enqueue = rr_enqueue;
    rq->queue.dequeue = rr_dequeue;
    rq->queue.destruct_queue = destruct_rr_process_queue;
    rq->queue.size = 0;

    rq->head = rq->tail = NULL;

    return rq;
}