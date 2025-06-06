#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "BlockedQueueADT.h"

#define MAX_CHAR_NAME 20

typedef enum {
    READY = 0,
    BLOCKED,
    EXITED
} Status;

typedef struct {
    uint64_t pid;
    uint64_t ppid;
    uint64_t isEmpty;
    BlockedQueueADT blockedQueue;
} entryPCB;

typedef struct{
    uint32_t tickCount;
    uint64_t rsp;
    uint8_t priority;
    Status status;
} Process;

typedef struct{
    uint64_t value;
    const char * name;
    BlockedQueueADT waitingQueue;
    uint8_t processCount;
    uint8_t lock;
} Semaphore;

#endif