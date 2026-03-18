#ifndef METRICS_H
#define METRICS_H

#include "./scheduler.h"

void calculate_metrics(SchedulerState *state, int n);
double calculate_average_turnaround(SchedulerState *state);
void print_metrics(SchedulerState *state);


#endif