#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/sjf-process-queue.h"
#include "./../../includes/data-structures/process-queue-adt.h"
#include "./../../includes/objects/process.h"


// Priority Queue Implementation
SJFQueueElement* construct_sjf_element(Process* process) {
    SJFQueueElement* element = malloc(sizeof(SJFQueueElement));
    if (element) {
        element->process = process;
        element->next = NULL;
    }
    return element; 
}
void destruct_sjf_element(SJFQueueElement* element) {
    free(element);
}

// SJF Queue Implementation
void sjf_enqueue(AbstractProcessQueue* self, Process* data) {
    SJFProcessQueue * q = (SJFProcessQueue *) self; 
    SJFQueueElement* node = construct_sjf_element(data);

    if (q->head == NULL) {
        q->head = node;     
    } else {
        SJFQueueElement* curr = q->head;
        SJFQueueElement* prev = NULL;

        while (curr != NULL && data->burst_time >= curr->process->burst_time) {
            prev = curr;
            curr = curr->next;
        }
        
        node->next = curr;

        if (prev == NULL) {
            q->head = node;
        } else {
            prev->next = node;
        }
    } 
    self->size++;
}

Process* sjf_dequeue(AbstractProcessQueue* self) {
    SJFProcessQueue * q = (SJFProcessQueue *)self;
    if (q->head == NULL) return NULL;

    SJFQueueElement* temp = q->head;
    Process* data = temp->process;

    q->head = q->head->next;

    free(temp);
    self->size--;
    return data;
}

void destruct_sjf_process_queue(AbstractProcessQueue* self) {
    SJFProcessQueue* fq = (SJFProcessQueue*) self;
    while (fq->head != NULL) {
        SJFQueueElement* temp = fq->head;
        fq->head = fq->head->next;
        free(temp); 
    }
    free(fq);
}

SJFProcessQueue* construct_sjf_process_queue() {
    SJFProcessQueue * q = malloc(sizeof(SJFProcessQueue));
    q->queue.enqueue = sjf_enqueue;
    q->queue.dequeue = sjf_dequeue;
    q->queue.destruct_queue = destruct_sjf_process_queue;
    q->queue.size = 0;
    q->head = NULL;
    return q;
}

