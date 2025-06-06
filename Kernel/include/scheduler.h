#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <stddef.h>
#include <structs.h>
#include <interrupts.h>
#include <lib.h>
#include <processManager.h>


#define TICKS_PER_QUANTUM 5

void initScheduler();
uint64_t getCurrentProcess();
void addToScheduler(uint64_t pid, int argc, char * argv[], uint64_t stackPtr, function fn, uint8_t priority);
void removeFromScheduler(uint64_t pid);
uint64_t scheduler(uint64_t rsp);
uint64_t setNewProcessStack(int argc, char * argv[], uint64_t stackPtr, function fn);
void blockProcess(uint64_t pid);
void unblockProcess(uint64_t pid);
void yieldProcess(uint64_t pid);
void getProcessInfo(uint64_t pid, ProcessInfo *info);

extern uint64_t currentProcess;

#endif