#ifndef TIMER_H
#define TIMER_H
#include <stdbool.h>

extern volatile int global_tick;
extern bool simulation_running;

void* timer_thread(void* arg);
void wait_until_tick(int target_tick);
void init_timer();

#endif 