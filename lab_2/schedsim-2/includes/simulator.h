#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "./objects/process.h"
#include "./objects/event.h"
#include "./scheduler.h"

void simulate_scheduler(SchedulerState*, int (SchedulerState*));

#endif