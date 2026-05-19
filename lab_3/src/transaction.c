#include "transaction.h"
#include "bankdb.h"
#include "bank.h"
#include "timer.h"
#include <stdio.h>
#include "buffer_pool.h"

void* execute_transaction(void* arg) {
    Transaction* tx = (Transaction*)arg;
    char* operation_string = "";

    wait_until_tick(tx->start_tick);
    
    // Read global_tick under locksto prevent data race
    pthread_mutex_lock(&tick_lock);
    tx->actual_start = global_tick;
    pthread_mutex_unlock(&tick_lock);
    
    tx->status = TX_RUNNING;
    
    for (int i = 0; i < tx->num_ops; i++) {
        Operation* op = &tx->ops[i];
        
        // Read tick_before under lock to prevent data race
        pthread_mutex_lock(&tick_lock);
        int tick_before = global_tick;
        int current_op_tick = global_tick;
        pthread_mutex_unlock(&tick_lock);
        
        switch (op->type) {
            case OP_DEPOSIT:
                if (verbose_mode) printf("[Tick %d] T%d started: DEPOSIT %d centavos into Acc %d\n", 
                                         current_op_tick, tx->tx_id, op->amount_centavos, op->account_id);
                
                operation_string = "DEPOSIT";
                load_account(&shared_pool, op->account_id);
                deposit(op->account_id, op->amount_centavos);
                unload_account(&shared_pool, op->account_id);
            
                break;
                
            case OP_WITHDRAW:
                if (verbose_mode) printf("[Tick %d] T%d started: WITHDRAW %d centavos from Acc %d\n", 
                                         current_op_tick, tx->tx_id, op->amount_centavos, op->account_id);

                operation_string = "WITHDRAW";
                load_account(&shared_pool, op->account_id);
                
                if (!withdraw(op->account_id, op->amount_centavos)) {
                    unload_account(&shared_pool, op->account_id);
                    tx->status = TX_ABORTED;
                    // Read final tick under lock before setting actual_end
                    pthread_mutex_lock(&tick_lock);
                    tx->actual_end = global_tick;
                    pthread_mutex_unlock(&tick_lock);
                    if (verbose_mode) printf("[Tick %d] T%d aborted: insufficient funds for WITHDRAW\n", current_op_tick, tx->tx_id);
                    return NULL;
                }
                unload_account(&shared_pool, op->account_id);
                break;
                
            case OP_TRANSFER:
                if (verbose_mode) printf("[Tick %d] T%d started: TRANSFER %d from Acc %d to Acc %d\n", 
                                         current_op_tick, tx->tx_id, op->amount_centavos, op->account_id, op->target_account);

                operation_string = "TRANSFER";
                int first = op->account_id;
                int second = op->target_account;
                if (deadlock_prevention) {
                    first = (op->account_id < op->target_account) ? op->account_id : op->target_account;
                    second = (op->account_id < op->target_account) ? op->target_account : op->account_id;
                }

                load_account(&shared_pool, first);
                load_account(&shared_pool, second);

                if (!transfer(op->account_id, op->target_account, op->amount_centavos, tx->tx_id)) {
                    unload_account(&shared_pool, first);
                    unload_account(&shared_pool, second);
                    tx->status = TX_ABORTED;
                    // Read final tick under lock before setting actual_end
                    pthread_mutex_lock(&tick_lock);
                    tx->actual_end = global_tick;
                    pthread_mutex_unlock(&tick_lock);
                    if (verbose_mode) {
                        if (deadlock_prevention) {
                            printf("[Tick %d] T%d aborted: transfer failed from Acc %d to Acc %d\n", current_op_tick, tx->tx_id, op->account_id, op->target_account);
                        } else {
                            printf("[Tick %d] T%d aborted: deadlock detected while transferring from Acc %d to Acc %d\n", current_op_tick, tx->tx_id, op->account_id, op->target_account);
                        }
                    }
                    return NULL;
                }

                unload_account(&shared_pool, second);
                unload_account(&shared_pool, first);
                break;
                
            case OP_BALANCE: {
                load_account(&shared_pool, op->account_id);
                int balance = get_balance(op->account_id);
                if (verbose_mode) printf("[Tick %d] T%d: Account %d balance = PHP %d.%02d\n", 
                       current_op_tick, tx->tx_id, op->account_id, balance / 100, balance % 100);
                
                operation_string = "BALANCE";

                unload_account(&shared_pool, op->account_id);
                break;
            }
        }
        
        // Read current tick under lock to prevent data race
        pthread_mutex_lock(&tick_lock);
        int current_tick = global_tick;
        int next_tick = current_tick + 1;
        pthread_mutex_unlock(&tick_lock);
        
        tx->wait_ticks += (current_tick - tick_before);
        wait_until_tick(next_tick);
    }
    
    // Read final tick under lock before setting actual_end
    pthread_mutex_lock(&tick_lock);
    tx->actual_end = global_tick;
    pthread_mutex_unlock(&tick_lock);
    
    tx->status = TX_COMMITTED;
    printf("T%d completed: %s successful.\n", 
            tx->tx_id, operation_string);
    return NULL;
}