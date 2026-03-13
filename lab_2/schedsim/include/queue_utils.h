#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

void enqueueing_process(Processes_pointer *processes, int *processes_unqueued, Queue *queue, int index);
void check_arrivals(SchedulerState *state, Processes_pointer *processes, int *processes_unqueued, Queue *queue);

#endif