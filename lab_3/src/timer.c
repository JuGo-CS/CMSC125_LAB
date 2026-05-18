#include "timer.h"
#include "bankdb.h"
#include <unistd.h>
#include <stdio.h>
#include <stdatomic.h>

volatile int global_tick = 0;
_Atomic bool simulation_running = true;
pthread_mutex_t tick_lock;
pthread_cond_t tick_changed;

void init_timer() {
    pthread_mutex_init(&tick_lock, NULL);
    pthread_cond_init(&tick_changed, NULL);
}

void* timer_thread(void* arg) {
    (void)arg;
    while (atomic_load(&simulation_running)) {
        // Sleep for tick interval (use tick_ms instead of hardcoded TICK_INTERVAL_MS)
        usleep(tick_ms * 1000);
        
        pthread_mutex_lock(&tick_lock);
        global_tick++;
        // Print tick inside the lock to prevent data race on global_tick
        printf("\nTick %d:\n", global_tick);
        pthread_cond_broadcast(&tick_changed);
        pthread_mutex_unlock(&tick_lock);
    }
    return NULL;
}

void wait_until_tick(int target_tick) {
    pthread_mutex_lock(&tick_lock);
    while (global_tick < target_tick) {
        pthread_cond_wait(&tick_changed, &tick_lock);
    }
    pthread_mutex_unlock(&tick_lock);
}