#include "./../include/scheduler.h"
#include "./../include/process.h"
#include "./../include/utils.h"

void initialize_processes_pointer(SchedulerState *state, Processes_pointer *processes){
    for(int i = 0; i < state->num_processes; i++){
        processes[i].processes = &state->processes[i];
        // processes[i].is_queued = false;
    }
}