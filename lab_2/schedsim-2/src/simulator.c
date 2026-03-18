#include <stdio.h>
#include "../includes/simulator.h"
#include "../includes/objects/event.h"

void simulate_scheduler(SchedulerState* state, int (*scheduling_algorithm)(SchedulerState*)) {
    printf("Starting simulation with %d processes...\n", state->num_processes);
    scheduling_algorithm(state);
}