#ifndef LIBC_H
#define LIBC_H

#include <color.h>
#include <sys_calls.h>

void printf(const char *format, ...);
void printfColor(const char * format, Color font, Color background, ...);
void putCharColor(char character, Color font, Color background);
void putChar(char character);
void scanf(const char *format, void * variable);
char getChar(void);
void newLine(void);

void printError(const char *format, ...);
void putCharError(char c);

int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);

#endif
