#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "parser.h"
#include "bankdb.h"

bool verbose_mode;
bool deadlock_prevention;
int tick_ms;
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
   
    while (fgets(line, sizeof(line), file) && bank->num_accounts < MAX_ACCOUNTS) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        if (sscanf(line, "%d %ld", &id, &bal) == 2) {
            bank->accounts[bank->num_accounts].account_id = id;
            bank->accounts[bank->num_accounts].balance_centavos = bal;
            bank->num_accounts++;
        }
    }

    fclose(file);
    return 0;
}


void print_system_state(Bank bank, Transaction *tx_list) {
    printf("\n==========================================\n");
    printf("   🏦 CURRENT BANK DATABASE STATE   \n");
    printf("==========================================\n");

    // 1. Print Accounts
    printf("ACCOUNTS:\n");
    printf("------------------------------------------\n");
    printf("%-12s | %-15s\n", "Account ID", "Balance");
    printf("------------------------------------------\n");
    for (int i = 0; i < bank.num_accounts; i++) {
        // Converting centavos to a decimal-style display
        double dollars = bank.accounts[i].balance_centavos / 100.0;
        printf("ID: %-8d | $%-14.2f\n", 
               bank.accounts[i].account_id, dollars);
    }

    printf("\n==========================================\n");
    printf("   📜 TRANSACTION WORKLOAD   \n");
    printf("==========================================\n");

    // 2. Print Transactions using the sentinel (-1)
    printf("%-5s | %-5s | %-10s | %-15s\n", "ID", "Tick", "Type", "Details");
    printf("------------------------------------------\n");
    
    for (int i = 0; i < MAX_TRANSACTIONS && tx_list[i].tx_id != TRANSACTION_END; i++) {
        Transaction *tx = &tx_list[i];
        Operation *op = &tx->ops[0]; // Assuming 1 op per tx for now

        char *type_str;
        switch(op->type) {
            case OP_DEPOSIT:  type_str = "DEPOSIT";  break;
            case OP_WITHDRAW: type_str = "WITHDRAW"; break;
            case OP_TRANSFER: type_str = "TRANSFER"; break;
            case OP_BALANCE:  type_str = "BALANCE";  break;
            default:          type_str = "UNKNOWN";  break;
        }

        printf("T%-4d | %-5d | %-10s | ", i, tx->start_tick, type_str);

        if (op->type == OP_TRANSFER) {
            printf("Acc %d -> %d ($%.2f)\n", 
                   op->account_id, op->target_account, op->amount_centavos / 100.0);
        } else if (op->type == OP_BALANCE) {
            printf("Acc %d\n", op->account_id);
        } else {
            printf("Acc %d ($%.2f)\n", 
                   op->account_id, op->amount_centavos / 100.0);
        }
    }
    printf("==========================================\n\n");
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
            Transaction *tx = &tx_list[tx_count];
            
            tx->tx_id = atoi(&tx_label[1]);
            tx->start_tick = tick;
            tx->status = TX_RUNNING;
            tx->num_ops = 1; 

            Operation *op = &tx->ops[0];
            op->type = get_op_type(op_str);
            op->account_id = acc_id;

            if (op->type == OP_TRANSFER) {
                sscanf(line, "%*s %*d %*s %*d %d %d", 
                       &op->target_account, &op->amount_centavos);
            } else if (op->type != OP_BALANCE) {
                sscanf(line, "%*s %*d %*s %*d %d", &op->amount_centavos);
            }

            tx_count++;
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
            case 'm': tick_ms = atoi(optarg); break;
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
