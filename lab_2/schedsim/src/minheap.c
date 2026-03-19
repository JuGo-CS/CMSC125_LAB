#include "./../include/process.h"
#include "./../include/scheduler.h"
#include "./../include/minheap.h"
#include "./../include/minheap_utils.h"


// Swap two integer slots in the heap array.
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Move the element at index up until heap property is restored.
// The heap is ordered by burst time (shortest first), with arrival time as a tiebreaker.
void heapify_up(MinHeap *heap, SchedulerState *state, int index){
    while(index > 0){
        int parent = (index - 1) / 2;

        int p1 = heap->data[parent];
        int p2 = heap->data[index];

<<<<<<< HEAD
        if(state->processes[p1]->burst_time <= state->processes[p2]->burst_time)
            break;
=======
        if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
          (state->processes[p1].burst_time == state->processes[p2].burst_time &&
           state->processes[p1].arrival_time <= state->processes[p2].arrival_time))
                break;
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b

        swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

// Push the value at index down until the heap property is restored.
void heapify_down(MinHeap *heap, SchedulerState *state, int index){
    while(1){
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if(left < heap->size){
            int p1 = heap->data[left];
            int p2 = heap->data[smallest];

<<<<<<< HEAD
            if(state->processes[p1]->burst_time < state->processes[p2]->burst_time)
                smallest = left;
=======
            if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
              (state->processes[p1].burst_time == state->processes[p2].burst_time &&
               state->processes[p1].arrival_time < state->processes[p2].arrival_time))
                    smallest = left;
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
        }

        if(right < heap->size){
            int p1 = heap->data[right];
            int p2 = heap->data[smallest];

<<<<<<< HEAD
            if(state->processes[p1]->burst_time < state->processes[p2]->burst_time)
                smallest = right;
=======
            if(state->processes[p1].burst_time < state->processes[p2].burst_time ||
              (state->processes[p1].burst_time == state->processes[p2].burst_time &&
               state->processes[p1].arrival_time < state->processes[p2].arrival_time))
                    smallest = right;
>>>>>>> 38d9a33979a24933dc539f0a5362cdb326f4282b
        }

        if(smallest == index)
            break;

        swap(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

// Insert a process index into the heap (takes ownership of the index).
void heap_insert(MinHeap *heap, SchedulerState *state, int process_index){
    heap->data[heap->size] = process_index;
    heapify_up(heap, state, heap->size);
    heap->size++;
}

// Remove and return the index of the process with the smallest burst time.
int heap_extract_min(MinHeap *heap, SchedulerState *state){
    int result = heap->data[0];

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    heapify_down(heap, state, 0);

    return result;
}

