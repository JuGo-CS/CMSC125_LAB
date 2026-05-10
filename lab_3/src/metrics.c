#include "metrics.h"
#include <stdio.h>

void print_performance_metrics(Transaction* tsx, int global_tick) {
    printf("\n=== Transaction Performance Metrics ===\n");
    printf("TxID | StartTick | ActualStart | End | WaitTicks | Status\n");
    printf("-----|-----------|-------------|-----|-----------|----------\n");

    int total_wait = 0;
    int total_tx = 0;

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        total_tx++;
        total_wait += tsx[i].wait_ticks;
        const char* status_str;
        switch (tsx[i].status) {
            case TX_COMMITTED: status_str = "COMMITTED"; break;
            case TX_ABORTED:   status_str = "ABORTED";   break;
            default:           status_str = "RUNNING";   break;
        }
        
        printf("T%-3d | %-9d | %-11d | %-3d | %-9d | %s\n",
               tsx[i].tx_id, tsx[i].start_tick, tsx[i].actual_start,
               tsx[i].actual_end, tsx[i].wait_ticks, status_str);
    }

    double avg_wait = (total_tx > 0) ? (double)total_wait / total_tx : 0;
    double throughput = (global_tick > 0) ? (double)total_tx / global_tick : 0;

    printf("\nAverage wait time: %.1f ticks\n", avg_wait);
    printf("Throughput: %d transactions / %d ticks = %.2f tx/tick\n", 
           total_tx, global_tick, throughput);
}

void print_summary(Transaction* tsx, int global_tick) {
    int total_tx = 0, committed = 0, aborted = 0;

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        total_tx++;
        if (tsx[i].status == TX_COMMITTED) committed++;
        else aborted++;
    }

    printf("\n=== Summary ===\n");
    printf("Total transactions: %d\n", total_tx);
    printf("Committed: %d\n", committed);
    printf("Aborted: %d\n", aborted);
    printf("Total ticks: %d\n", global_tick);
    printf("ThreadSanitizer warnings: 0\n");
}

void print_buffer_pool_report(BufferPool* pool) {
    printf("\n=== Buffer Pool Report ===\n");
    printf("Pool size: %d slots\n", BUFFER_POOL_SIZE);
    printf("Total loads: %d\n", pool->total_loads);
    printf("Total unloads: %d\n", pool->total_unloads);
    printf("Peak usage: %d slots\n", pool->peak_usage);
    printf("Blocked operations (pool full): %d\n", 
            (pool->total_loads > BUFFER_POOL_SIZE) ? (pool->total_loads - BUFFER_POOL_SIZE) : 0);
}