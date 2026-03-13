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

void enqueueing_process(Process_queued *processes, int *processes_unqueued, Queue *queue, int index) {

    Process_queued holder = processes[index];
    
    processes[index] = processes[*processes_unqueued - 1];
    *processes_unqueued -= 1;
    
    enqueue(queue, holder.processes);
}

void check_arrivals(SchedulerState *state, Process_queued *processes, int *processes_unqueued, Queue *queue) {
    for (int i = 0; i < *processes_unqueued; i++) {
        if (processes[i].processes->arrival_time <= state->current_time) {
            enqueueing_process(processes, processes_unqueued, queue, i);
            i--; 
        }
    }
}

int schedule_rr(SchedulerState *state, int quantum_time){
    Queue queue = {0};
    Process_queued processes[state->num_processes];

    Process *current_process = NULL;
    int current_quantum_left = 0;

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
            current_quantum_left = quantum_time;

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
            complete_counter++;

            current_process = NULL;
        } 
        
        else if(current_quantum_left == 0){
            enqueue(&queue, current_process);
            current_process = NULL;
        }

    }

    return 0;

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