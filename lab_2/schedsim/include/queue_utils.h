#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

void enqueueing_process(Processes_pointer *processes, int *unqueued_counter, Queue *queue, int index);
void check_arrivals(int current_time, Processes_pointer *processes, int *unqueued_counter, Queue *queue);

#endif