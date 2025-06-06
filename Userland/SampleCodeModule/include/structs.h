#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#define MAX_CHAR_NAME 20
#define MAX_PROCESSES 10

typedef enum {
    READY = 0,
    BLOCKED,
    EXITED
} Status;

typedef struct {
    uint64_t pid;
    uint64_t ppid;
    uint8_t priority;
    uint64_t rsp;
    Status status;
    char name[MAX_CHAR_NAME];
} ProcessInfo;

#endif