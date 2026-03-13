#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/queue.h"

// void initialize_rr(SchedulerState *state){
//     for(int i = 0; i < state->num_processes; i++){
//         state->processes[i].remaining_time = state->processes[i].burst_time;
//         // state->processes[i]->last_update = 0;
//         // state->processes[i]->waiting_time = 0;
//     }
// }

int schedule_rr(SchedulerState *state, int quantum_time){
    Queue queue = {0};
    Process_queued processes[state->num_processes];
    int complete_counter = 0;
    int processes_unqueued = state->num_processes;

    initialize_process_for_queue(state, processes);
    while(complete_counter < state->num_processes){

        for(int i = 0; i < processes_unqueued; i++){
            if(processes[i].processes->arrival_time <= state->current_time){
                printf("At time stamp: %d, Procees PID enqueue: %s\n\n",state->current_time, processes[i].processes->pid);
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
        if(p->start_time == -1){
            p->start_time = state->current_time;
        }

        if(p->remaining_time <= quantum_time){
            state->current_time += p->remaining_time;
            p->finish_time = state->current_time;
            p->remaining_time = 0;

            complete_counter++;
        } else {
            state->current_time += quantum_time;
            p->remaining_time -= quantum_time;

            enqueue(&queue, p);
        }

    }


}

// int schedule_rr(SchedulerState *state, int quantum_time){
//     Process *p;
//     int processes_done = 0;
//     bool is_process_updated;

//     // printf("Quantum Time: %d\n", quantum_time);

//     while(processes_done < state->num_processes){
//         // printf("%d\n", state->current_time);

//         is_process_updated = false;

//         for(int i = 0; i < state->num_processes; i++){
//             p = &state->processes[i];
//             // printf("%s, remaining time: %d\n", p->pid, p->remaining_time);

//             if(p->remaining_time && state->current_time >= p->arrival_time){
//                 if(p->start_time == -1){ 
//                     p->start_time = state->current_time;
//                 }
                
//                 if(p->remaining_time <= quantum_time){
//                     state->current_time += p->remaining_time;
//                     p->remaining_time = 0;
//                     p->finish_time = state->current_time;
//                     processes_done++;

//                     is_process_updated = true;
//                 } 
                
//                 else {
//                     state->current_time += quantum_time;
//                     p->remaining_time -= quantum_time;

//                     is_process_updated = true;
//                 }
//             }
//         }

//         if(!is_process_updated) {
//             state->current_time++;
//         }
//     }
        


//     return 0;
// }