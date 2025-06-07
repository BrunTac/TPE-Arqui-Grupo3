#include "pipes.h"
#include "semaphores.h"
#include "structs.h"
#include "newmm.h"
#include "processManager.h"
#include "scheduler.h"

Pipe * pipes[MAX_PIPES] = {NULL};

uint8_t pipe_open(const char * name){
    int8_t firstFree = -1;
    for(uint8_t i = 0; i < MAX_PIPES; i++){
        if(firstFree == -1 && !pipes[i]){
            firstFree = i;
        }
        if(pipes[i] && strcmp(pipes[i]->name, name) == 0){
            pipes[i]->openingProcessCount++;
            return i;
        }
    }
    if(firstFree == -1){
        //ERROR ????
    }
    pipes[firstFree] = malloc_mm(getHeap(), sizeof(Pipe));

    pipes[firstFree]->name = name;
    pipes[firstFree]->readingIdx = 0;
    pipes[firstFree]->toBeRead = 0;
    pipes[firstFree]->writingQueue = newQueue();
    pipes[firstFree]->readingQueue = newQueue();
    pipes[firstFree]->sem = sem_open(name, 1);
    pipes[firstFree]->openingProcessCount = 1;

    return firstFree + FD_AMOUNT;
}

void pipe_write(uint8_t pipeId, char c){
    if(!pipe_isValid(pipeId)){
        // ERROR ??????????
        return ;
    }
    uint8_t idx = pipeId - FD_AMOUNT;

    sem_wait(pipes[idx]->sem);
    while(pipes[idx]->toBeRead == MAX_BUFFER){
        uint64_t pid = getCurrentProcess();
        enqueue(pipes[idx]->writingQueue, pid);
        sem_post(pipes[idx]->sem);
        blockProcess(pid);
        yieldProcess(pid);
        sem_wait(pipes[idx]->sem);
    }
    pipes[idx]->buffer[(pipes[idx]->readingIdx + pipes[idx]->toBeRead) % MAX_BUFFER] = c;
    if(pipes[idx]->toBeRead++ == 0){
        dequeue(pipes[idx]->readingQueue);
    }
    sem_post(pipes[idx]->sem);
}

char pipe_read(uint8_t pipeId){
    if(!pipe_isValid(pipeId)){
        // ERROR ??????????
        return 0;
    }
    uint8_t idx = pipeId - FD_AMOUNT;

    sem_wait(pipes[idx]->sem);
    while(pipes[idx]->toBeRead == 0){
        uint64_t pid = getCurrentProcess();
        enqueue(pipes[idx]->readingQueue, pid);
        sem_post(pipes[idx]->sem);
        blockProcess(pid);
        yieldProcess(pid);
        sem_wait(pipes[idx]->sem);
    }
    char c = pipes[idx]->buffer[pipes[idx]->readingIdx];

    pipes[idx]->readingIdx = (pipes[idx]->readingIdx + 1) % MAX_BUFFER;
    if(pipes[idx]->toBeRead-- == MAX_BUFFER){
        dequeue(pipes[idx]->writingQueue);
    }
    sem_post(pipes[idx]->sem);

    return c;
}

void pipe_close(uint8_t pipeId){
    if(!pipe_isValid(pipeId)){
        // ERROR ??????????
        return ;
    }
    uint8_t idx = pipeId - FD_AMOUNT;

    sem_wait(pipes[idx]->sem);
    if(pipes[idx]->openingProcessCount-- == 1){
        sem_post(pipes[idx]->sem);
        sem_close(pipes[idx]->sem);
        free_mm(getHeap(), pipes[idx]);
        pipes[idx] = NULL;
        return ;
    }
    sem_post(pipes[idx]->sem);
}

uint8_t pipe_isValid(uint8_t pipeId){
    uint8_t idx = pipeId - FD_AMOUNT;
    return idx >= 0 && idx < MAX_PIPES && pipes[idx] != NULL;
}