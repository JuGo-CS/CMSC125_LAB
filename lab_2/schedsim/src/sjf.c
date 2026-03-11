#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include <stdio.h>

int schedule_sjf(SchedulerState *state){

    MinHeap heap = {0};
    int completed = 0;
    int next_process = 0;

    state->current_time = 0;

    while(completed < state->num_processes){

        while(next_process < state->num_processes &&
              state->processes[next_process].arrival_time <= state->current_time){

            heap_insert(&heap, state, next_process++);
        }

        if(heap.size == 0){
            state->current_time++;
            continue;
        }

        int min_index = heap_extract_min(&heap, state);
        Process *p = &state->processes[min_index];

        if(p->start_time == -1){
            p->start_time = state->current_time;
        }

        state->current_time += p->burst_time;

        p->finish_time = state->current_time;
        p->remaining_time = 0;

        completed++;
    }

    return 0;
}