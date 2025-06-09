#include "phylo.h"
#include "libc.h"
#include "sys_calls.h"

void addPhilosopher(uint8_t idx);
void removePhilosopher(uint8_t idx);
void view();
void philosopher(uint64_t argc, char ** argv);
uint8_t left(uint8_t i);
uint8_t right(uint8_t i);
void take_forks(int i);
void put_forks(int i);
void think(int i);
void eat(int i);

char * names[] = {"Lift", "Wit", "Szeth", "Jasnah", "Adolin", "Kaladin", "Shallan", "Dalinar", "Navani"};

typedef enum {
    THINKING = 0,
    EATING
} PhiloStatus;

typedef struct {
    uint64_t pid;
    PhiloStatus status;
    uint8_t waitingLeft;
    uint8_t waitingRight;
} Philosopher;

int8_t forks[MAX_PHILOSOPHERS];
Philosopher philos[MAX_PHILOSOPHERS];
uint8_t philosophersAmount;

uint8_t fds[] = {STDIN, STDOUT, STDERR};

uint8_t printUpdateSem;
uint8_t printing;

//uint8_t mutex;

void phylo(uint8_t  philos){
    philosophersAmount = philos;
    for(uint8_t i = 0; i < philosophersAmount; i++){
        addPhilosopher(i);
    }
    //mutex = sys_openSem("mutex", 1);

    printUpdateSem = sys_openSem("printUpdateSem", 1);
    printing = 0;
    char * argv[] = {"view"};
    uint64_t viewPid = sys_createProcess(view, 1, argv, 1, argv[0], fds);
    
    char c;
    while((c = getChar()) != '\0'){
        if(c == 'a' && philosophersAmount < MAX_PHILOSOPHERS){
            printf("Inviting over philosopher %s%n", names[philosophersAmount]);
            /*if(philos[0].waitingLeft){
                sys_postSem(forks[0]);
            }
            if(philos[philosophersAmount - 1].waitingRight){
                sys_postSem(forks[0]);
            }*/
            addPhilosopher(philosophersAmount++);
        }else if(c == 'r' && philosophersAmount > 0){
            printf("See you soon philosopher %s!%n", names[philosophersAmount - 1]);
            removePhilosopher(--philosophersAmount);
        }else if(c == 'e'){
            break ;
        }
    }
    for(uint8_t i = 0; i < philosophersAmount; i++){
        removePhilosopher(i);
    }
    sys_killProcess(viewPid);
    sys_closeSem(printUpdateSem);
}

void addPhilosopher(uint8_t idx){
    forks[idx] = sys_openSem(names[idx], 1);

    char idxToStr[] = {idx + '0', '\0'};
    char * argv[] = {names[idx], idxToStr};
    philos[idx].pid = sys_createProcess(philosopher, 2, argv, 1, names[idx], fds);
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
        newLine();
        printing = 0;
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
    
    //sys_waitSem(mutex);
    if(i % 2 == 1){
        philos[i].waitingLeft = 1;
        sys_waitSem(forks[left(i)]);
        philos[i].waitingLeft = 0;
        philos[i].waitingRight = 1;
        sys_waitSem(forks[right(i)]);
        philos[i].waitingRight = 0;
    }else{
        philos[i].waitingRight = 1;
        sys_waitSem(forks[right(i)]);
        philos[i].waitingRight = 0;
        philos[i].waitingLeft = 1;
        sys_waitSem(forks[left(i)]);
        philos[i].waitingLeft = 0;
    }
    //sys_postSem(mutex);
}

void put_forks(int i){
    sys_postSem(forks[left(i)]);
    sys_postSem(forks[right(i)]);
}

void think(int i){
    philos[i].status = THINKING;
    sys_sleep(10);
}

uint8_t left(uint8_t i){
    return i;
}

uint8_t right(uint8_t i){
    return (i + 1) % philosophersAmount;
}

void eat(int i){
    philos[i].status = EATING;
    if(!printing){
        sys_postSem(printUpdateSem);
        printing = 1;
    }
    sys_sleep(10);
}