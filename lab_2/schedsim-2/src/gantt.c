#include <stdio.h>
#include <string.h>
#include "./../includes/scheduler.h"
#include "./../includes/gantt.h"


void append_gantt_entry(SchedulerState* state, Process* process, int start_time, int run_time) {
    if (run_time <= 0 || state->gantt_size >= MAX_GANTT_ENTRIES || !process) {
        return;
    }

    GanttEntry entry = {0};
    strncpy(entry.name, process->pid, sizeof(entry.name) - 1);
    entry.start = start_time;
    entry.end = start_time + run_time;

    state->gantt[state->gantt_size++] = entry;
}


void print_gantt(SchedulerState *state) {
    if (state->gantt_size == 0) {
        printf("No gantt entries to display.\n");
        return;
    }

    printf("\n=== Gantt Chart ===\n");

    for (int i = 0; i < state->gantt_size; i++) {
        GanttEntry *e = &state->gantt[i];
        int duration = e->end - e->start;
        int chars = duration / GANTT_SCALE;
        if (chars < 1) chars = 1;

        int name_len = strlen(e->name);
        int dashes = chars - name_len;
        if (dashes < 0) dashes = 0;

        printf("[");
        printf("%s", e->name);
        for (int j = 0; j < dashes; j++) printf("-");
        printf("]");
    }
    printf("\n");

    for (int i = 0; i < state->gantt_size; i++) {
        GanttEntry *e = &state->gantt[i];
        int duration = e->end - e->start;
        int chars = duration / GANTT_SCALE;
        if (chars < 1) chars = 1;

        int name_len = strlen(e->name);
        int dashes = chars - name_len;
        if (dashes < 0) dashes = 0;

        int block_width = 1 + name_len + dashes + 1;
        printf("%-*d", block_width, e->start);
    }

    printf("%d", state->gantt[state->gantt_size - 1].end);
}


