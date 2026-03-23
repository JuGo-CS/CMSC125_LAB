#include <stdlib.h>
#include <string.h>
#include "./../../includes/data-structures/process-queue-adt.h"
#include "./../../includes/data-structures/mlfq-process-queue.h"
#include "./../../includes/objects/process.h"

// Helper functions for queue element management
static MLFQQueueElement* construct_mlfq_element(Process* process) {
    MLFQQueueElement* element = malloc(sizeof(MLFQQueueElement));
    if (element) {
        element->process = process;
        element->next = NULL;
    }
    return element;
}

static void destruct_mlfq_element(MLFQQueueElement* element) {
    free(element);
}

/**
 * Enqueue process into appropriate priority level
 * Processes arrive at the highest priority (level 0)
 */
static void mlfq_enqueue(AbstractProcessQueue* self, Process* process) {
    MLFQProcessQueue* mq = (MLFQProcessQueue*) self;
    
    // New arrivals start at highest priority (level 0)
    if (process->priority == UNKNOWN_TIME) {
        process->priority = 0;
        process->time_in_queue = 0;
    }
    
    int level = process->priority;
    if (level >= mq->num_levels) level = mq->num_levels - 1;
    
    MLFQQueueElement* node = construct_mlfq_element(process);
    MLFQPriorityQueue* pq = &mq->level_queues[level];
    
    if (pq->tail == NULL) {
        pq->head = pq->tail = node;
    } else {
        pq->tail->next = node;
        pq->tail = node;
    }
    
    self->size++;
}

/**
 * Dequeue: Select process from highest priority non-empty queue
 */
static Process* mlfq_dequeue(AbstractProcessQueue* self) {
    MLFQProcessQueue* mq = (MLFQProcessQueue*) self;
    
    // Find highest priority non-empty queue
    for (int i = 0; i < mq->num_levels; i++) {
        MLFQPriorityQueue* pq = &mq->level_queues[i];
        
        if (pq->head != NULL) {
            MLFQQueueElement* temp = pq->head;
            Process* process = temp->process;
            
            pq->head = pq->head->next;
            if (pq->head == NULL) pq->tail = NULL;
            
            destruct_mlfq_element(temp);
            self->size--;
            return process;
        }
    }
    
    return NULL;  // All queues empty
}

/**
 * Remove process from its current priority level and move to new level
 */
static void mlfq_move_process(MLFQProcessQueue* mq, Process* process, int new_level) {
    if (process->priority == new_level) return;  // Already at target level
    
    int old_level = process->priority;
    if (old_level < 0 || old_level >= mq->num_levels) return;
    
    MLFQPriorityQueue* old_pq = &mq->level_queues[old_level];
    MLFQQueueElement* current = old_pq->head;
    MLFQQueueElement* prev = NULL;
    
    // Find and remove process from old queue
    while (current != NULL) {
        if (current->process == process) {
            if (prev == NULL) {
                old_pq->head = current->next;
            } else {
                prev->next = current->next;
            }
            
            if (current == old_pq->tail) {
                old_pq->tail = prev;
            }
            
            destruct_mlfq_element(current);
            mq->queue.size--;  // Decrement size because we removed an element
            break;
        }
        prev = current;
        current = current->next;
    }
    
    // Add to new level queue
    process->priority = new_level;
    process->time_in_queue = 0;  // Reset allotment tracking for new level
    
    MLFQQueueElement* node = construct_mlfq_element(process);
    if (!node) return;
    
    MLFQPriorityQueue* new_pq = &mq->level_queues[new_level];
    
    if (new_pq->tail == NULL) {
        new_pq->head = new_pq->tail = node;
    } else {
        new_pq->tail->next = node;
        new_pq->tail = node;
    }
    
    mq->queue.size++;  // Increment size because we added an element
}

void mlfq_promote_process(MLFQProcessQueue* mq, Process* process, int new_level) {
    mlfq_move_process(mq, process, new_level);
}

void mlfq_demote_process(MLFQProcessQueue* mq, Process* process, int new_level) {
    mlfq_move_process(mq, process, new_level);
}

/**
 * Move all processes to highest priority queue
 */
void mlfq_priority_boost(MLFQProcessQueue* mq) {
    // Collect all processes from all queues
    Process** all_processes = malloc(sizeof(Process*) * mq->queue.size);
    if (!all_processes) return;
    
    int count = 0;
    for (int i = 0; i < mq->num_levels; i++) {
        MLFQQueueElement* current = mq->level_queues[i].head;
        while (current != NULL) {
            all_processes[count++] = current->process;
            current = current->next;
        }
    }
    
    // Clear all level queues and free elements
    for (int i = 0; i < mq->num_levels; i++) {
        MLFQQueueElement* current = mq->level_queues[i].head;
        while (current != NULL) {
            MLFQQueueElement* temp = current;
            current = current->next;
            destruct_mlfq_element(temp);
        }
        mq->level_queues[i].head = NULL;
        mq->level_queues[i].tail = NULL;
    }
    
    // Re-enqueue all processes at highest priority
    for (int i = 0; i < count; i++) {
        all_processes[i]->priority = 0;
        all_processes[i]->time_in_queue = 0;
        
        MLFQQueueElement* node = construct_mlfq_element(all_processes[i]);
        if (!node) continue;
        
        MLFQPriorityQueue* pq = &mq->level_queues[0];
        if (pq->tail == NULL) {
            pq->head = pq->tail = node;
        } else {
            pq->tail->next = node;
            pq->tail = node;
        }
    }
    
    free(all_processes);
}

int mlfq_get_quantum(MLFQProcessQueue* mq, int level) {
    if (level < 0 || level >= mq->num_levels) return -1;
    return mq->config->levels[level].time_quantum;
}

int mlfq_get_allotment(MLFQProcessQueue* mq, int level) {
    if (level < 0 || level >= mq->num_levels) return -1;
    return mq->config->levels[level].allotment;
}

static void destruct_mlfq_process_queue(AbstractProcessQueue* self) {
    MLFQProcessQueue* mq = (MLFQProcessQueue*) self;
    
    for (int i = 0; i < mq->num_levels; i++) {
        MLFQQueueElement* current = mq->level_queues[i].head;
        while (current != NULL) {
            MLFQQueueElement* temp = current;
            current = current->next;
            destruct_mlfq_element(temp);
        }
    }
    
    free(mq);
}

MLFQProcessQueue* construct_mlfq_process_queue(MLFQConfig* config) {
    if (!config || config->num_levels <= 0) {
        return NULL;
    }
    
    MLFQProcessQueue* mq = malloc(sizeof(MLFQProcessQueue));
    if (!mq) return NULL;
    
    // Initialize abstract queue interface
    mq->queue.enqueue = mlfq_enqueue;
    mq->queue.dequeue = mlfq_dequeue;
    mq->queue.destruct_queue = destruct_mlfq_process_queue;
    mq->queue.size = 0;
    
    // Initialize MLFQ-specific fields
    mq->config = config;
    mq->num_levels = config->num_levels;
    mq->last_boost_time = 0;
    
    // Initialize all priority level queues
    for (int i = 0; i < mq->num_levels; i++) {
        mq->level_queues[i].head = NULL;
        mq->level_queues[i].tail = NULL;
    }
    
    return mq;
}
