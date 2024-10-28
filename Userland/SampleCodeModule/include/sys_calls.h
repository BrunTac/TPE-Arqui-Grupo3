#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>

void sys_write(int fd, char * c);
void sys_write_color(int fd, char * c, Color color);
char sys_read(int fd);
void sys_clear();
void sys_show_registers();

#endif