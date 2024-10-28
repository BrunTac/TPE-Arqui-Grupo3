#ifndef LIBC_H
#define LIBC_H

void printf(const char *format, ...);
void scanf(const char *format, void * variable);
void putchar(int character);
int getchar(void);
void newLine(void);
//void printfColor(const char * format, Color color, ...);
//void putcharColor(int character, Color color);

int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);

#endif
