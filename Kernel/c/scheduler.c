#include <scheduler.h>

Process * currentProcess;

void scheduler(){
    currentProcess->tickCount++;
    if(currentProcess->tickCount == TICKS_PER_QUANTUM){
        
    }
}
