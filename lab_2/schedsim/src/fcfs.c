#include <stdio.h>
#include <stdlib.h>
#include "./../include/scheduler.h"
#include "./../include/queue.h"
#include "./../include/queue_utils.h"



int schedule_fcfs(SchedulerState *state){

    Queue queue = {0};                              // the queue itself for this algorithm 
    Process_queued processes[state->num_processes];         // to initialize all the process, it will be used for queueing the process logic

    Process *current_process = NULL;

    int complete_counter = 0;
    int processes_unqueued = state->num_processes;

    initialize_process_for_queue(state, processes);

    while(complete_counter < state->num_processes){

        check_arrivals(state, processes, &processes_unqueued, &queue);

        if(current_process == NULL){
            if(queue.size == 0){
                state->current_time++;
                continue;
            }
            
            current_process = dequeue(&queue);

            if(current_process->start_time == -1){
                current_process->start_time = state->current_time;
            }
        }

        current_process->remaining_time--;
        state->current_time++;

        if(current_process->remaining_time == 0){
            current_process->finish_time = state->current_time;
            current_process = NULL;
            complete_counter++;
        }
    }

    return 0;
}

// int schedule_fcfs(SchedulerState *state){
//     int completed = 0;
//     while(completed < state->num_processes){

//         for(int i = 0; i < state->num_processes; i++){
//             Process *p = &state->processes[i];
//             if(p->arrival_time <= state->current_time && p->remaining_time > 0){
//                 p->start_time = state->current_time;
//                 state->current_time += p->burst_time;
//                 p->finish_time = state->current_time;
//                 p->remaining_time = 0;

//                 completed++;
//             }
//         }
//     }
// }



