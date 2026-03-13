#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"
#include "./../include/utils.h"
#include <stdio.h>

int schedule_stcf(SchedulerState *state){

    MinHeap heap = {0};
    Processes_pointer processes[state->num_processes];

    int completed = 0;
    int processes_not_inserted = state->num_processes;

    initialize_processes_pointer(state, processes);

    while(completed < state->num_processes){

        check_arrivals_heap(state, processes, &processes_not_inserted, &heap);

        if(heap.size == 0){
            state->current_time++;
            continue;
        }

        int min_index = heap_extract_min(&heap, state);
        Process *p = &state->processes[min_index];

        if(p->start_time == -1){
            p->start_time = state->current_time;
        }

        // run process for 1 time unit
        p->remaining_time--;
        state->current_time++;

        check_arrivals_heap(state, processes, &processes_not_inserted, &heap);

        if(p->remaining_time == 0){
            p->finish_time = state->current_time;
            completed++;
        }
        else{
            heap_insert(&heap, state, min_index);
        }
    }

    return 0;
}


