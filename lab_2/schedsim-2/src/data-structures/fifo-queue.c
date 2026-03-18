#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/fifo-queue.h"

// FIFO Queue Implementation
FIFOQueueElement* construct_fifo_element(void* data) {
    FIFOQueueElement* element = malloc(sizeof(FIFOQueueElement));
    if (element) {
        element->data = data;
        element->next = NULL;
    }
    return element; 
}
void destruct_fifo_element(FIFOQueueElement* element) {
    free(element->data);
    free(element);
}

// FIFO Implementation
void fifo_enqueue(AbstractQueue* self, void* data) {
    FIFOQueue* fq = (FIFOQueue*) self; 

    FIFOQueueElement* node = malloc(sizeof(FIFOQueueElement));
    node->data = data;
    node->next = NULL;

    if (fq->tail == NULL) {
        fq->head = fq->tail = node;
    } else {
        fq->tail->next = node;
        fq->tail = node;
    }
    self->size++;
}

void* fifo_dequeue(AbstractQueue* self) {
    FIFOQueue* fq = (FIFOQueue*)self;
    if (fq->head == NULL) return NULL;

    FIFOQueueElement* temp = fq->head;
    void* data = temp->data;

    fq->head = fq->head->next;
    if (fq->head == NULL) fq->tail = NULL;

    free(temp);
    self->size--;
    return data;
}

FIFOQueue* construct_fifo_queue() {
    FIFOQueue* fq = malloc(sizeof(FIFOQueue));
    fq->queue.enqueue = fifo_enqueue;
    fq->queue.dequeue = fifo_dequeue;
    fq->queue.size = 0;
    fq->head = NULL;
    fq->tail = NULL;
    return fq;
}

void destruct_fifo_queue(FIFOQueue* fq) {
    while (fq->head != NULL) {
        free(fifo_dequeue((AbstractQueue*)fq));
    }
    free(fq);
}
