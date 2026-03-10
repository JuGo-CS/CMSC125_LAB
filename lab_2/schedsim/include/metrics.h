#ifndef METRICS_H
#define METRICS_H

#include "./../include/scheduler.h"

void calculate_metrics(SchedulerState *state, int n);
double calculate_average_turnaround(SchedulerState *state, int n);
void print_metrics(SchedulerState *state);


#endif