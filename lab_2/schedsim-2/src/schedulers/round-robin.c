#include <stdio.h>
#include <stdlib.h>
#include "./../includes/scheduler.h"

int schedule_rr(SchedulerState *state){
    // Calculate how long this process can run in this quantum
    int run_time = 0;
    if(state->running->remaining_time <= state->quantum_time){
        run_time = state->running->remaining_time;
    } else {
        run_time = state->quantum_time;
    }
    
    // Set start time if this is first time running
    if(state->running->start_time == -1){
        state->running->start_time = state->current_time;
    }
    
    // If this quantum will complete the process, set finish_time
    if(run_time == state->running->remaining_time){
        state->running->finish_time = state->current_time + run_time;
    }
    
    // Return run_time so simulator knows how long it ran
    return run_time;
}
