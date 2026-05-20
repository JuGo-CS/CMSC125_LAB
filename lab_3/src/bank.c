#include "bank.h"
#include "bankdb.h"
#include <stdatomic.h>

void init_bank(int num_accounts) {
    bank.num_accounts = num_accounts;
    atomic_store(&bank.reserve_balance_centavos, 0);
    pthread_mutex_init(&bank.bank_lock, NULL);
    for (int i = 0; i < num_accounts; i++) {
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

    atomic_fetch_sub(&bank.reserve_balance_centavos, (long long)amount_centavos);
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

    atomic_fetch_add(&bank.reserve_balance_centavos, (long long)amount_centavos);
    return true;
}

bool transfer(int from_id, int to_id, int amount_centavos) {
    if (from_id == to_id) {
        return true;
    }
    
    // PREVENTION: Acquire locks in ascending order of account ID to prevent
    // lock-order-inversion. This ensures all transactions follow the same
    // global lock order, breaking the circular-wait condition of deadlock.
    int first_id = (from_id < to_id) ? from_id : to_id;
    int second_id = (from_id < to_id) ? to_id : from_id;
    
    Account* acc_first = &bank.accounts[first_id];
    Account* acc_second = &bank.accounts[second_id];
    
    pthread_rwlock_wrlock(&acc_first->lock);
    pthread_rwlock_wrlock(&acc_second->lock);
    
    if (bank.accounts[from_id].balance_centavos < amount_centavos) {
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