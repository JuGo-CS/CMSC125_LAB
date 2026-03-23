/**
 * MLFQ Process Queue Header
 * 
 * Implements a multi-level feedback queue with multiple priority levels.
 * Each level maintains its own queue and time quantum.
 */

#ifndef MLFQ_PROCESS_QUEUE_H
#define MLFQ_PROCESS_QUEUE_H

#include "./process-queue-adt.h"
#include "./../mlfq-config.h"

#define MAX_MLFQ_LEVELS 10

typedef struct MLFQQueueElement {
    Process* process;
    struct MLFQQueueElement* next;
} MLFQQueueElement;

typedef struct {
    MLFQQueueElement* head;
    MLFQQueueElement* tail;
} MLFQPriorityQueue;

typedef struct {
    AbstractProcessQueue queue;
    MLFQPriorityQueue level_queues[MAX_MLFQ_LEVELS];  // One queue per priority level
    MLFQConfig* config;                               // Configuration with levels and boost period
    int num_levels;
    int last_boost_time;                              // Track last priority boost time
} MLFQProcessQueue;

/**
 * Construct MLFQ process queue with given configuration
 */
MLFQProcessQueue* construct_mlfq_process_queue(MLFQConfig* config);

/**
 * Move a process to a different priority level
 */
void mlfq_promote_process(MLFQProcessQueue* mq, Process* process, int new_level);
void mlfq_demote_process(MLFQProcessQueue* mq, Process* process, int new_level);

/**
 * Perform priority boost: move all processes to highest priority
 */
void mlfq_priority_boost(MLFQProcessQueue* mq);

/**
 * Get the time quantum for a given priority level
 */
int mlfq_get_quantum(MLFQProcessQueue* mq, int level);

/**
 * Get the allotment for a given priority level
 */
int mlfq_get_allotment(MLFQProcessQueue* mq, int level);

#endif
