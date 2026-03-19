#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/queue_utils.h"
#include "./../include/utils.h"


// Run RR scheduler using the given time quantum.
// - state: contains the processes and where results are recorded.
// - quantum_time: how many time units each process can run before being preempted.
int schedule_rr(SchedulerState *state, int quantum_time){
    Queue queue = {0};
    Processes_pointer processes[state->num_processes];

    Process *current_process = NULL;
    GanttEntry ganttentry = {0};
    int current_quantum_left = 0;

    int complete_counter = 0;
    int unqueued_counter = state->num_processes;
    

    initialize_processes_pointer(state, processes);

    // Keep going until all processes finish.
    while(complete_counter < state->num_processes){

        // Add any newly arrived processes into the ready queue.
        check_arrivals(state->current_time, processes, &unqueued_counter, &queue);

        // If no process is running, grab the next one (if any).
        if(current_process == NULL){

            if(queue.size == 0){
                // nothing to run right now, just bump simulation time
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

        // Run the current process for one time unit.
        state->current_time++;
        current_process->remaining_time--;
        current_quantum_left--;

        // Check if new processes arrived during this time unit.
        check_arrivals(state->current_time, processes, &unqueued_counter, &queue);

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
<<<<<<< HEAD
=======

>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
}

