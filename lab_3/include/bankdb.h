#ifndef BANKDB_H
#define BANKDB_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define TICK_INTERVAL_MS 10 // Used by timer
#define BUFFER_POOL_SIZE 5
#define MAX_TRANSACTIONS 100

// Global configuration flags
extern bool verbose_mode;
extern bool deadlock_prevention;

typedef struct {
    int account_id;
    int balance_centavos;
    pthread_rwlock_t lock;
} Account;

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int num_accounts;
    pthread_mutex_t bank_lock;
} Bank;

extern Bank bank; // Global bank instance

typedef enum {
    OP_DEPOSIT,
    OP_WITHDRAW,
    OP_TRANSFER,
    OP_BALANCE,
} OpType;

typedef struct {
    OpType type;
    int account_id;
    int amount_centavos;
    int target_account;
} Operation;

typedef enum {
    TX_RUNNING,
    TX_COMMITTED,
    TX_ABORTED
} TxStatus;

typedef struct {
    int tx_id;
    Operation ops[256];
    int num_ops;
    int start_tick;
    pthread_t thread;
    
    int actual_start;
    int actual_end;
    int wait_ticks;
    
    TxStatus status;
} Transaction;

#endif 