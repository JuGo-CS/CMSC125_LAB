
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/minheap.h"

void inserting_process_heap(Processes_pointer *processes, int *notinserted_counter, MinHeap *heap, SchedulerState *state, int index){

    Processes_pointer holder = processes[index];

    processes[index] = processes[*notinserted_counter - 1];
    (*notinserted_counter)--;

    int process_index = holder.processes - state->processes;
    heap_insert(heap, state, process_index);
}

void check_arrivals_heap(SchedulerState *state, Processes_pointer *processes, int *notinserted_counter, MinHeap *heap){
    for(int i = 0; i < *notinserted_counter; i++){
        if(processes[i].processes->arrival_time <= state->current_time){
            inserting_process_heap(processes, notinserted_counter, heap, state, i);
            i--;
        }
    }
}