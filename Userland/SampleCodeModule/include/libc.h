#ifndef LIBC_H
#define LIBC_H

#include <color.h>
#include <sys_calls.h>

void printf(const char *format, ...);
void scanf(const char *format, void * variable);
void putChar(char character);
char getChar(void);
void newLine(void);
void printfColor(const char * format, Color font, Color background, ...);
void putCharColor(char character, Color font, Color background);

int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);

#endif
