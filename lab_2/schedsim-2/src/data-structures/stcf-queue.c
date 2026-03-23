#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/stcf-process-queue.h"
#include "./../../includes/data-structures/process-queue-adt.h"
#include "./../../includes/objects/process.h"

STCFQueueElement* construct_stcf_element(Process* process) {
    STCFQueueElement* element = malloc(sizeof(STCFQueueElement));
    if (element) {
        element->process = process;
        element->next = NULL;
    }
    return element;
}

void stcf_enqueue(AbstractProcessQueue* self, Process* data) {
    STCFProcessQueue* q = (STCFProcessQueue*) self;
    STCFQueueElement* node = construct_stcf_element(data);

    if (q->head == NULL) {
        q->head = node;
    } else {
        STCFQueueElement* curr = q->head;
        STCFQueueElement* prev = NULL;

        while (curr != NULL && data->remaining_time >= curr->process->remaining_time) {
            prev = curr;
            curr = curr->next;
        }

        node->next = curr;
        if (prev == NULL) q->head = node;
        else prev->next = node;
    }
    self->size++;
}

Process* stcf_dequeue(AbstractProcessQueue* self) {
    STCFProcessQueue* q = (STCFProcessQueue*) self;
    if (q->head == NULL) return NULL;

    STCFQueueElement* temp = q->head;
    Process* data = temp->process;
    q->head = q->head->next;
    free(temp);
    self->size--;
    return data;
}

void destruct_stcf_process_queue(AbstractProcessQueue* self) {
    STCFProcessQueue* q = (STCFProcessQueue*) self;
    while (q->head != NULL) {
        STCFQueueElement* temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
    free(q);
}

STCFProcessQueue* construct_stcf_process_queue() {
    STCFProcessQueue* q = malloc(sizeof(STCFProcessQueue));
    q->queue.enqueue = stcf_enqueue;
    q->queue.dequeue = stcf_dequeue;
    q->queue.destruct_queue = destruct_stcf_process_queue;
    q->queue.size = 0;
    q->head = NULL;
    return q;
}