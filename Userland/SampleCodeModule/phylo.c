/*#include "sys_calls.h"
#include "phylo.h"
#include "libc.h"

#define MAX_PHILOSOPHERS 20

#define LEFT i
#define RIGHT (i+1) % philosophersAmount

typedef enum {
    THINKING = 0,
    EATING
} PhiloStatus;

typedef struct {
    uint64_t pid;
    PhiloStatus status;
} Philosopher;

int8_t forks[MAX_PHILOSOPHERS];
Philosopher philos[MAX_PHILOSOPHERS];
uint8_t philosophersAmount;

uint8_t fds[] = {STDIN, STDOUT, STDERR};

uint8_t printUpdateSem;

void phylo(uint64_t argc, char ** argv){
    philosophersAmount = atoi(argv[1]);
    for(uint8_t i = 0; i < philosophersAmount; i++){
        addPhilosopher(i);
    }
    printUpdateSem = sys_openSem("printUpdateSem", 0);
    char * argvView[] = {"view"};
    uint64_t viewPid = sys_createProcess(view, 1, argvView, 1, argvView[0], fds);
    
    char c;
    while((c = getChar()) != '\0'){
        if(c == 'a' && philosophersAmount < MAX_PHILOSOPHERS){
            addPhilosopher(philosophersAmount++);
        }else if(c == 'r' && philosophersAmount > 0){
            removePhilosopher(--philosophersAmount);
        }
    }
    for(uint8_t i = 0; i < philosophersAmount; i++){
        removePhilosopher(i);
    }
    sys_killProcess(viewPid);
    sys_closeSem(printUpdateSem);
}

void addPhilosopher(uint8_t idx){
    char name[MAX_BUFFER];
    numToStr(idx, name);
    forks[idx] = sys_openSem(name, 1);

    char aux[2] = {idx, '\0'};
    char * argv[] = {name, aux};
    philos[idx].pid = sys_createProcess(philosopher, 2, argv, 1, name, fds);
    philos[idx].status = THINKING;
}

void removePhilosopher(uint8_t idx){
    sys_killProcess(philos[idx].pid);
    sys_closeSem(forks[idx]);
}

void view(){
    while(1){
        sys_waitSem(printUpdateSem);
        for(uint8_t i = 0; i < philosophersAmount; i++){
            printf("%s ", philos[i].status == EATING ? "E" : ".");
        }
    }
}

void philosopher(uint64_t argc, char ** argv){
    uint8_t i = atoi(argv[1]);
    while(1){
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
}

void take_forks(int i){
    if(i % 2 == 1){
        sys_waitSem(forks[LEFT]);
        sys_waitSem(forks[RIGHT]);
    }else{
        sys_waitSem(forks[RIGHT]);
        sys_waitSem(forks[LEFT]);
    }
}

void put_forks(int i){
    sys_postSem(forks[LEFT]);
    sys_postSem(forks[RIGHT]);
}

void think(int i){
    philos[i].status = THINKING;
    sys_postSem(printUpdateSem);
    sys_sleep(20);
}

void eat(int i){
    philos[i].status = EATING;
    sys_postSem(printUpdateSem);
    sys_sleep(20);
}
*/
