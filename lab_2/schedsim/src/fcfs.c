#include <stdio.h>
#include <stdlib.h>
#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/queue.h"

void initialize_process_for_queue(SchedulerState *state, Process_queued *processes){
    for(int i = 0; i < state->num_processes; i++){
        processes[i].processes = &state->processes[i];
        processes[i].is_queued = false;
    }
}

int schedule_fcfs(SchedulerState *state){

    Queue queue_process = {0};                                  // the queue itself for this algorithm 
    Process_queued processes[state->num_processes];       // to initialize all the process, it will be used for queueing the process logic
    int completed_process = 0;

    initialize_process_for_queue(state, processes);
    while(completed_process < state->num_processes){

        for(int i = 0; i < state->num_processes; i++){
            if(!processes[i].is_queued && processes[i].processes->arrival_time <= state->current_time){
                processes[i].is_queued = true;
                enqueue(&queue_process, processes[i].processes);
            }
        }

        if(queue_process.size == 0){
            state->current_time++;
            continue;
        }

        Process *p = dequeue(&queue_process);

        // if(queue_process.head_process->processes->arrival_time <= current_time){
        p->start_time = state->current_time;
        state->current_time += p->burst_time;
        p->finish_time = state->current_time;

        completed_process++;
        
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



