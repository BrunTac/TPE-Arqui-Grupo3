#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>
#include <libc.h>
#include <structs.h>
#include <stddef.h>

void sys_time(char destiny[3][3]);
void sys_read(char * c);
void sys_writeChar(char c, Color font, Color background, uint8_t isError);
void sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background);
void sys_clear();
void sys_saveRegisters();
void sys_drawSquare(Color color, int x, int y);
void sys_scrWidth(int * ans);
void sys_scrHeight(int * ans);
void sys_sleep(int ticks);
void sys_beep(uint32_t frequency, int ticks);
void sys_changeFont(int size);
void sys_readLastPressed(char * c);
void sys_ticksElapsed(int * ans);
void sys_getFontWidth(int * ans);
void sys_showRegisters();
void sys_clearLastPressed();
int64_t sys_createProcess(function fn, int argc, char * argv[], int priority, uint8_t fds[FD_AMOUNT], uint8_t isBackground);
void sys_exitProcess();
void sys_waitpid(uint64_t pid);
int8_t sys_openSem(const char * name, uint64_t value);
void sys_waitSem(uint8_t sem);
void sys_postSem(uint8_t sem);
void sys_closeSem(uint8_t sem);
int64_t sys_getProcessInfo(ProcessInfo * buffer);
int64_t sys_getPid();
int8_t sys_pipeOpen(const char * name);
void sys_pipeClose(uint8_t pipeId);
int64_t sys_changePriority(uint64_t pid, uint8_t priority);
int64_t sys_killProcess(uint64_t pid);
void *sys_malloc(size_t size);
int64_t sys_free(void *memToFree);
void *sys_memset(void * destination, int32_t c, uint64_t length);
int64_t sys_viewmem();
int64_t sys_getProcessStatus(uint64_t pid);
int64_t sys_unblockProcess(uint64_t pid);
int64_t sys_blockProcess(uint64_t pid);
void sys_yieldAll();

#endif