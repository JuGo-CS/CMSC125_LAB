// src/main.c
#include "bankdb.h"
#include "bank.h"
#include "timer.h"
#include "transaction.h"
#include "buffer_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool verbose_mode = false;
bool deadlock_prevention = true; 

long long calculate_total_balance() {
    long long total = 0;
    for (int i = 0; i < bank.num_accounts; i++) {
        total += get_balance(i);
    }
    return total;
}

int main(int argc, char* argv[]) {

    //initial parser (to be fixed/better implementation soon)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0) {
            verbose_mode = true;
        }
        if (strcmp(argv[i], "--deadlock=prevention") == 0) {
            deadlock_prevention = true;
        }
    }

    init_timer();
    init_bank(MAX_ACCOUNTS);
    init_buffer_pool(&shared_pool);

    // for testing (initial way of checking how everything works, will be removed in final version)
    bank.accounts[10].balance_centavos = 10000; 
    bank.accounts[20].balance_centavos = 5000;
    
    long long initial_total = calculate_total_balance();
    printf("Initial total: PHP %lld.%02lld\n", initial_total / 100, initial_total % 100);



    //hardcoded transactions first for testing (magaaply palang ng tests files in the future)
    Transaction txs[2];
    
    txs[0].tx_id = 1;
    txs[0].start_tick = 0;
    txs[0].num_ops = 1;
    txs[0].ops[0].type = OP_TRANSFER;
    txs[0].ops[0].account_id = 10;
    txs[0].ops[0].target_account = 20;
    txs[0].ops[0].amount_centavos = 5000;

    txs[1].tx_id = 2;
    txs[1].start_tick = 0;
    txs[1].num_ops = 1;
    txs[1].ops[0].type = OP_TRANSFER;
    txs[1].ops[0].account_id = 20;
    txs[1].ops[0].target_account = 10;
    txs[1].ops[0].amount_centavos = 3000;

    pthread_t timer_tid;
    pthread_create(&timer_tid, NULL, timer_thread, NULL);

    int num_txs = 2;
    for (int i = 0; i < num_txs; i++) {
        pthread_create(&txs[i].thread, NULL, execute_transaction, &txs[i]);
    }

    for (int i = 0; i < num_txs; i++) {
        pthread_join(txs[i].thread, NULL);
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