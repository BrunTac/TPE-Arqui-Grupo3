#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>
#include <lib.h>


#define STDIN 0
#define STDOUT 1
#define STDERR 2

uint64_t * getRegisters();
uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();


extern Color WHITE;
extern Color BLACK;

static int * sys_time(){
    char buffer[MAX_BUFFER];
    int time[3];
    time[0] = uintToBase(getHours(), buffer, 16);
    time[1] = uintToBase(getMinutes(), buffer, 16);
    time[2] = uintToBase(getSeconds(), buffer, 16);

    return time;
}

static void sys_write(int fd, char c, Color font, Color background) {

    print(c, font, background);

}

static void sys_read(int fd, char * c) {

    *(c) = getKey();

}

static void sys_clear(){

    clear();

}

static void sys_printRegisters(){

    printRegisters();

}

void * sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2) {

    switch(id) {
        case 2:
            return sys_time();
        case 3:
            sys_read((int) arg1, (char *) arg2);
            break ;
        case 4:
            sys_write((int) arg1,(char) arg2, WHITE, BLACK);
            break ;
        case 5:
            sys_clear();
            break ;
        case 6:
            sys_printRegisters();
            break ;
    }
    return 0;
}
