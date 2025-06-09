#include <processManager.h>
#include <memoryManager.h>
#include <lib.h>
#include "pipes.h"

extern char endOfKernel;
void int_20h();

static entryPCB processes[MAX_PROCESSES];

memory_manager_t *heapManager;
memory_manager_t *stackManager;

uint8_t terminatingForeground = 0;

void freeProcess(uint64_t pid){
    freeQueue(processes[pid].blockedQueue);
    for(uint64_t i = 0; i < processes[pid].argc; i++){
        free_mm(stackManager, (void *) processes[pid].argv[i]);
    }
    free_mm(stackManager, (void *) processes[pid].argv);
    free_mm(stackManager, (void *) processes[pid].stackPtr);
}

int64_t exitProcess(uint64_t pid){ 
    if (isValidPid(pid)){
        int8_t fd = getOutputFd(pid);
        if(fd != STDOUT){
            pipe_write(fd, '\0');
        }
        processes[pid].isEmpty = 1;
        removeFromScheduler(pid);
        emptyQueue(processes[pid].blockedQueue);
        freeProcess(pid);
        
        if(!terminatingForeground){
            int_20h();
        }
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
        return ;
    }
    if(pid == currentPid || isQueued(processes[currentPid].blockedQueue, pid)){
        return ;
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

int64_t createProcess(function fn, int argc, char * argv[], int priority, uint8_t fds[FD_AMOUNT], uint8_t isBackground){
    uint64_t stackPtr = (uint64_t) malloc_mm(stackManager, STACK_SIZE);
    stackPtr += STACK_SIZE;
    uint64_t pid;
    for(pid = 0; pid < MAX_PROCESSES; pid++){
        if(processes[pid].isEmpty){
            break ;
        }
    }
    if(pid == MAX_PROCESSES){
        return -1;
    }
    processes[pid].isEmpty = 0;
    processes[pid].pid = pid;
    processes[pid].ppid = getCurrentProcess();
    processes[pid].argc = argc;
    processes[pid].argv = (char **) malloc_mm(stackManager, sizeof(char *) * argc);
    for(uint64_t i = 0; i < argc; i++){
        processes[pid].argv[i] = (char *) malloc_mm(stackManager, strlen(argv[i]) + 1);
        strcpy(processes[pid].argv[i], argv[i]);
    }
    processes[pid].fn = fn;
    processes[pid].blockedQueue = newQueue();
    processes[pid].stackPtr = (uintptr_t) stackPtr;
    strcpy(processes[pid].name, argv[0]);
    processes[pid].fileDescriptors[0] = fds[0];
    processes[pid].fileDescriptors[1] = fds[1];
    processes[pid].fileDescriptors[2] = fds[2];
    processes[pid].isBackground = isBackground;
    addToScheduler(pid, processes[pid].argc, processes[pid].argv, stackPtr, processes[pid].fn, priority);
    return pid;
}

int64_t listProcesses(ProcessInfo * buffer){

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

void terminateForeground() {
    terminatingForeground = 1;
    for(int i = 2 ; i < MAX_PROCESSES ; i++){
        if(!processes[i].isBackground) {
            exitProcess(i);
        }
    }
    terminatingForeground = 0;
    int_20h();
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


