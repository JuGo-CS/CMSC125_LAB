#include <stdio.h>
#include <stdlib.h>
#include "./../include/process.h"
#include "./../include/scheduler.h"

int schedule_fcfs(SchedulerState *state){

    for(int i = 0; i < state->num_processes; i++){
        // state->processes[i].waiting_time = state->current_time;
        state->processes[i].start_time = state->current_time;
        state->current_time += state->processes[i].burst_time;
        state->processes[i].finish_time = state->current_time;
    }

    return 0;
}