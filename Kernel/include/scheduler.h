#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <stddef.h>
#include <circularListStatic.h>
#include <structs.h>

#define TICKS_PER_QUANTUM 5

void initScheduler();
void * scheduler(void * rsp);

extern Process * currentProcess;
extern List readyProcesses;

#endif