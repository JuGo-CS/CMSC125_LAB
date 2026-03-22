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

    if (rq->pending_tail == NULL) {
        rq->pending_head = rq->pending_tail = node;
    } else {
        rq->pending_tail->next = node;
        rq->pending_tail = node;
    }

    self->size++;
}

// Reinsert preempted running process into the round buffer
void rr_requeue(RRProcessQueue* rq, Process* process) {
    RRQueueElement* node = construct_rr_element(process);

    if (rq->round_tail == NULL) {
        rq->round_head = rq->round_tail = node;
    } else {
        rq->round_tail->next = node;
        rq->round_tail = node;
    }

    rq->queue.size++;
}

static void rr_append_pending_to_active(RRProcessQueue* rq) {
    if (rq->pending_head == NULL) return;

    if (rq->active_tail == NULL) {
        rq->active_head = rq->pending_head;
        rq->active_tail = rq->pending_tail;
    } else {
        rq->active_tail->next = rq->pending_head;
        rq->active_tail = rq->pending_tail;
    }

    rq->pending_head = rq->pending_tail = NULL;
}

Process* rr_dequeue(AbstractProcessQueue* self) {
    RRProcessQueue* rq = (RRProcessQueue*) self;

    // If no active entries, finish current round and start a new one
    if (rq->active_head == NULL) {
        // Move prepared round entries into active
        rq->active_head = rq->round_head;
        rq->active_tail = rq->round_tail;
        rq->round_head = rq->round_tail = NULL;

        // Add pending arrivals after round entries
        rr_append_pending_to_active(rq);
    }

    if (rq->active_head == NULL) {
        return NULL;
    }

    RRQueueElement* temp = rq->active_head;
    Process* process = temp->process;
    rq->active_head = rq->active_head->next;
    if (rq->active_head == NULL) rq->active_tail = NULL;

    destruct_rr_element(temp);
    self->size--;
    return process;
}

void destruct_rr_process_queue(AbstractProcessQueue* self) {
    RRProcessQueue* rq = (RRProcessQueue*) self;
    RRQueueElement* temp;

    while (rq->active_head != NULL) {
        temp = rq->active_head;
        rq->active_head = rq->active_head->next;
        destruct_rr_element(temp);
    }
    while (rq->round_head != NULL) {
        temp = rq->round_head;
        rq->round_head = rq->round_head->next;
        destruct_rr_element(temp);
    }
    while (rq->pending_head != NULL) {
        temp = rq->pending_head;
        rq->pending_head = rq->pending_head->next;
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

    rq->active_head = rq->active_tail = NULL;
    rq->round_head = rq->round_tail = NULL;
    rq->pending_head = rq->pending_tail = NULL;

    return rq;
}