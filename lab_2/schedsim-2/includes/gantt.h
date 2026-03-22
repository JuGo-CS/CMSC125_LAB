#ifndef GANTT_H
#define GANTT_H


// How many time units correspond to one character in the chart.
// Mas malaki value, mas short ang gantt chart
// Mas maliit value, mas mahaba gantt
#define GANTT_SCALE 30
#define MAX_GANTT_ENTRIES 1024

typedef struct {
    char name[PID_LENGTH];
    int start;
    int end;
} GanttEntry;

#endif