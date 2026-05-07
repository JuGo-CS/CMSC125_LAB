// src/main.c
#include "bankdb.h"
#include "bank.h"
#include "timer.h"
#include "parser.h"
#include "transaction.h"
#include "buffer_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long calculate_total_balance() {
    long long total = 0;
    for (int i = 0; i < bank.num_accounts; i++) {
        total += get_balance(i);
    }
    return total;
}


int main(int argc, char* argv[]) {
       // init_bank(MAX_ACCOUNTS);
    init_timer();
    init_buffer_pool(&shared_pool);
    parse_command_line(argc, argv);
    print_system_state(bank, tsx);

    long long initial_total = calculate_total_balance();
    printf("Initial total: PHP %lld.%02lld\n", initial_total / 100, initial_total % 100);

    pthread_t timer_tid;
    pthread_create(&timer_tid, NULL, timer_thread, NULL);

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        pthread_create(&tsx[i].thread, NULL, execute_transaction, &tsx[i]);
    }

    for (int i = 0; tsx[i].tx_id != TRANSACTION_END; i++) {
        pthread_join(tsx[i].thread, NULL);
    }

    simulation_running = false;
    pthread_join(timer_tid, NULL);

    long long final_total = calculate_total_balance();
    printf("Final total:   PHP %lld.%02lld\n", final_total / 100, final_total % 100);
    
    if (initial_total == final_total) {
        printf("Conservation check: PASSED\n");
    } else {
        printf("Conservation check: FAILED\n");
    }

    return 0;
}