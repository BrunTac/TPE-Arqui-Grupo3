#ifndef SYS_CALLS_H
#define SYS_CALLS_H

#include <color.h>
#include <stdint.h>

uint64_t sys_write(int fd, char c, Color font, Color background);
char sys_read(int fd);
void sys_clear();
void sys_show_registers();

#endif