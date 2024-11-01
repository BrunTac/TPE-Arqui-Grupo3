#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

typedef uint32_t Color;

enum{
    WHITE = 0xFFFFFF,
    BLACK = 0x000000,
    RED = 0xFF0000
};

void putPixel(Color color, uint64_t x, uint64_t y);
void putCharInPos(unsigned char c, int x, int y, Color fgcolor, Color bgcolor);
void putChar(unsigned char c, Color fgcolor, Color bgcolor);
void print(const char c, Color fgcolor, Color bgcolor);
void prints(const char *str, Color fnt, Color bgd);
void printsInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background);
void printHex(const uint64_t num, Color fnt, Color bgd);
void scrollUp();
uint32_t* getPixelPtr(uint16_t x, uint16_t y);
void print_newline();
void print_backspace(Color fnt, Color bgd);
void printcheck();
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void clear();
void drawSquare(Color color, int x, int y);
uint64_t getHeight();
uint64_t getWidth();

#endif
