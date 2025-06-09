#include <scheduler.h>

void int_20h();

uint64_t currentProcess;
static Process processes[MAX_PROCESSES];

void root(){
    yieldProcess(getCurrentProcess());
}

void initScheduler(){
    currentProcess = -1;
    char * name = "root";
	char * argv[] = {name};
    currentProcess = createProcess(root, 1, argv, 0, name, NULL);

    for(int i = 1; i < MAX_PROCESSES; i++){
        processes[i].status = -1;
    }
}

uint64_t getCurrentProcess(){
    return currentProcess;
}

void addToScheduler(uint64_t pid, int argc, char * argv[], uint64_t stackPtr, function fn, uint8_t priority){
    processes[pid].rsp = setNewProcessStack(argc, argv, stackPtr, fn);
    processes[pid].priority = priority;
    processes[pid].status = READY;
    processes[pid].tickCount = 0;
}

void removeFromScheduler(uint64_t pid){
    processes[pid].status = EXITED;
}

uint64_t scheduler(uint64_t rsp){
    processes[currentProcess].rsp = rsp;

    if(processes[currentProcess].status != READY || processes[currentProcess].tickCount >= TICKS_PER_QUANTUM * processes[currentProcess].priority){
        do{
            currentProcess = (currentProcess + 1) % MAX_PROCESSES;
        }while(processes[currentProcess].status != READY);
        processes[currentProcess].tickCount = 0;
    }else{
        processes[currentProcess].tickCount++;
    }

    return processes[currentProcess].rsp;
}

void blockProcess(uint64_t pid){
    processes[pid].status = BLOCKED;
}

void unblockProcess(uint64_t pid){
    processes[pid].status = READY;
}

void yieldProcess(uint64_t pid){
    processes[pid].tickCount = TICKS_PER_QUANTUM * processes[currentProcess].priority;
    int_20h();
}

void getProcessInfo(uint64_t pid, ProcessInfo *info){
    info->status = processes[pid].status;
    info->rsp = processes[pid].rsp; 
    info->priority = processes[pid].priority;
}

int64_t changePriority(uint64_t pid, uint8_t priority){
    if (isValidPid(pid) && priority > 0 && priority < 10){
        processes[pid].priority = priority;
        return 0;
    }
    return -1;
} 

void terminateForeground() {
    for(int i = 2 ; i < MAX_PROCESSES ; i++)
        if(processes[i].status == READY) {
            yieldProcess(i);
            exitProcess(i);
        }
}