#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include <memoryManager.h>
#include <lib.h>
#include <scheduler.h>
#include <structs.h>

#define STACK_SIZE 4096
#define MAX_PROCESSES 30

#define MM_STRUCT_SIZE      0x1000
#define HEAP_REGION_SIZE      0x40000
#define STACK_REGION_SIZE     0x20000

extern memory_manager_t *heapManager;
extern memory_manager_t *stackManager;

void initializeProcessManager();
void waitpid(uint64_t pid);
int64_t createProcess(function fn, int argc, char * argv[], int priority, const char * name, uint8_t fds[FD_AMOUNT]);
int64_t exitProcess(uint64_t pid);
int64_t listProcesses(ProcessInfo * buffer);
int8_t getInputFd(uint64_t pid);
int8_t getOutputFd(uint64_t pid);
int8_t getErrorFd(uint64_t pid);
uint8_t isValidPid(uint64_t pid);
void ctrlC();
void ctrlD();

#endif