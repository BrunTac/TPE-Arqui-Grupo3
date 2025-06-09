#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "libc.h"

#define MAX_CHAR_NAME 20
#define MAX_PROCESSES 30

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

typedef struct{
    char * name;
    char * description;
    function fn;
    uint64_t argc;
    uint8_t isBuiltIn;
    uint8_t canBeBackground;
} Command;

#endif