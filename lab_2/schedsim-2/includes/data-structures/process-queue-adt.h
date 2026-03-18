#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H

#include "./../objects/process.h"

typedef struct AbstractProcessQueue {
    void (*enqueue)(struct AbstractProcessQueue*, Process* process);
    Process* (*dequeue)(struct AbstractProcessQueue*);
    void (*destruct_queue)(struct AbstractProcessQueue*);
    int size;
} AbstractProcessQueue;

#endif