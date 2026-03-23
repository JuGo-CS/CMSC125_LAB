#include <stdio.h>
#include <stdlib.h>
#include "./../includes/scheduler.h"
#include "./../includes/data-structures/event-queue.h"

extern EventQueue* event_queue;

int schedule_stcf(SchedulerState *state){
    if(state->running->start_time == - 1){
        state->running->start_time = state->current_time;
        state->running->response_time = state->current_time - state->running->arrival_time;
    } 

    Event* next_arrival_event = peek_next_arrival(event_queue);
    int time_until_arrival = 0;

    //get how long the process will run (it will be base on the next event~time)
    if (next_arrival_event != NULL && next_arrival_event->time > state->current_time) {
        time_until_arrival = next_arrival_event->time - state->current_time;
    } else {
        time_until_arrival = state->running->remaining_time;
    }

    int run_time = 0;
    if(state->running->remaining_time <= time_until_arrival){
        run_time = state->running->remaining_time;
    } else {
        run_time = time_until_arrival;
    }

    if (run_time == state->running->remaining_time) {
        state->running->finish_time = state->current_time + run_time;
        state->running->turnaround_time = state->running->finish_time - state->running->arrival_time;
        state->running->waiting_time = state->running->turnaround_time - state->running->burst_time;
    }

    return run_time;  

}

