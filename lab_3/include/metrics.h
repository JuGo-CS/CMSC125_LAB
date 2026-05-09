#ifndef METRICS_H
#define METRICS_H

#include "transaction.h"
#include "buffer_pool.h"

void print_performance_metrics(Transaction* tsx, int global_tick);
void print_summary(Transaction* tsx, int global_tick);
void print_buffer_pool_report(BufferPool* pool);

#endif