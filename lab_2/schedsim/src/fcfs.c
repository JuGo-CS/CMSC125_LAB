#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/queue_utils.h"
#include "./../include/utils.h"



int schedule_fcfs(SchedulerState *state){

    Queue queue = {0};                                          // the queue itself for this algorithm 
    Processes_pointer processes[state->num_processes];          // to initialize all the process, it will be used for queueing the process logic

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};

    int complete_counter = 0;
    int unqueued_counter = state->num_processes;

    initialize_processes_pointer(state, processes);

    while(complete_counter < state->num_processes){

        check_arrivals(state->current_time, processes, &unqueued_counter, &queue);

        if(current_process == NULL){
            if(queue.size == 0){
                state->current_time++;
                continue;
            }
            
            current_process = dequeue(&queue);

            if(current_process->start_time == -1){
                strcpy(ganttentry.name, current_process->pid);
                ganttentry.start = state->current_time;
                current_process->start_time = state->current_time;
            }
        }

        current_process->remaining_time--;
        state->current_time++;

        if(current_process->remaining_time == 0){
            current_process->finish_time = state->current_time;

            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};

            current_process = NULL;
            complete_counter++;
        }
    }

    return 0;
}




