#ifndef BLOCKED_QUEUE_ADT_H
#define BLOCKED_QUEUE_ADT_H

#include <stdint.h>

typedef struct BlockedQueue * BlockedQueueADT;

BlockedQueueADT newQueue(void);

void enqueue(BlockedQueueADT queue, uint64_t pid);

void dequeue(BlockedQueueADT queue);

void freeQueue(BlockedQueueADT queue);

uint8_t isQueued(BlockedQueueADT queue, uint64_t pid);

void emptyQueue(BlockedQueueADT queue); 

#endif
