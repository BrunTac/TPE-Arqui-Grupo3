#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "BlockedQueueADT.h"

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

#endif