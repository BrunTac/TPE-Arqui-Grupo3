#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>
#include <stddef.h>
#include <lib.h>
#include <time.h>
#include <soundDriver.h>
#include <processManager.h>
#include <semaphores.h>
#include <memoryManager.h>
#include "pipes.h"

typedef int64_t (*syscall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();
uint64_t saveRegisters();
uint64_t showRegisters();


static int64_t sys_time(char time[3][3]){
    uintToBase(getHours(), time[0], 16);
    uintToBase(getMinutes(), time[1], 16);
    uintToBase(getSeconds(), time[2], 16); 

    return 0;
}

static int64_t sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background) {
    printsInPos(str, x, y, font, background);

    return 0;
}

static int64_t sys_writeChar(char c, Color font, Color background, uint8_t isError) {
    uint64_t pid = getCurrentProcess();
    int8_t fd = isError? getErrorFd(pid) : getOutputFd(pid);
    if(fd < 0){
        return -1;
    }
    if(fd == STDOUT){
        print(c, font, background);
    }else if(fd == STDERR){
        print(c, RED, BLACK);
    }else{
        return pipe_write(fd, c);
    }

    return 0;
}

static int64_t sys_read(char * c) {
    uint64_t pid = getCurrentProcess();
    int8_t fd = getInputFd(pid);
    if(fd < 0){
        return -1;
    }
    if(fd == STDIN){
        *(c) = getKey();
    }else{
        int val = pipe_read(fd); 
        if (val < 0){
            return -1;
        }
        *(c) = (char) val;
    }
    return 0;
}

static int64_t sys_clear(){
    clear();

    return 0;
}

static int64_t sys_saveRegisters(){
    saveRegisters();

    return 0;
}

static int64_t sys_drawSquare(Color color, int x, int y){
    drawSquare(color, x , y);

    return 0;
}

static int64_t sys_sleep(int ticks) {
    sleep(ticks);

    return 0;
}

static int64_t sys_beep(uint32_t frequency, int ticks) {
    beep(frequency, ticks);

    return 0;
}

static int64_t sys_changeFont(int size) {
    updateSize(size);

    return 0;
}
 
static int64_t sys_scrHeight(int* ans){
    *ans = getHeight();

    return 0;
}

static int64_t sys_scrWidth(int* ans){
    *ans = getWidth();

    return 0;
}

static int64_t sys_readLastPressed(char * c) {
    *(c) = getLastPressed();
    return 0;
}

static int64_t sys_ticksElapsed(int * ans){
    *ans = ticks_elapsed();
    return 0;
}

static int64_t sys_getFontWidth(int * ans){
    *ans = getFontWidth();
    return 0;
}

static int64_t sys_showRegisters() {
    showRegisters();
    return 0;
}

static int64_t sys_clearLastPressed() {
    clearLastPressed();
    return 0;
}

static int64_t sys_waitpid(uint64_t pid) {
    waitpid(pid);
    return pid;
}

static int64_t sys_createProcess(function fn, int argc, char * argv[], int priority, uint8_t fds[FD_AMOUNT], uint8_t isBackground) {
    return createProcess(fn, argc, argv, priority, fds, isBackground);
}

static int64_t sys_exitProcess() {
    uint64_t pid = getCurrentProcess();
    exitProcess(pid);
    return 0;
}

static int64_t sys_openSem(const char * name, uint64_t value) {
    return sem_open(name, value);
}

static int64_t sys_waitSem(uint8_t sem) {
    sem_wait(sem);
    return 0;
}

static int64_t sys_postSem(uint8_t sem) {
    sem_post(sem);
    return 0;
}

static int64_t sys_closeSem(uint8_t sem) {
    sem_close(sem);
    return 0;
}

static int64_t sys_getProcessInfo(ProcessInfo * buffer){
    return listProcesses(buffer);
}

static int64_t sys_getPid(){
    return getCurrentProcess();
}

static int64_t sys_pipeOpen(const char * name){
    return pipe_open(name);
}

static int64_t sys_pipeClose(uint8_t pipeId){
    pipe_close(pipeId);
    return 0;
}

static int64_t sys_changePriority(uint64_t pid, uint8_t priority){
    return changePriority(pid, priority);
}

static int64_t sys_killProcess(uint64_t pid){
    return exitProcess(pid);
}

static void * sys_malloc(size_t size) {
    return malloc(size);
}

static int64_t sys_free(void *memToFree) {
    free(memToFree);
    return 0;
}

static void * sys_memset(void * destination, int32_t c, uint64_t length) {
    return memset(destination, c, length);
}

static int64_t sys_viewmem() {
    viewmem();
    return 0;
}

static int64_t sys_blockProcess(uint64_t pid){
    blockProcess(pid);
    return 0;
}

static int64_t sys_unblockProcess(uint64_t pid){
    unblockProcess(pid);
    return 0;
}

static int64_t sys_getProcessStatus(uint64_t pid){
    return getStatus(pid);
}

static void sys_yield(){
    yieldProcess(getCurrentProcess());
}

syscall syscallTable[] = {
    NULL, (syscall)sys_writeInPos, (syscall)sys_time, (syscall)sys_read, (syscall)sys_writeChar, 
    (syscall)sys_clear, (syscall)sys_saveRegisters, (syscall)sys_drawSquare, (syscall)sys_scrHeight, (syscall)sys_scrWidth,
    (syscall)sys_sleep, (syscall)sys_beep, (syscall)sys_readLastPressed, (syscall)sys_ticksElapsed, (syscall)sys_changeFont,
    (syscall)sys_getFontWidth, (syscall)sys_showRegisters, (syscall)sys_clearLastPressed, (syscall)sys_createProcess, (syscall)sys_exitProcess,
    (syscall)sys_waitpid, (syscall)sys_openSem, (syscall)sys_waitSem, (syscall)sys_postSem, (syscall)sys_closeSem, (syscall)sys_getProcessInfo,
    (syscall)sys_getPid, (syscall)sys_pipeOpen, (syscall)sys_pipeClose, (syscall)sys_changePriority, (syscall)sys_killProcess,
    (syscall)sys_malloc, (syscall)sys_free, (syscall)sys_memset, (syscall)sys_viewmem, (syscall)sys_blockProcess, 
    (syscall)sys_unblockProcess, (syscall)sys_getProcessStatus, (syscall)sys_yield
};

uint64_t sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {

    syscall handler;
    if (id < (sizeof(syscallTable) / sizeof(syscallTable[0]))){
        handler = syscallTable[id];
        return handler(arg1, arg2, arg3, arg4, arg5, arg6);
    }
    return -1;
}
