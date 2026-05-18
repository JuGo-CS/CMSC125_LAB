#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <stdbool.h>

#include "bankdb.h"
#include "parser.h"

// extern declarations for global variables
bool verbose_mode;
bool deadlock_prevention;
int tick_ms;
Bank bank; 
Transaction tsx[MAX_TRANSACTIONS] ; 

// function prototypes
int load_accounts(const char *filename);
int load_transactions(const char *filename);
OpType get_op_type(char *op_str);

void parse_command_line(int argc, char *argv[]) {
    // command line args buffers
    char *account_file = NULL;
    char *trace_file = NULL;
    char *deadlock_prevention_buffer = NULL;
    long tick_ms_buffer = 1000;
    bool verbose_flag = false;

    // getting command line args
    int c;
    while (1) {
        static struct option long_options[] = {
            {"accounts", required_argument, 0, 'a'},
            {"trace",    required_argument, 0, 't'},
            {"deadlock", required_argument, 0, 'd'},
            {"tick-ms",  required_argument, 0, 'm'},
            {"verbose",  no_argument,       0, 'v'},
            {0, 0, 0, 0}
       };
    
        int option_index = 0;
        c = getopt_long(argc, argv, "a:t:d:m:v", long_options, &option_index);
        if (c == -1) break;

        switch (c) {
            case 'a': {
                account_file = optarg;    
                break;
            }
            case 't': {
                trace_file = optarg;   
                break;
            }
            case 'd': {
                if (strcmp(optarg, "prevention") != 0 && strcmp(optarg, "detection") != 0) {
                    fprintf(stderr, "Error: Invalid --deadlock argument: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                deadlock_prevention_buffer = optarg; 
                break; 
            }
            case 'm': {
                errno = 0; // Reset errno before conversion
                char *endptr = '\0'; // Ensure endptr is null-terminated
                tick_ms_buffer = strtol(optarg, &endptr, 10);

                if ( endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "Error: Invalid --tick duration: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                if ( (errno == ERANGE && (tick_ms_buffer == LONG_MAX || tick_ms_buffer < 0)) || (tick_ms_buffer <= 0) ) {
                    fprintf(stderr, "Error: Invalid --tick duration (Out of long type range OR negative): %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                break;
            }
            case 'v': {
                verbose_flag = true; 
                break;
            }
            default:
                exit(EXIT_FAILURE);
        }       
    }

    // checks for required arguments (account and trace files)
    if (account_file == NULL || trace_file == NULL) {
        fprintf(stderr, "Error: --accounts and --trace are required arguments.\n");
        exit(EXIT_FAILURE);
    }

    // check if load accounts is valid
    if (load_accounts(account_file) != 0) {
        fprintf(stderr, "Failed to load accounts from: %s\n", account_file);
        exit(EXIT_FAILURE); 
    }

    // check if load transactions is valid
    if (load_transactions(trace_file) != 0) {
        fprintf(stderr, "Failed to load transactions from: %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    // print anything
    deadlock_prevention = deadlock_prevention_buffer;
    tick_ms = tick_ms_buffer;
    verbose_mode = verbose_flag;

}          

int load_accounts(const char *filename) {
    // file initialization and error handling
    char absolute_file_path[8 + strlen(filename) + 1];
    snprintf(absolute_file_path, sizeof(absolute_file_path), "./tests/%s", filename);
    FILE *file = fopen(absolute_file_path, "r");
    if (!file) {
        perror("Could not open account file");
        return -1;
    }

    // initialize bank accounts and metadata
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        bank.accounts[i].account_id = UNINITIALIZED_ID;        
        bank.accounts[i].balance_centavos = UNINITIALIZED_BALANCE;    
    }   

    // reading from file and parsing accounts
    char line[128];
    int id;
    long bal;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        if (sscanf(line, "%d %ld", &id, &bal) == 2) {
            if (id < 0 || id >= MAX_ACCOUNTS) {
                fprintf(stderr, "Error: Invalid account ID: %d\n", id);
                continue;
            }
            bank.accounts[id].account_id = id;
            bank.accounts[id].balance_centavos = bal;
            pthread_rwlock_init(&bank.accounts[id].lock, NULL);
        }
    }

    // close file and return success
    fclose(file);
    return 0;
}


int load_transactions(const char *filename) {
    // file initialization and error handling
    char absolute_file_path[8 + strlen(filename) + 1];
    snprintf(absolute_file_path, sizeof(absolute_file_path), "./tests/%s", filename);
    FILE *file = fopen(absolute_file_path, "r");
    if (!file) {
        perror("Could not open transaction file");
        return -1;
    }
    
    // initialize transactions and metadata
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        tsx[i].tx_id = UNINITIALIZED_ID;
        tsx[i].num_ops = 0;
    }


    // reading from file and parsing accounts
    char tx_label[10], op_str[20];   
    int acc_id, tick;               
    char line[256];
    char tx_count = -1;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // get required vars
        sscanf(line, "%s %d %s %d", tx_label, &tick, op_str, &acc_id);      
        if (sscanf(line, "%s %d %s %d", tx_label, &tick, op_str, &acc_id) == 4) {
            // find index with a tx_label or create new transaction if not found
            int tx_index = -1;
            for (int i = 0; i < MAX_TRANSACTIONS; i++) {
                if (tsx[i].tx_id == atoi(tx_label + 1)) {
                    tx_index = i;
                    break;
                }
            }
            if (tx_index == -1 && tx_count < MAX_TRANSACTIONS - 1) { 
                tx_index = ++tx_count;
                tsx[tx_index].tx_id = atoi(tx_label + 1);
                tsx[tx_index].start_tick = tick;
                tsx[tx_index].status = TX_RUNNING;
            }

            // add operation to transaction's operation list
            Operation *op = &tsx[tx_index].ops[tsx[tx_index].num_ops];
            op->type = get_op_type(op_str);
            op->account_id = acc_id;
            if (op->type == OP_TRANSFER) {
                sscanf(line, "%*s %*d %*s %*d %d %d", 
                       &op->target_account, &op->amount_centavos);
            } else if (op->type != OP_BALANCE) {
                sscanf(line, "%*s %*d %*s %*d %d", &op->amount_centavos);
            } else if (op->type == OP_BALANCE) {
                op->amount_centavos = 0; 
            } else {
                fprintf(stderr, "Error: Invalid operation type in line: %s\n", line);
                continue;
            }
            tsx[tx_index].num_ops++;
        }
    }
    
    // close file and return success
    fclose(file);
    return 0;
    
}

OpType get_op_type(char *op_str) {
    if (strcmp(op_str, "DEPOSIT") == 0)  return OP_DEPOSIT;
    if (strcmp(op_str, "WITHDRAW") == 0) return OP_WITHDRAW;
    if (strcmp(op_str, "TRANSFER") == 0) return OP_TRANSFER;
    if (strcmp(op_str, "BALANCE") == 0)  return OP_BALANCE;
    return OP_BALANCE; 
}

