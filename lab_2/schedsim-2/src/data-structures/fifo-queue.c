#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/process-queue-adt.h"
#include "./../../includes/data-structures/fcfs-process-queue.h"

// FCFS Queue Implementation
FCFSQueueElement* construct_fcfs_element(Process* process) {
    FCFSQueueElement* element = malloc(sizeof(FCFSQueueElement));
    if (element) {
        element->process = process;
        element->next = NULL;
    }
    return element; 
}
void destruct_fcfs_element(FCFSQueueElement* element) {
    free(element->process);
    free(element);
}

// fcfs Implementation
void fcfs_enqueue(AbstractProcessQueue* self, Process* process) {
    FCFSProcessQueue* fq = (FCFSProcessQueue*) self; 

    FCFSQueueElement* node = construct_fcfs_element(process);
    if (fq->tail == NULL) {
        fq->head = fq->tail = node;
    } else {
        fq->tail->next = node;
        fq->tail = node;
    }
    self->size++;
}

Process* fcfs_dequeue(AbstractProcessQueue* self) {
    FCFSProcessQueue* fq = (FCFSProcessQueue*) self;
    if (fq->head == NULL) return NULL;

    FCFSQueueElement* temp = fq->head;
    Process* process = temp->process; 

    fq->head = fq->head->next;
    if (fq->head == NULL) fq->tail = NULL;

    free(temp);
    self->size--;
    return process;
}

FCFSProcessQueue* construct_fcfs_process_queue() {
    FCFSProcessQueue* fq = malloc(sizeof(FCFSProcessQueue));
    fq->queue.enqueue = fcfs_enqueue;
    fq->queue.dequeue = fcfs_dequeue;
    fq->queue.size = 0;
    fq->head = NULL;
    fq->tail = NULL;
    return fq;
}

void destruct_fcfs_process_queue(FCFSProcessQueue* fq) {
    while (fq->head != NULL) {
        free(fcfs_dequeue((AbstractProcessQueue*)fq));
    }
    free(fq);
}
