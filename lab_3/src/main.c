// src/main.c
#include "bankdb.h"
#include "bank.h"
#include "timer.h"
#include "parser.h"
#include "transaction.h"
#include "buffer_pool.h"
#include "metrics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

long long calculate_total_balance() {
    long long total = bank.reserve_balance_centavos;
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        total += get_balance(i);
    }
    return total;
}


int main(int argc, char* argv[]) {
    parse_command_line(argc, argv);
    init_bank(MAX_ACCOUNTS);
    init_timer();
    init_buffer_pool(&shared_pool);

    printf("=== Banking System Execution Log ===\n");
    printf("Timer thread started (tick interval: %ld)\n", tick_ms);

    long long initial_total = calculate_total_balance();
    printf("Initial total: PHP %lld.%02lld\n\n", initial_total / 100, initial_total % 100);

    pthread_t timer_tid;
    pthread_create(&timer_tid, NULL, timer_thread, NULL);

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        pthread_create(&tsx[i].thread, NULL, execute_transaction, &tsx[i]);
    }

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        pthread_join(tsx[i].thread, NULL);
    }

    // Use atomic_store to safely write to simulation_running
    atomic_store(&simulation_running, false);
    pthread_join(timer_tid, NULL);
    
    // print metrics
    print_performance_metrics(tsx, global_tick);
    print_summary(tsx, global_tick);
    print_buffer_pool_report(&shared_pool);


    // conversation check
    long long final_total = calculate_total_balance();
    printf("\n=== Conversation Report ===\n");
    printf("Final total:   PHP %lld.%02lld\n", final_total / 100, final_total % 100);
    
    if (initial_total == final_total) {
        printf("Conservation check: PASSED\n");
    } else {
        printf("Conservation check: FAILED\n");
    }
    return 0;
}