#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

void enqueueing_process(Process_queued *processes, int *processes_unqueued, Queue *queue, int index);
void check_arrivals(SchedulerState *state, Process_queued *processes, int *processes_unqueued, Queue *queue);


#endif