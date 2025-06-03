#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include <newmm.h>
#include <lib.h>
#include <scheduler.h>
#include <structs.h>

#define STACK_SIZE 4096
#define MAX_PROCESSES 10

#define MM_STRUCT_SIZE      0x1000
#define HEAP_REGION_SIZE      0x40000
#define STACK_REGION_SIZE     0x20000

void initializeProcessManager();
mm_t * getHeap();
void waitpid(uint64_t pid);
uint64_t createProcess(function fn, int argc, char * argv[], int priority);
void exitProcess();

#endif