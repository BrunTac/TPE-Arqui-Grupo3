#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#define FD_AMOUNT 3
#define MAX_BUFFER 256

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef void (*function)(uint64_t argc, char ** argv);

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void printRegisters(uint64_t * registers);
char * cpuVendor(char *result);
int strcmp(const char * s1, const char * s2);
void strcpy(char * destination, const char * source);
uint64_t strlen(const char *str);

#endif