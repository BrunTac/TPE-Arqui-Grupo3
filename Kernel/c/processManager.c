#include <processManager.h>
#include <memoryManager.h>
#include <lib.h>

extern char endOfKernel;

void setNewProcessStack(int argc, char * argv[], void * stackPtr, function fn);

MemoryManagerADT heapManager;
MemoryManagerADT stackManager;
entryPCB * processes[MAX_PROCESSES];

void initMemoryManagers() {
    void *metadataHeap  = (void *)&endOfKernel;
    void *heapMemory    = (void *)((uint64_t)&endOfKernel + MM_STRUCT_SIZE);

    void *metadataStack = (void *)((uint64_t)heapMemory + HEAP_REGION_SIZE);
    void *stackMemory   = (void *)((uint64_t)metadataStack + MM_STRUCT_SIZE);

    heapManager = createMemoryManager(metadataHeap, heapMemory);
    stackManager = createMemoryManager(metadataStack, stackMemory);
}

/*void * processWrapper(void * arg) {
    ProcessStartArgs * args = (ProcessStartArgs *) arg;
    args->fn(args->argc, args->argv);
    exit(0);
    return NULL;
}*/

void initializeProcessManager(){
    initMemoryManagers();
    
    for(uint64_t i = 0; i < MAX_PROCESSES; i++){
        processes[i]->isEmpty = 1;
    }

    initScheduler();
}

void createProcess(function fn, int argc, char * argv[], int priority){
    void * stackPtr = allocMemory(stackManager, STACK_SIZE);
    stackPtr += STACK_SIZE;

    /*ProcessStartArgs * args = allocMemory(heapManager, sizeof(ProcessStartArgs));
    args->fn = fn;
    args->argc = argc;
    args->argv = argv;

    setNewProcessStack(1, (char *[]) { (char *) args }, stackPtr, (function) processWrapper);*/

    setNewProcessStack(argc, argv, stackPtr, fn);

    uint64_t pid;
    for(pid = 0; pid < MAX_PROCESSES; pid++){
        if(processes[pid]->isEmpty){
            break ;
        }
    }
    if(pid == MAX_PROCESSES){
        // MAYBE PRINT ERROR MESSAGE: PROCESS LIMIT REACHED
        return ;
    }
    processes[pid]->isEmpty = 0;
    processes[pid]->pid = pid;
    processes[pid]->status = READY;

    Process newProcess = {0, stackPtr, processes[pid]};
    if(!addNode(&readyProcesses, &newProcess)){
        // MAYBE PRINT ERROR MESSAGE: PROCESS LIMIT REACHED
    }
    return ;
}


