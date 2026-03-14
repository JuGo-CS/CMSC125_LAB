#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"
#include "./../include/utils.h"
#include <stdio.h>
#include <string.h>

// Run STCF on the state (preemptive shortest remaining burst first).
int schedule_stcf(SchedulerState *state){

    MinHeap heap = {0};
    Processes_pointer processes[state->num_processes];
    GanttEntry ganttentry = {0};

    int completed = 0;
    int notinserted_counter = state->num_processes;
    char current_pid[16] = "x_pid_holder";

    initialize_processes_pointer(state, processes);

    // Keep going until every process is done.
    while(completed < state->num_processes){

        // Add newly arrived processes into the heap.
        check_arrivals_heap(state, processes, &notinserted_counter, &heap);

        if(heap.size == 0){
            // nothing ready yet
            state->current_time++;
            continue;
        }

        // Pick the process with the smallest remaining time.
        int min_index = heap_extract_min(&heap, state);
        Process *p = &state->processes[min_index];

        if(p->start_time == -1){
            p->start_time = state->current_time;
        }

        // If we switched processes, close the previous gantt entry and start a new one.
        if(strcmp(p->pid, current_pid) != 0){
            if(strcmp(current_pid, "x_pid_holder") != 0){
                ganttentry.end = state->current_time;
                state->gantt[state->gantt_size++] = ganttentry;
            }
            strcpy(ganttentry.name, p->pid);
            ganttentry.start = state->current_time;
            strcpy(current_pid, p->pid);
        }

        // run process for 1 time unit
        p->remaining_time--;
        state->current_time++;

        // New arrivals might come in during this time unit.
        check_arrivals_heap(state, processes, &notinserted_counter, &heap);

        if(p->remaining_time == 0){
            p->finish_time = state->current_time;

            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};
            strcpy(current_pid, "x_pid_holder");

            completed++;
        }
        else{
            heap_insert(&heap, state, min_index);
        }
    }

    return 0;
}


