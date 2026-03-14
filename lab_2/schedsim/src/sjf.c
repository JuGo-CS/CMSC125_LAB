#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"
#include "./../include/utils.h"
#include <stdio.h>
#include <string.h>

int schedule_sjf(SchedulerState *state){

    MinHeap heap = {0};
    Processes_pointer processes[state->num_processes];

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};

    int completed_process = 0;
    int notinserted_counter = state->num_processes;

    initialize_processes_pointer(state, processes);

    while(completed_process < state->num_processes){

        check_arrivals_heap(state, processes, &notinserted_counter, &heap);

        if(current_process == NULL){
            if(heap.size == 0){
                state->current_time++;
                continue;
            }

            int min_index = heap_extract_min(&heap, state);
            current_process = &state->processes[min_index];

            strcpy(ganttentry.name, current_process->pid);
            ganttentry.start = state->current_time;

            if(current_process->start_time == -1){
                current_process->start_time = state->current_time;
            }
        }

        current_process->remaining_time--;
        state->current_time++;

        if (current_process->remaining_time == 0) {
            current_process->finish_time = state->current_time;

            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};

            current_process = NULL;
            completed_process++;
        }
    }

    return 0;
}

