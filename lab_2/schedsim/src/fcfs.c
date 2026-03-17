#include <stdio.h>
#include <stdlib.h>
#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/queue.h"


int schedule_fcfs(SchedulerState *state){
    Queue queue = {0};                                      // the queue itself for this algorithm 
    Process_queued processes[state->num_processes];         // to initialize all the process, it will be used for queueing the process logic
    int complete_counter = 0;
    int processes_unqueued = state->num_processes;

    initialize_process_for_queue(state, processes);
    while(complete_counter < state->num_processes){

        for(int i = 0; i < processes_unqueued; i++){
            if(processes[i].processes->arrival_time <= state->current_time){
                // processes[i].is_queued = true;

                Process_queued holder = processes[i];
                processes[i--] = processes[--processes_unqueued];

                enqueue(&queue, holder.processes);
            }
        }

        if(queue.size == 0){
            state->current_time++;
            continue;
        }

        Process *p = dequeue(&queue);

        // if(queue.head_process->processes->arrival_time <= current_time){
        p->start_time = state->current_time;
        state->current_time += p->burst_time;
        p->finish_time = state->current_time;

        complete_counter++;
        
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



