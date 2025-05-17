#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef enum {
    READY = 0,
    BLOCKED,
    EXITED
} Status;

typedef struct {
    uint64_t pid;
    Status status;
    uint8_t isEmpty;
} entryPCB;

typedef struct{
    uint32_t tickCount;
    void * rsp;
    entryPCB * PCB;
} Process;

#endif