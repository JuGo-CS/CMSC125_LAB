#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"
#include "./../include/utils.h"
#include <stdio.h>

int schedule_sjf(SchedulerState *state){

    MinHeap heap = {0};
    Processes_pointer processes[state->num_processes];
    Process *current_process = NULL;

    int completed_process = 0;
    int processes_not_inserted = state->num_processes;

    initialize_processes_pointer(state, processes);

    while(completed_process < state->num_processes){

        check_arrivals_heap(state, processes, &processes_not_inserted, &heap);

        if(current_process == NULL){
            if(heap.size == 0){
                state->current_time++;
                continue;
            }

            int min_index = heap_extract_min(&heap, state);
            current_process = &state->processes[min_index];

            if(current_process->start_time == -1){
                current_process->start_time = state->current_time;
            }
        }

        current_process->remaining_time--;
        state->current_time++;

        if (current_process->remaining_time == 0) {
            current_process->finish_time = state->current_time;
            current_process = NULL;
            completed_process++;
        }
    }

    return 0;
}

