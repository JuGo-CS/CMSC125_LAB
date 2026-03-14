/*
 * fcfs.c
 *
 * First-Come First-Served scheduler.
 * Runs each process in arrival order without preemption.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/queue_utils.h"
#include "./../include/utils.h"


// Run FCFS on the given scheduler state.
// - state: holds the list of processes and where to store the Gantt chart
// Returns 0 (unused return value, main ignores it).
int schedule_fcfs(SchedulerState *state){

    Queue queue = {0};                                          // the queue itself for this algorithm 
    Processes_pointer processes[state->num_processes];          // to initialize all the process, it will be used for queueing the process logic

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};

    int complete_counter = 0;
    int unqueued_counter = state->num_processes;

    initialize_processes_pointer(state, processes);

    while(complete_counter < state->num_processes){

        //will check every second/time_step if new process arrives
        check_arrivals(state->current_time, processes, &unqueued_counter, &queue);

        //if there's not current process running, will try to check if there are a process in the queue
        if(current_process == NULL){

            //if the queue is empty, simply simulate that the time is moving forward (increment the current time)
            if(queue.size == 0){
                state->current_time++;
                continue;
            }   

            current_process = dequeue(&queue);

            //setting the gantt properties and the start_time of the process 
            //      (if start time has a place value holder ~meaning it doesnt run yet)
            if(current_process->start_time == -1){
                strcpy(ganttentry.name, current_process->pid);
                ganttentry.start = state->current_time;
                current_process->start_time = state->current_time;
            }
        }

        //simulate the process running and the time moving forward
        current_process->remaining_time--;
        state->current_time++;

        //the process is done
        if(current_process->remaining_time == 0){
            current_process->finish_time = state->current_time;

            //needed for the gantt chart later
            ganttentry.end = state->current_time;
            state->gantt[state->gantt_size++] = ganttentry;
            ganttentry = (GanttEntry){0};

            current_process = NULL;
            complete_counter++;
        }
    }

    return 0;
}




