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
    // Must update reserve_balance under bank_lock to prevent data race.
    pthread_mutex_lock(&bank.bank_lock);
    bank.reserve_balance_centavos -= amount_centavos;
    pthread_mutex_unlock(&bank.bank_lock);
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

    // Must update reserve_balance under bank_lock to prevent data race.
    pthread_mutex_lock(&bank.bank_lock);
    bank.reserve_balance_centavos += amount_centavos;
    pthread_mutex_unlock(&bank.bank_lock);
    return true;
}

bool transfer(int from_id, int to_id, int amount_centavos, int tx_id) {
    if (from_id == to_id) {
        return true;
    }

    Account* acc_from = &bank.accounts[from_id];
    Account* acc_to = &bank.accounts[to_id];
    
    if (deadlock_prevention) {
        // PREVENTION: Acquire locks in ascending order of account ID to prevent
        // lock-order-inversion. This ensures all transactions follow the same
        // global lock order, breaking the circular-wait condition of deadlock.
        int first_id = (from_id < to_id) ? from_id : to_id;
        int second_id = (from_id < to_id) ? to_id : from_id;
        
        Account* acc_first = &bank.accounts[first_id];
        Account* acc_second = &bank.accounts[second_id];
        
        pthread_rwlock_wrlock(&acc_first->lock);
        pthread_rwlock_wrlock(&acc_second->lock);
        
        if (acc_from->balance_centavos < amount_centavos) {
            pthread_rwlock_unlock(&acc_second->lock);
            pthread_rwlock_unlock(&acc_first->lock);
            return false;
        }
        
        bank.accounts[from_id].balance_centavos -= amount_centavos;
        bank.accounts[to_id].balance_centavos += amount_centavos;
        
        pthread_rwlock_unlock(&acc_second->lock);
        pthread_rwlock_unlock(&acc_first->lock);
        return true;
        
    } else {
        // DETECTION: Try-lock with timeout heuristic. If second lock acquisition
        // fails (thread is already waiting), abort this transaction to break potential deadlock.
        pthread_rwlock_wrlock(&acc_from->lock);
        
        // Delay to allow the other transaction to lock its first lock
        usleep(1000);
        if (pthread_rwlock_trywrlock(&acc_to->lock) != 0) {
            // Failed to acquire second lock - possible deadlock detected
            pthread_rwlock_unlock(&acc_from->lock);
            return false;
        }
        
        if (acc_from->balance_centavos < amount_centavos) {
            pthread_rwlock_unlock(&acc_to->lock);
            pthread_rwlock_unlock(&acc_from->lock);
            return false;
        }
        
        bank.accounts[from_id].balance_centavos -= amount_centavos;
        bank.accounts[to_id].balance_centavos += amount_centavos;
        
        pthread_rwlock_unlock(&acc_to->lock);
        pthread_rwlock_unlock(&acc_from->lock);
        return true;
    }
}