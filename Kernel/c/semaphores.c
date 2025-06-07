#include "semaphores.h"
#include "structs.h"
#include "memoryManager.h"
#include "processManager.h"
#include "scheduler.h"

extern int tryLock(uint8_t * lock);
extern void unlock(uint8_t * lock);

Semaphore * sems[MAX_SEMAPHORES] = {NULL};

void sem_post(uint8_t sem){
    if(!sem_isValid(sem)){
        // ERROR ??????????
        return ;
    }

    tryLock(&sems[sem]->lock);

    if(sems[sem]->value++ == 0){
        dequeue(sems[sem]->waitingQueue);
    }

    unlock(&sems[sem]->lock);
}

void sem_wait(uint8_t sem){
    if(!sem_isValid(sem)){
        // ERROR ??????????
        return ;
    }
    tryLock(&sems[sem]->lock);
    while(sems[sem]->value == 0){
        uint64_t pid = getCurrentProcess();
        enqueue(sems[sem]->waitingQueue, pid);
        unlock(&sems[sem]->lock);
        blockProcess(pid);
        yieldProcess(pid);
        tryLock(&sems[sem]->lock);
    }
    sems[sem]->value--;

    unlock(&sems[sem]->lock);
}

uint8_t sem_open(const char * name, uint64_t value){
    int8_t firstFree = -1;
    for(uint8_t i = 0; i < MAX_SEMAPHORES; i++){
        if(firstFree == -1 && !sems[i]){
            firstFree = i;
        }
        if(strcmp(sems[i]->name, name) == 0){
            sems[i]->processCount++;
            return i;
        }
    }
    if(firstFree == -1){
        //ERROR ????
    }
    sems[firstFree] = malloc(sizeof(Semaphore));

    sems[firstFree]->value = value;
    sems[firstFree]->name = name;
    sems[firstFree]->waitingQueue = newQueue();
    sems[firstFree]->processCount = 1;
    sems[firstFree]->lock = 0;

    return firstFree;
}

void sem_close(uint8_t sem){
    if(!sem_isValid(sem)){
        // ERROR ??????????
        return ;
    }
    tryLock(&sems[sem]->lock);
    if(sems[sem]->processCount-- == 1){
        unlock(&sems[sem]->lock);
        free(sems[sem]);
        sems[sem] = NULL;
        return ;
    }
    unlock(&sems[sem]->lock);
}

uint8_t sem_isValid(uint8_t sem){
    return sem >= 0 && sem < MAX_SEMAPHORES && sems[sem] != NULL;
}