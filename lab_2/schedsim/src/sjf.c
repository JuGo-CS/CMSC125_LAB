#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"
#include "./../include/utils.h"
#include <stdio.h>
#include <string.h>

// Run SJF on the provided state.
// The algorithm picks the shortest job that has arrived and runs it to completion.
int schedule_sjf(SchedulerState *state){

    MinHeap heap = {0};
    Processes_pointer processes[state->num_processes];

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};

    int completed_process = 0;
    int notinserted_counter = state->num_processes;

    initialize_processes_pointer(state, processes);

    // Run until all processes are finished.
    while(completed_process < state->num_processes){

<<<<<<< HEAD
        while(next_process < state->num_processes &&
              state->processes[next_process]->arrival_time <= state->current_time){
=======
        // Add any newly arrived processes into the heap so the scheduler can pick.
        check_arrivals_heap(state, processes, &notinserted_counter, &heap);
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b

        // If we don't have a current process, grab the shortest one from the heap.
        if(current_process == NULL){
            if(heap.size == 0){
                // nothing ready yet, advance time
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

        // Run current process for one time unit.
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
<<<<<<< HEAD

        int min_index = heap_extract_min(&heap, state);
        Process *p = state->processes[min_index];

        if(p->start_time == -1){
            p->start_time = state->current_time;
        }

        state->current_time += p->burst_time;

        p->finish_time = state->current_time;
        p->remaining_time = 0;

        completed_process++;
=======
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
    }

    return 0;
}

