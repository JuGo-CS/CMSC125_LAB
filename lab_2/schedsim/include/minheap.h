#ifndef MINHEAP_H
#define MINHEAP_H

#include "./../include/scheduler.h"

typedef struct {
    int data[MAX_PROCESSES];
    int size;
} MinHeap;

void heap_insert(MinHeap *heap, SchedulerState *state, int process_index);
int heap_extract_min(MinHeap *heap, SchedulerState *state);

#endif