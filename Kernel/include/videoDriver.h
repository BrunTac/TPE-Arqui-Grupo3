#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void putChar(unsigned char c, int x, int y, int fgcolor, int bgcolor);
void print(const char * word, int x, int y, int fgcolor, int bgcolor);


#endif
