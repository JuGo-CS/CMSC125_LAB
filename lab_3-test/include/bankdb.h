#ifndef BANKDB_H
#define BANKDB_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100
#define TRANSACTION_END -1
#define UNINITIALIZED_ID -1
#define UNINITIALIZED_BALANCE -1

// #define BUFFER_POOL_SIZE 5

// account struct
typedef struct {
    int account_id;
    int balance_centavos;
    pthread_rwlock_t lock;
} Account;

// bank struct
typedef struct {
    Account accounts[MAX_ACCOUNTS];
    long long reserve_balance_centavos;
    pthread_mutex_t bank_lock;  
} Bank;

// types for transaction operations and transaction status
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

// actual transaction structure
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

// Global configuration flags
extern bool verbose_mode;
extern bool deadlock_prevention;
extern int tick_ms;
extern Bank bank; 
extern Transaction tsx[MAX_TRANSACTIONS]; 
#endif 