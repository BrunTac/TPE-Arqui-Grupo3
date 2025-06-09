#ifndef _PIPES_H_
#define _PIPES_H_

#include <stdint.h>

#define MAX_PIPES 20

int64_t pipe_open(const char * name);
int64_t pipe_write(uint8_t pipeId, char c);
int pipe_read(uint8_t pipeId);
void pipe_close(uint8_t pipeId);
uint8_t pipe_isValid(uint8_t pipeId);

#endif