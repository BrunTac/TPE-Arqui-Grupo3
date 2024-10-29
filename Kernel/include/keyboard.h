#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define BUFFER_SIZE 256

void keyboard_handler();
void bufferKey(char key);
char getKey();
void emptyBuffer();

#endif