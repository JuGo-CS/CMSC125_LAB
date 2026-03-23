/**
 * MLFQ Configuration Header
 * 
 * Defines the data structures for Multi-Level Feedback Queue configuration.
 * Configuration format:
 * 
 * # Queue configurations
 * Q0 10 50        # Queue ID, time quantum, allotment
 * Q1 30 150
 * Q2 -1 -1       # FCFS queue (no quantum or allotment)
 * 
 * # Priority boost specification
 * BOOST_PERIOD 200
 */

#ifndef MLFQ_CONFIG_H
#define MLFQ_CONFIG_H

#define MAX_MLFQ_LEVELS 10
#define MLFQ_CONFIG_LINE_MAX 256

typedef struct {
    int queue_id;           // 0, 1, 2, ...
    int time_quantum;       // Time quantum for this level (-1 for FCFS)
    int allotment;          // Max time before demotion (-1 for unlimited)
} MLFQLevel;

typedef struct {
    MLFQLevel levels[MAX_MLFQ_LEVELS];
    int num_levels;
    int boost_period;       // Period for priority boost (-1 if disabled)
} MLFQConfig;

/**
 * Load MLFQ configuration from file
 * Returns pointer to MLFQConfig on success, NULL on failure
 */
MLFQConfig* load_mlfq_config(const char* config_file);

/**
 * Free MLFQ configuration
 */
void free_mlfq_config(MLFQConfig* config);

/**
 * Validate MLFQ configuration
 */
int validate_mlfq_config(MLFQConfig* config);

#endif
