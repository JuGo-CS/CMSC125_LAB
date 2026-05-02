#ifndef BANK_H
#define BANK_H
#include <stdbool.h>

void init_bank(int num_accounts);
int get_balance(int account_id);
void deposit(int account_id, int amount_centavos);
bool withdraw(int account_id, int amount_centavos);
bool transfer(int from_id, int to_id, int amount_centavos, int tx_id);

#endif 