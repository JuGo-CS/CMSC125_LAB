/**
 * Comparative Analysis Module
 * Runs all scheduling algorithms on the same workload and compares results
 */

#ifndef COMPARE_H
#define COMPARE_H

#include "./scheduler.h"

typedef struct {
    char algorithm_name[32];
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    int context_switches;
} AlgorithmResult;

/**
 * Run all algorithms and collect results
 */
void run_comparison(SchedulerState base_state, char* input_description);

#endif
