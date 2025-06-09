#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "BlockedQueueADT.h"
#include "lib.h"

#define MAX_CHAR_NAME 20

typedef enum {
    READY = 0,
    BLOCKED,
    EXITED
} Status;

typedef struct {
    uint64_t pid;
    uint64_t ppid;
    uint64_t argc;
    char ** argv;
    function fn;
    uint64_t isEmpty;
    BlockedQueueADT blockedQueue;
    char name[MAX_CHAR_NAME];
    uint8_t fileDescriptors[FD_AMOUNT];
} entryPCB;

typedef struct{
    uint32_t tickCount;
    uint64_t rsp;
    uint8_t priority;
    Status status;
} Process;

typedef struct {
    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint64_t rsp;
    Status status;
    char name[MAX_CHAR_NAME];
} ProcessInfo;

typedef struct{
    uint64_t value;
    const char * name;
    BlockedQueueADT waitingQueue;
    uint8_t processCount;
    uint8_t lock;
} Semaphore;

typedef struct{
    const char * name;
    char buffer[MAX_BUFFER];
    uint32_t readingIdx;
    uint32_t toBeRead;
    BlockedQueueADT writingQueue;
    BlockedQueueADT readingQueue;
    uint8_t sem;
    uint8_t openingProcessCount;
} Pipe;

#endif