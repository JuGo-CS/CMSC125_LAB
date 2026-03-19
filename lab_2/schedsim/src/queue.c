#include "./../include/process.h"
#include "./../include/scheduler.h"
#include <stdlib.h>
#include "./../include/queue.h"
#include "./../include/queue_utils.h"

// Return how many items are currently in the queue.
int queue_size(Queue *queue){
    return queue->size;
}

// Add a process pointer to the end of the queue.
// - queue: the queue to enqueue into
// - new_process: pointer to the process being queued
void enqueue(Queue *queue, Process *new_process){
    Queue_process *new_node = malloc(sizeof(Queue_process));
    new_node->process = new_process;
    new_node->next_process = NULL;

    if(queue->size == 0){
        queue->head_process = new_node;
        queue->tail_process = new_node;
    }
    else{
        queue->tail_process->next_process = new_node;
        queue->tail_process = new_node;
    }

    queue->size++;
}

// Remove and return the oldest process in the queue.
// Returns NULL if the queue is empty (caller should not do this currently).
Process* dequeue(Queue *queue){

    Queue_process *curr_head = queue->head_process;
    Process *p = curr_head->process;

    queue->head_process = curr_head->next_process;

    if(queue->head_process == NULL){
        queue->tail_process = NULL;
    }

    free(curr_head);
    queue->size--;

    return p;
}

<<<<<<< HEAD
void initialize_process_for_queue(SchedulerState *state, Process_queued *processes){
    for(int i = 0; i < state->num_processes; i++){
        processes[i].processes = state->processes[i];
        // processes[i].is_queued = false;
    }
}
=======
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
