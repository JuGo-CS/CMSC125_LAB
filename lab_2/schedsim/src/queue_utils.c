#include "./../include/scheduler.h"
#include "./../include/queue.h"

void enqueueing_process(Processes_pointer *processes, int *processes_unqueued, Queue *queue, int index) {

    Processes_pointer holder = processes[index];
    
    processes[index] = processes[*processes_unqueued - 1];
    *processes_unqueued -= 1;
    
    enqueue(queue, holder.processes);
}

void check_arrivals(SchedulerState *state, Processes_pointer *processes, int *processes_unqueued, Queue *queue) {
    for (int i = 0; i < *processes_unqueued; i++) {
        if (processes[i].processes->arrival_time <= state->current_time) {
            enqueueing_process(processes, processes_unqueued, queue, i);
            i--; 
        }
    }
}