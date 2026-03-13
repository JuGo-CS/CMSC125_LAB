#ifndef MINHEAP_UTILS_H
#define MINHEAP_UTILS_H

#include "./../include/scheduler.h"

void check_arrivals_heap(SchedulerState *state, Processes_pointer *processes, int *processes_unqueued, MinHeap *heap);
void inserting_process_heap(Processes_pointer *processes, int *processes_unqueued, MinHeap *heap, SchedulerState *state, int index);

#endif