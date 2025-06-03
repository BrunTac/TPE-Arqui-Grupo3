#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>
#include <libc.h>

void sys_time(char destiny[3][3]);
void sys_read(int fd, char * c);
void sys_writeChar(int fd, char c, Color font, Color background);
void sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background);
void sys_clear();
void sys_saveRegisters();
void sys_drawSquare(Color color, int x, int y);
void sys_scrWidth(int * ans);
void sys_scrHeight(int * ans);
void sys_sleep(int ticks);
void sys_beep(uint32_t frequency, int ticks);
void sys_changeFont(int size);
void sys_readLastPressed(int fd, char * c);
void sys_ticksElapsed(int * ans);
void sys_getFontWidth(int * ans);
void sys_showRegisters();
void sys_clearLastPressed();
uint64_t sys_createProcess(function fn, int argc, char * argv[], int priority);
void sys_exitProcess();
void sys_waitpid(uint64_t pid);

#endif