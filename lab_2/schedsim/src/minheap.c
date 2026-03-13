#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"


void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap *heap, SchedulerState *state, int index){
    while(index > 0){
        int parent = (index - 1) / 2;

        int p1 = heap->data[parent];
        int p2 = heap->data[index];

        if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
          (state->processes[p1].burst_time == state->processes[p2].burst_time &&
           state->processes[p1].arrival_time <= state->processes[p2].arrival_time))
                break;

        swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

void heapify_down(MinHeap *heap, SchedulerState *state, int index){
    while(1){
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if(left < heap->size){
            int p1 = heap->data[left];
            int p2 = heap->data[smallest];

            if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
              (state->processes[p1].burst_time == state->processes[p2].burst_time &&
               state->processes[p1].arrival_time < state->processes[p2].arrival_time))
                    smallest = left;
        }

        if(right < heap->size){
            int p1 = heap->data[right];
            int p2 = heap->data[smallest];

            if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
              (state->processes[p1].burst_time == state->processes[p2].burst_time &&
               state->processes[p1].arrival_time < state->processes[p2].arrival_time))
                    smallest = right;
        }

        if(smallest == index)
            break;

        swap(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

void heap_insert(MinHeap *heap, SchedulerState *state, int process_index){
    heap->data[heap->size] = process_index;
    heapify_up(heap, state, heap->size);
    heap->size++;
}

int heap_extract_min(MinHeap *heap, SchedulerState *state){
    int result = heap->data[0];

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    heapify_down(heap, state, 0);

    return result;
}

