#include "./../include/scheduler.h"
#include "./../include/queue.h"

void enqueueing_process(Processes_pointer *processes, int *unqueued_counter, Queue *queue, int index) {

    Processes_pointer holder = processes[index];
    
    processes[index] = processes[*unqueued_counter - 1];
    *unqueued_counter -= 1;
    
    enqueue(queue, holder.processes);
}

void check_arrivals(int current_time, Processes_pointer *processes, int *unqueued_counter, Queue *queue) {
    for (int i = 0; i < *unqueued_counter; i++) {
        if (processes[i].processes->arrival_time <= current_time) {
            enqueueing_process(processes, unqueued_counter, queue, i);
            i--; 
        }
    }
}