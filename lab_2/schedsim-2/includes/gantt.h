#ifndef GANTT_H
#define GANTT_H


#include "./objects/process.h"

typedef struct SchedulerState SchedulerState;

// How many time units correspond to one character in the chart.
// Mas malaki value, mas short ang gantt chart
// Mas maliit value, mas mahaba gantt
#define GANTT_SCALE 5
#define MAX_GANTT_ENTRIES 1024


typedef struct {
    char name[PID_LENGTH];
    int start;
    int end;
} GanttEntry;

void append_gantt_entry(SchedulerState* state, Process* process, int start_time, int run_time);
void print_gantt(SchedulerState*);

#endif