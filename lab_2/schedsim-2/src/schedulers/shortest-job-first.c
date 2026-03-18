#include <stdio.h>
#include <stdlib.h>
#include "./../includes/scheduler.h"

int schedule_sjf(SchedulerState *state){
    state->running->start_time = state->current_time;
    state->running->finish_time = state->running->start_time + state->running->burst_time;
    state->running->response_time = state->running->start_time - state->running->arrival_time;
    state->running->waiting_time = state->running->start_time - state->running->arrival_time;
    state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
    return 0;
}




