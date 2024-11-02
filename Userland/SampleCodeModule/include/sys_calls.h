#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>

void sys_time(char destiny[3][3]);
void sys_read(int fd, char * c);
void sys_writeChar(int fd, char c, Color font, Color background);
void sys_writeInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background);
void sys_clear();
void sys_show_registers();
void sys_drawSquare(Color color, int x, int y);
void sys_scrWidth(int * ans);
void sys_scrHeight(int * ans);
void sys_sleep(int ticks);
void sys_beep(uint32_t frequency, int ticks);
void sys_changeFont(int size);
void sys_readLastPressed(int fd, char * c);
void sys_ticksElapsed(int * ans);
void sys_getFontWidth(int * ans);

#endif