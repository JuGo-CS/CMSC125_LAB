#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include "parser.h"
#include "bankdb.h"

bool verbose_mode;
bool deadlock_prevention;
long tick_ms;
Bank bank; // Global bank instance
Transaction tsx[MAX_TRANSACTIONS] ; // Global transaction list instance

int load_accounts(Bank *bank, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open balance file");
        return -1;
    }

    char line[128];
    int id;
    long bal;
   
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        if (sscanf(line, "%d %ld", &id, &bal) == 2) {
            if (id < 0 || id >= MAX_ACCOUNTS) {
                fprintf(stderr, "Invalid account ID: %d\n", id);
                continue;
            }
            bank->accounts[id].account_id = id;
            bank->accounts[id].balance_centavos = bal;
        }
    }
    
    fclose(file);
    return 0;
}
OpType get_op_type(char *op_str) {
    if (strcmp(op_str, "DEPOSIT") == 0)  return OP_DEPOSIT;
    if (strcmp(op_str, "WITHDRAW") == 0) return OP_WITHDRAW;
    if (strcmp(op_str, "TRANSFER") == 0) return OP_TRANSFER;
    if (strcmp(op_str, "BALANCE") == 0)  return OP_BALANCE;
    return OP_BALANCE; // Default
}

int load_transactions(Transaction *tx_list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Trace file error");
        return -1;
    }

    char line[256];
    int tx_count = 0;

    while (fgets(line, sizeof(line), file) && tx_count < MAX_TRANSACTIONS - 1) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char tx_label[10], op_str[20];
        int tick, acc_id;

        if (sscanf(line, "%s %d %s %d", tx_label, &tick, op_str, &acc_id) == 4) {
            // find index with a tx_label or create new transaction if not found
            int tx_index = -1;
            for (int i = 0; i < MAX_TRANSACTIONS; i++) {
                if (tx_list[i].tx_id == atoi(tx_label + 1)) {
                    tx_index = i;
                    break;
                }
            }
            if (tx_index == -1 && tx_count < MAX_TRANSACTIONS - 1) { 
                tx_index = tx_count++;
                tx_list[tx_index].tx_id = atoi(tx_label + 1);
                tx_list[tx_index].start_tick = tick;
                tx_list[tx_index].status = TX_RUNNING;
            }

            // add operation to transaction's operation list
            Operation *op = &tx_list[tx_index].ops[tx_list[tx_index].num_ops];
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
            tx_list[tx_index].num_ops++;
        }
    }

    tx_list[tx_count].tx_id = TRANSACTION_END;
    fclose(file);
    return 0;
}

void parse_command_line(int argc, char *argv[]) {
    // Initialize to NULL so we can check if they were actually set
    char *account_file = NULL;
    char *trace_file = NULL;
    
    // Initialize tick_ms with default value
    tick_ms = TICK_INTERVAL_MS;

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
            case 'a': account_file = optarg; break;
            case 't': trace_file = optarg;   break;
            case 'd':
                if (strcmp(optarg, "prevention") == 0) deadlock_prevention = true;
                else if (strcmp(optarg, "detection") == 0) deadlock_prevention = false;
                break;
            case 'm': {
                errno = 0; // Reset errno before conversion
                char *endptr = '\0'; // Ensure endptr is null-terminated
                tick_ms = strtol(optarg, &endptr, 10);

                if ( endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "Invalid --tick duration: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                if ( (errno == ERANGE && (tick_ms == LONG_MAX || tick_ms < 0)) || (tick_ms <= 0) ) {
                    fprintf(stderr, "Invalid --tick duration (Out of long type range OR negative): %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                break;
            }
            case 'v': verbose_mode = true;    break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (account_file == NULL || trace_file == NULL) {
        fprintf(stderr, "Error: --accounts and --trace are required arguments.\n");
        exit(EXIT_FAILURE);
    }

    if (load_accounts(&bank, account_file) != 0) {
        fprintf(stderr, "Failed to load accounts from: %s\n", account_file);
        exit(EXIT_FAILURE);
    }
    
    if (load_transactions(tsx, trace_file) != 0) { 
        fprintf(stderr, "Failed to load transactions from: %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
}
