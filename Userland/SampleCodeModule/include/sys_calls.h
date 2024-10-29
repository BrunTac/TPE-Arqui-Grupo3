#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>

void sys_write(int fd, char c, Color font, Color background);
void sys_read(int fd, char * c);
void sys_clear();
void sys_show_registers();

#endif