#include "transaction.h"
#include "bankdb.h"
#include "bank.h"
#include "timer.h"
#include <stdio.h>
#include "buffer_pool.h"

void* execute_transaction(void* arg) {
    Transaction* tx = (Transaction*)arg;
    
    wait_until_tick(tx->start_tick);
    tx->actual_start = global_tick;
    tx->status = TX_RUNNING;
    
    for (int i = 0; i < tx->num_ops; i++) {
        Operation* op = &tx->ops[i];
        int tick_before = global_tick;
        
        switch (op->type) {
            case OP_DEPOSIT:
                load_account(&shared_pool, op->account_id);
                deposit(op->account_id, op->amount_centavos);
                unload_account(&shared_pool, op->account_id);
                break;
                
            case OP_WITHDRAW:
                load_account(&shared_pool, op->account_id);
                if (!withdraw(op->account_id, op->amount_centavos)) {
                    unload_account(&shared_pool, op->account_id);
                    tx->status = TX_ABORTED;
                    return NULL;
                }
                unload_account(&shared_pool, op->account_id);
                break;
                
            case OP_TRANSFER:
                // Buffer Pool Deadlock Prevention(consistent order)
                int first = (op->account_id < op->target_account) ? op->account_id : op->target_account;
                int second = (op->account_id < op->target_account) ? op->target_account : op->account_id;

                load_account(&shared_pool, first);
                load_account(&shared_pool, second);

                if (!transfer(op->account_id, op->target_account, op->amount_centavos, tx->tx_id)) {
                    unload_account(&shared_pool, first);
                    unload_account(&shared_pool, second);
                    tx->status = TX_ABORTED;
                    return NULL;
                }

                unload_account(&shared_pool, second);
                unload_account(&shared_pool, first);
                break;
                
            case OP_BALANCE: {
                load_account(&shared_pool, op->account_id);
                int balance = get_balance(op->account_id);
                printf("T%d: Account %d balance = PHP %d.%02d\n", tx->tx_id, op->account_id, balance / 100, balance % 100);
                unload_account(&shared_pool, op->account_id);
                break;
            }
        }
        tx->wait_ticks += (global_tick - tick_before);
    }
    
    tx->actual_end = global_tick;
    tx->status = TX_COMMITTED;
    return NULL;
}