#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/queue_utils.h"
#include "./../include/utils.h"


int schedule_rr(SchedulerState *state, int quantum_time){
    Queue queue = {0};
    Processes_pointer processes[state->num_processes];

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};
    int current_quantum_left = 0;

    int complete_counter = 0;
    int processes_unqueued = state->num_processes;
    

    initialize_processes_pointer(state, processes);

    while(complete_counter < state->num_processes){

        check_arrivals(state, processes, &processes_unqueued, &queue);

        if(current_process == NULL){

            if(queue.size == 0){
                state->current_time++;
                continue;
            }

            current_process = dequeue(&queue);
            current_quantum_left = quantum_time;

            strcpy(ganttentry.name, current_process->pid);
            ganttentry.start = state->current_time;

            if(current_process->start_time == -1){
                current_process->start_time = state->current_time;
            }
        }

        state->current_time++;
        current_process->remaining_time--;
        current_quantum_left--;

        check_arrivals(state, processes, &processes_unqueued, &queue);

        if(current_process->remaining_time == 0){
            current_process->finish_time = state->current_time;

            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};

            complete_counter++;
            current_process = NULL;
        } 
        
        else if(current_quantum_left == 0){
            enqueue(&queue, current_process);
            current_process = NULL;

            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};
        }

    }

    return 0;

}

