#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../includes/command.h"
#include "./../includes/scheduler.h"
#include "./../includes/data-structures/fcfs-process-queue.h"
#include "./../includes/data-structures/process-queue-adt.h"

int main(int argc, char *argv[]) {
    CommandLineArguments* args = parse_command_line(argc, argv);   
    SchedulerState state = {0};

     if (args->input_file) {
        get_processes_from_file(&state, args->input_file);
    } else if (args->process_str) {
        get_processes_from_string(&state, args->process_str);
    } else {
        fprintf(stderr, "No input provided!\n");
        return 1;
    } 

    // create queue
    FCFSProcessQueue* fq = construct_fcfs_process_queue();
    for (int i = 0; i < state.num_processes; i++) {
        fq->queue.enqueue(
            (AbstractProcessQueue*) fq,
            construct_process(state.processes[i]->pid, state.processes[i]->arrival_time, state.processes[i]->burst_time)
        );
    }
    
    FCFSQueueElement* curr = fq->head;
    while (curr != NULL) {  
        printf("Process %s: Arrival=%d Burst=%d\n", curr->process->pid,  curr->process->arrival_time, curr->process->burst_time);
        curr = curr->next;
    }
    printf("\n");

    destruct_fcfs_process_queue(fq);
    free_scheduler_processes(&state);
    free(args);

    return 0;
}



