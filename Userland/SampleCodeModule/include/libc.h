#ifndef LIBC_H
#define LIBC_H

#include <color.h>
#include <stdarg.h>

#define MAX_BUFFER 256

void printf(const char *format, ...);
void printfColor(const char * format, Color font, Color background, ...);
void printError(const char *format, ...);
void putChar(char character);

void numToStr(uint64_t num, char * destination);

void printfd(int fd, const char * format, Color font, Color background, va_list args);
void write(int fd, char character, Color font, Color background);

void scanf(const char *format, void * variable);
char getChar(void);
void newLine(void);

int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);

#endif
