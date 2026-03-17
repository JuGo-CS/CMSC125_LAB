#include <stdlib.h>
#include <string.h>
#include "./../include/process.h"

Process* construct_process(char *pid, int arrival_time, int burst_time) {
    Process* process = (Process*) malloc(sizeof(Process));
    if (process) {
        memset(process, 0, sizeof(Process));
        strncpy(process->pid, pid, PID_LENGTH - 1);
        process->pid[PID_LENGTH - 1] = '\0';
        process->arrival_time = arrival_time;
        process->burst_time = burst_time;
    }
    return process;
}

void destruct_process(Process *process) {
    free(process);
}