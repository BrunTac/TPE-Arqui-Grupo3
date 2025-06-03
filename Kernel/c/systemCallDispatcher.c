#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>
#include <lib.h>
#include <time.h>
#include <soundDriver.h>
#include <processManager.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef uint64_t (*syscall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();
uint64_t saveRegisters();
uint64_t showRegisters();


static uint64_t sys_time(char time[3][3]){
    uintToBase(getHours(), time[0], 16);
    uintToBase(getMinutes(), time[1], 16);
    uintToBase(getSeconds(), time[2], 16); 

    return 0;
}

static uint64_t sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background) {
    printsInPos(str, x, y, font, background);

    return 0;
}

static uint64_t sys_writeChar(int fd, char c, Color font, Color background) {
    if(fd == 1){
        print(c, font, background);
    }else if(fd == 2){
        print(c, RED, BLACK);
    }

    return 0;
}

static uint64_t sys_read(int fd, char * c) {
    if(fd == 0){
         *(c) = getKey();
    }

    return 0;
}

static uint64_t sys_clear(){
    clear();

    return 0;
}

static uint64_t sys_saveRegisters(){
    saveRegisters();

    return 0;
}

static uint64_t sys_drawSquare(Color color, int x, int y){
    drawSquare(color, x , y);

    return 0;
}

static uint64_t sys_sleep(int ticks) {
    sleep(ticks);

    return 0;
}

static uint64_t sys_beep(uint32_t frequency, int ticks) {
    beep(frequency, ticks);

    return 0;
}

static uint64_t sys_changeFont(int size) {
    updateSize(size);

    return 0;
}
 
static uint64_t sys_scrHeight(int* ans){
    *ans = getHeight();

    return 0;
}

static uint64_t sys_scrWidth(int* ans){
    *ans = getWidth();

    return 0;
}

static uint64_t sys_readLastPressed(int fd, char * c) {
    if(fd == 0){
         *(c) = getLastPressed();
    }
    return 0;
}

static uint64_t sys_ticksElapsed(int * ans){
    *ans = ticks_elapsed();
    return 0;
}

static uint64_t sys_getFontWidth(int * ans){
    *ans = getFontWidth();
    return 0;
}

static uint64_t sys_showRegisters() {
    showRegisters();
    return 0;
}

static uint64_t sys_clearLastPressed() {
    clearLastPressed();
    return 0;
}

static uint64_t sys_waitpid(uint64_t pid) {
    waitpid(pid);
    return pid;
}

static uint64_t sys_createProcess(function fn, int argc, char * argv[], int priority) {
    return createProcess(fn, argc, argv, priority);
}

static uint64_t sys_exitProcess() {
    exitProcess();
    return 0;
}

syscall syscallTable[] = {
    NULL, (syscall)sys_writeInPos, (syscall)sys_time, (syscall)sys_read, (syscall)sys_writeChar, 
    (syscall)sys_clear, (syscall)sys_saveRegisters, (syscall)sys_drawSquare, (syscall)sys_scrHeight, (syscall)sys_scrWidth,
    (syscall)sys_sleep, (syscall)sys_beep, (syscall)sys_readLastPressed, (syscall)sys_ticksElapsed, (syscall)sys_changeFont,
    (syscall)sys_getFontWidth, (syscall)sys_showRegisters, (syscall)sys_clearLastPressed, (syscall)sys_createProcess, (syscall)sys_exitProcess,
    (syscall)sys_waitpid
};

uint64_t sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {

    syscall handler;
    if (id < (sizeof(syscallTable) / sizeof(syscallTable[0]))){
        handler = syscallTable[id];
        return handler(arg1, arg2, arg3, arg4, arg5);
    }
    return -1;
}
