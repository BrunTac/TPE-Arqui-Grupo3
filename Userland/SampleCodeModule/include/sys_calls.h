#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>

void sys_time(char destiny[3][3]);
void sys_read(int fd, char * c);
void sys_write(int fd, char c, Color font, Color background);
void sys_clear();
void sys_show_registers();
void sys_drawSquare(Color color, int x, int y);
int sys_scrWidth();
int sys_scrHeight();
void sys_beep();

#endif