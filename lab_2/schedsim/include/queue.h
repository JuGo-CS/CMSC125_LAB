#ifndef QUEUE_H
#define QUEUE_H

#include "./../include/process.h"
#include <stdbool.h>

typedef struct Queue_process{
    Process *process;
    struct Queue_process *next_process;
} Queue_process;

typedef struct{
    int size;
    Queue_process *head_process;
    Queue_process *tail_process;
} Queue;

typedef struct{
    Process *processes;
    bool is_queued;
} Process_queued;

int queue_size(Queue *queue);
void enqueue(Queue *queue, Process *new_process);
Process* dequeue(Queue *queue);

#endif


