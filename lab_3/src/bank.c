#include "bank.h"
#include "bankdb.h"
#include <stdio.h>
#include <unistd.h>

void init_bank(int num_accounts) {
    bank.num_accounts = num_accounts;
    bank.reserve_balance_centavos = 0;
    pthread_mutex_init(&bank.bank_lock, NULL);
    for (int i = 0; i < num_accounts; i++) {
        bank.accounts[i].account_id = i;
        bank.accounts[i].balance_centavos = 0;
        pthread_rwlock_init(&bank.accounts[i].lock, NULL);
    }
}

int get_balance(int account_id) {
    Account* acc = &bank.accounts[account_id];
    pthread_rwlock_rdlock(&acc->lock);
    int balance = acc->balance_centavos;
    pthread_rwlock_unlock(&acc->lock);
    return balance;
}

void deposit(int account_id, int amount_centavos) {
    Account* acc = &bank.accounts[account_id];
    pthread_rwlock_wrlock(&acc->lock);
    acc->balance_centavos += amount_centavos;
    pthread_rwlock_unlock(&acc->lock);

    // To preserve total system money, withdrawals and deposits move money
    // to/from the internal reserve balance instead of creating/destroying it.
    bank.reserve_balance_centavos -= amount_centavos;
}

bool withdraw(int account_id, int amount_centavos) {
    Account* acc = &bank.accounts[account_id];
    pthread_rwlock_wrlock(&acc->lock);
    if (acc->balance_centavos < amount_centavos) {
        pthread_rwlock_unlock(&acc->lock);
        return false;
    }
    acc->balance_centavos -= amount_centavos;
    pthread_rwlock_unlock(&acc->lock);

    bank.reserve_balance_centavos += amount_centavos;
    return true;
}

bool transfer(int from_id, int to_id, int amount_centavos, int tx_id) {
    if (from_id == to_id) {
        return true;
    }

    Account* acc_first = &bank.accounts[from_id];
    Account* acc_second = &bank.accounts[to_id];
    
    pthread_rwlock_wrlock(&acc_first->lock);
    if (deadlock_prevention) {
        pthread_rwlock_wrlock(&acc_second->lock);
    } else {
        // Delay to allow the other transaction to lock its first lock
        usleep(1000);
        if (pthread_rwlock_trywrlock(&acc_second->lock) != 0) {
            pthread_rwlock_unlock(&acc_first->lock);
            return false;
        }
    }
    
    Account* from_acc = &bank.accounts[from_id];
    if (from_acc->balance_centavos < amount_centavos) {
        pthread_rwlock_unlock(&acc_second->lock);
        pthread_rwlock_unlock(&acc_first->lock);
        return false;
    }
    
    bank.accounts[from_id].balance_centavos -= amount_centavos;
    bank.accounts[to_id].balance_centavos += amount_centavos;
    
    pthread_rwlock_unlock(&acc_second->lock);
    pthread_rwlock_unlock(&acc_first->lock);
    return true;
}