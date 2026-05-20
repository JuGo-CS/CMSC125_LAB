#include "buffer_pool.h"
#include <stdio.h>

BufferPool shared_pool;

void init_buffer_pool(BufferPool* pool) {
    sem_init(&pool->empty_slots, 0, BUFFER_POOL_SIZE);
    pthread_mutex_init(&pool->pool_lock, NULL);
    
    pool->total_loads = 0;
    pool->total_unloads = 0;
    pool->current_usage = 0;
    pool->peak_usage = 0;
    
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        pool->slots[i].in_use = false;
    }
}

void load_account(BufferPool* pool, int account_id) {
    
    sem_wait(&pool->empty_slots);  // Block if pool is full
    pthread_mutex_lock(&pool->pool_lock);
    
    pool->total_loads++;
    pool->current_usage++;
    if (pool->current_usage > pool->peak_usage) {
        pool->peak_usage = pool->current_usage;
    }

    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        if (!pool->slots[i].in_use) {
            pool->slots[i].account_id = account_id;
            pool->slots[i].data = &bank.accounts[account_id];
            pool->slots[i].in_use = true;
            break;
        }
    }
    
    pthread_mutex_unlock(&pool->pool_lock);
}

void unload_account(BufferPool* pool, int account_id) {
    pthread_mutex_lock(&pool->pool_lock);
    
    pool->total_unloads++;
    pool->current_usage--;

    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        if (pool->slots[i].in_use && pool->slots[i].account_id == account_id) {
            pool->slots[i].in_use = false;
            break;
        }
    }
    
    pthread_mutex_unlock(&pool->pool_lock);
    sem_post(&pool->empty_slots);  // Signal that a slot is freed
}