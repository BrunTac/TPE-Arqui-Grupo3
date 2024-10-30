#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} Color;

extern Color WHITE;
extern Color BLACK;
extern Color RED;

void putPixel(Color color, uint64_t x, uint64_t y);
void putChar(unsigned char c, int x, int y, Color fgcolor, Color bgcolor);
void print(const char c, Color fgcolor, Color bgcolor);
void prints(const char *str, Color fnt, Color bgd);
void printHex(const uint64_t num, Color fnt, Color bgd);
void scrollUp();
uint32_t* getPixelPtr(uint16_t x, uint16_t y);
void print_newline();
void print_backspace(Color fnt, Color bgd);
void printcheck();
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void clear();
void drawSquare(Color color, int x, int y);
int getHeight();
int getWidth();

#endif
