#include <scheduler.h>

Process * currentProcess = NULL;
List readyProcesses;

void initScheduler(){
    initList(&readyProcesses);
}

void * scheduler(void * rsp){
    if(isEmpty(&readyProcesses)){
        return NULL;
    }
    currentProcess = (Process *) getCurrentData(&readyProcesses);

    currentProcess->tickCount++;
    currentProcess->rsp = rsp;

    if(currentProcess->PCB->status == BLOCKED){
        removeCurrentNode(&readyProcesses);
        currentProcess = (Process *) getCurrentData(&readyProcesses);
    }else if(currentProcess->PCB->status == EXITED){
        currentProcess->PCB->isEmpty = 1;
        removeCurrentNode(&readyProcesses);
        currentProcess = (Process *) getCurrentData(&readyProcesses);
    }else if(currentProcess->tickCount == TICKS_PER_QUANTUM){
        nextNode(&readyProcesses);
        currentProcess = (Process *) getCurrentData(&readyProcesses);
    }

    return currentProcess->rsp;
}
