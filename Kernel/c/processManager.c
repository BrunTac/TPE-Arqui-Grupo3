#include <processManager.h>
#include <memoryManager.h>
#include <lib.h>

extern char endOfKernel;
void int_20h();

static entryPCB processes[MAX_PROCESSES];
MemoryManagerADT heapManager;
MemoryManagerADT stackManager;

void exitProcess(){
    processes[currentProcess].isEmpty = 1;
    removeFromScheduler(currentProcess);
    int_20h();
}

void initMemoryManagers() {
    void *metadataHeap  = (void *)&endOfKernel;
    void *heapMemory    = (void *)((uint64_t)&endOfKernel + MM_STRUCT_SIZE);

    void *metadataStack = (void *)((uint64_t)heapMemory + HEAP_REGION_SIZE);
    void *stackMemory   = (void *)((uint64_t)metadataStack + MM_STRUCT_SIZE);

    heapManager = createMemoryManager(metadataHeap, heapMemory);
    stackManager = createMemoryManager(metadataStack, stackMemory);
}

void initializeProcessManager(){
    initMemoryManagers();
    
    for(uint64_t i = 0; i < MAX_PROCESSES; i++){
        processes[i].isEmpty = 1;
    }

    initScheduler();
}

void createProcess(function fn, int argc, char * argv[], int priority){
    uint64_t stackPtr = (uint64_t) allocMemory(stackManager, STACK_SIZE);
    stackPtr += STACK_SIZE;

    uint64_t pid;
    for(pid = 0; pid < MAX_PROCESSES; pid++){
        if(processes[pid].isEmpty){
            break ;
        }
    }
    if(pid == MAX_PROCESSES){
        // MAYBE PRINT ERROR MESSAGE: PROCESS LIMIT REACHED
        
    }
    processes[pid].isEmpty = 0;
    processes[pid].pid = pid;
    processes[pid].ppid = currentProcess;
    addToScheduler(pid, argc, argv, stackPtr, fn, priority);
    return ;
}


