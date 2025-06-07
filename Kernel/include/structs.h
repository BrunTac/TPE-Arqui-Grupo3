#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "BlockedQueueADT.h"

#define MAX_CHAR_NAME 20

#include <stddef.h>

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

// normal mm structs

typedef struct header {
    struct header *next;
    size_t size;
    uint8_t isFree;
} header_t;

typedef struct {
    header_t *firstBlock;
    uintptr_t memStart;
    uintptr_t memEnd;
} mm_t;

// buddy mm structs

typedef struct buddy {
    struct buddy *buddy, *left, *right, *parent;
    uint8_t isFree;
    size_t size;
} buddy_t;

typedef struct {
    buddy_t *megaBuddy;
    uintptr_t memStart;
    uintptr_t memEnd;
} bmm_t;

#endif