#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H_

#include <stdint.h>

#define MAX_SEMAPHORES 20

void sem_post(uint8_t sem);
void sem_wait(uint8_t sem);
uint8_t sem_open(const char * name, uint64_t value);
void sem_close(uint8_t sem);
uint8_t sem_isValid(uint8_t sem);

#endif