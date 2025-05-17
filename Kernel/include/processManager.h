#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include <memoryManager.h>
#include <lib.h>
#include <circularListStatic.h>
#include <scheduler.h>
#include <structs.h>

#define STACK_SIZE 4096
#define MAX_PROCESSES 1024

#define MM_STRUCT_SIZE      0x1000
#define HEAP_REGION_SIZE      0x40000
#define STACK_REGION_SIZE     0x20000

/*typedef struct {
    function fn;
    int argc;
    char ** argv;
} ProcessStartArgs;*/

void initializeProcessManager();
void createProcess(function fn, int argc, char * argv[], int priority);

#endif