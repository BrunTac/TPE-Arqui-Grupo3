#ifndef LIBC_H
#define LIBC_H

#include <color.h>
#include <stdarg.h>

#define MAX_BUFFER 256
#define FD_AMOUNT 3

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef void (*function)(uint64_t argc, char ** argv);

void printf(const char *format, ...);
void printfColor(const char * format, Color font, Color background, ...);
void printError(const char *format, ...);
void putChar(char character);

void numToStr(uint64_t num, char * destination);
int atoi(const char *str);

void print(const char * format, Color font, Color background, va_list args, uint8_t isError);
void write(char character, Color font, Color background, uint8_t isError);

void scanf(const char *format, void * variable);
char getChar(void);
void newLine(void);
void printDashLine();

int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);
uint64_t strlen(const char *str);

void tabPressed(int * ticks);
int isSpace(char c);
int isVocal(char c);

#endif
