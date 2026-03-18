#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H

typedef struct AbstractQueue {
    void (*enqueue)(struct AbstractQueue*, void* item);
    void* (*dequeue)(struct AbstractQueue*);
    int size;
} AbstractQueue;

#endif