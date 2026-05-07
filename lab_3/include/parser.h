#ifndef PARSER_H
#define PARSER_H
#include "bankdb.h"

void parse_command_line(int, char *[]);
void print_system_state(Bank bank, Transaction *tx_list);
#endif
