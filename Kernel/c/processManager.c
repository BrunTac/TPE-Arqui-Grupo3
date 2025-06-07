#include <processManager.h>
#include <memoryManager.h>
#include <lib.h>

extern char endOfKernel;
void int_20h();

static entryPCB processes[MAX_PROCESSES];

memory_manager_t *heapManager;
memory_manager_t *stackManager;

int64_t exitProcess(uint64_t pid){ 
    if (isValidPid(pid)){
        processes[pid].isEmpty = 1;
        removeFromScheduler(pid);

    emptyQueue(processes[pid].blockedQueue);
    free_mm(stackManager, stackPtr);
    
        int_20h();
        return 0;
    }
    return -1;
}

void initMemoryManagers() {
    void *metadataHeap  = (void *)&endOfKernel;
    void *heapMemory    = (void *)((uint64_t)&endOfKernel + MM_STRUCT_SIZE);

    void *metadataStack = (void *)((uint64_t)heapMemory + HEAP_REGION_SIZE);
    void *stackMemory   = (void *)((uint64_t)metadataStack + MM_STRUCT_SIZE);

    heapManager = createMemoryManager_mm(metadataHeap, heapMemory, HEAP_REGION_SIZE);
    stackManager = createMemoryManager_mm(metadataStack, stackMemory, STACK_REGION_SIZE);
}

void waitpid(uint64_t pid){
    if(!isValidPid(pid)){
        return ;
    }
    uint64_t currentPid = getCurrentProcess();
    if(currentPid < 0){
        // ERROR ????
    }
    if(pid == currentPid || isQueued(processes[currentPid].blockedQueue, pid)){
        // KILL(pid) o algun warning ?
    }
    enqueue(processes[pid].blockedQueue, currentPid);
    blockProcess(currentPid);
    yieldProcess(currentPid);
}

void initializeProcessManager(){
    initMemoryManagers();
    
    for(uint64_t i = 0; i < MAX_PROCESSES; i++){
        processes[i].isEmpty = 1;
    }

    initScheduler();
}

uint64_t createProcess(function fn, int argc, char * argv[], int priority, const char * name, uint8_t fds[FD_AMOUNT]){
    uint64_t stackPtr = (uint64_t) malloc_mm(stackManager, STACK_SIZE);
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
    processes[pid].ppid = getCurrentProcess();
    processes[pid].blockedQueue = newQueue();
    strcpy(processes[pid].name, name);
    processes[pid].fileDescriptors[0] = fds[0];
    processes[pid].fileDescriptors[1] = fds[1];
    processes[pid].fileDescriptors[2] = fds[2];
    addToScheduler(pid, argc, argv, stackPtr, fn, priority);
    return pid;
}

uint64_t listProcesses(ProcessInfo * buffer){

    uint64_t count = 0;
    for(uint64_t i = 0; i < MAX_PROCESSES && count < MAX_PROCESSES; i++){
        if(!processes[i].isEmpty){
            buffer[count].pid = processes[i].pid;
            buffer[count].ppid = processes[i].ppid;
            strcpy(buffer[count].name, processes[i].name);
            getProcessInfo(i, &buffer[count]);
            count++;
        }
    }
    return count;
}

int8_t getInputFd(uint64_t pid){
    if(pid > 0 && pid < MAX_PROCESSES && !processes[pid].isEmpty){
        return processes[pid].fileDescriptors[0];
    }
    return -1;
}

int8_t getOutputFd(uint64_t pid){
    if(pid > 0 && pid < MAX_PROCESSES && !processes[pid].isEmpty){
        return processes[pid].fileDescriptors[1];
    }
    return -1;
}

int8_t getErrorFd(uint64_t pid){
    if(isValidPid(pid)){
        return processes[pid].fileDescriptors[2];
    }
    return -1;
}

uint8_t isValidPid(uint64_t pid){
    return pid > 0 && pid < MAX_PROCESSES && !processes[pid].isEmpty;
}




