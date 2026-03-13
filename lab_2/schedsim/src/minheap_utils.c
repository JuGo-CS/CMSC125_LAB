
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/minheap.h"

void inserting_process_heap(Processes_pointer *processes, int *processes_unqueued, MinHeap *heap, SchedulerState *state, int index){

    Processes_pointer holder = processes[index];

    processes[index] = processes[*processes_unqueued - 1];
    (*processes_unqueued)--;

    int process_index = holder.processes - state->processes;
    heap_insert(heap, state, process_index);
}

void check_arrivals_heap(SchedulerState *state, Processes_pointer *processes, int *processes_unqueued, MinHeap *heap){
    for(int i = 0; i < *processes_unqueued; i++){
        if(processes[i].processes->arrival_time <= state->current_time){
            inserting_process_heap(processes, processes_unqueued, heap, state, i);
            i--;
        }
    }
}