#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>
#include <lib.h>
#include <time.h>
#include <soundDriver.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

uint64_t * showRegisters();
uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();


static void sys_time(char time[3][3]){
    uintToBase(getHours(), time[0], 16);
    uintToBase(getMinutes(), time[1], 16);
    uintToBase(getSeconds(), time[2], 16); 
}

static void sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background) {
    printsInPos(str, x, y, font, background);
}

static void sys_writeChar(int fd, char c, Color font, Color background) {
    if(fd == 1){
        print(c, font, background);
    }else if(fd == 2){
        print(c, RED, BLACK);
    }
}

static void sys_read(int fd, char * c) {
    if(fd == 0){
         *(c) = getKey();
    }
}

static void sys_clear(){

    clear();
}

static void sys_showRegisters(){

    showRegisters();
}

static void sys_drawSquare(Color color, int x, int y){

    drawSquare(color, x , y);
}

static void sys_sleep(int ticks) {

    sleep(ticks);
}

static void sys_beep() {

    beep();
}
 
static void sys_scrHeight(int* ans){

    *ans = getHeight();
}

static void sys_scrWidth(int* ans){

    *ans = getWidth();
}

static void sys_readLastPressed(int fd, char * c) {
    if(fd == 0){
         *(c) = getLastPressed();
    }
}

static void sys_ticksElapsed(int * ans){
    
    *ans = ticks_elapsed();
}

void sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {

    switch(id) {
        case 1:
            sys_writeInPos((const char *) arg1,(uint64_t) arg2, (uint64_t) arg3, (Color) arg4, (Color) arg5);
            break ;
        case 2:
            sys_time((char (*)[3])arg1);
            break ;
        case 3:
            sys_read((int) arg1, (char *) arg2);
            break ;
        case 4:
            sys_writeChar((int) arg1,(char) arg2, (Color) arg3, (Color) arg4);
            break ;
        case 5:
            sys_clear();
            break ;
        case 6:
            sys_showRegisters();
            break ;
        case 7:
            sys_drawSquare((Color) arg1, (int)arg2, (int)arg3);
            break ;
        case 8:
            return sys_scrHeight((int*) arg1);
        case 9:
            return sys_scrWidth((int*) arg1);
        case 10:
            sys_sleep((int) arg1);
            break ;
        case 11:
            sys_beep();
            break ;
        case 12:
            sys_readLastPressed((int) arg1, (char *) arg2);
            break ;
        case 13:
            sys_ticksElapsed((int *) arg1);
            break ;
    }
}
