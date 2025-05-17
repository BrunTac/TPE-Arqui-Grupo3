#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <linkedListStatic.h>

#define TICKS_PER_QUANTUM 10

typedef struct{
    uint64_t pid;
    uint32_t tickCount;
} Process;

void scheduler();

#endif