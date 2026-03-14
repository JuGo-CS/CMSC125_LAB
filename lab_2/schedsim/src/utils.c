#include "./../include/scheduler.h"
#include "./../include/process.h"
#include "./../include/utils.h"

// Set up the array of pointers so other code can easily look up processes.
// - state: the scheduler state containing the process array
// - processes: output array that will hold pointers into state->processes
void initialize_processes_pointer(SchedulerState *state, Processes_pointer *processes){
    for(int i = 0; i < state->num_processes; i++){
        processes[i].processes = &state->processes[i];
        // processes[i].is_queued = false;
    }
}