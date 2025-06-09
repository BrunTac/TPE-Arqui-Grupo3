#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define BUFFER_SIZE 256

void keyboard_handler();
void bufferKey(char key);
char getKey();
void emptyBuffer();
char getLastPressed();
void clearLastPressed();
void checkShift(uint8_t scanCode);
void checkCtrl(uint8_t scanCode);

#endif