#include <keyboard.h>
#include <processManager.h>
#include <stdint.h>
#include "processManager.h"
#include "scheduler.h"

char buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;
int toBeRead = 0;
int readingIdx = 0;
int shiftOn = 0;
int ctrlOn = 0;
char lastPressed = '\0';
uint64_t waitingPid;

extern uint8_t readKey();
void checkCtrl(uint8_t scanCode);
void checkShift(uint8_t scanCode);

char scanCodeToAscii_shiftOff[0x60] = {
            0x00, 0x1B, '1', '2', '3', '4', '5', '6',
            '7', '8', '9', '0', '-', '=', '\b', 0x09,
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
            'o', 'p', '[', ']', '\n', 0x00, 'a', 's',
            'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
            '\'', '`', 0x00, '\\', 'z', 'x', 'c', 'v',
            'b', 'n', 'm', ',', '.', '/', 0x00, '*',
            0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, '7', '8', '9', '-', '4',
            '5', '6', '+', '1', '2', '3', '0', '.',
            0x00, 0x00, 0x00, 0x00, 0x00
};


char scanCodeToAscii_shiftOn[0x60] = {
            0x00, 0x1B, '!', '@', '#', '$', '%', '^',
            '&', '*', '(', ')', '_', '+', '\b', 0x09,
            'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
            'O', 'P', '{', '}', '\n', 0x00, 'A', 'S',
            'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
            '"', '~', 0x00, '|', 'Z', 'X', 'C', 'V',
            'B', 'N', 'M', '<', '>', '?', 0x00, '*',
            0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, '7', '8', '9', '-', '4',
            '5', '6', '+', '1', '2', '3', '0', '.',
             0x00, 0x00, 0x00, 0x00, 0x00
};

void bufferKey(char key) {
        buffer[(readingIdx + toBeRead) % BUFFER_SIZE] = key;
        if(toBeRead++ == 0){
                unblockProcess(waitingPid);
        }
}

void keyboard_handler() {
        uint8_t scanCode = readKey();
        char key;

        checkShift(scanCode);
        checkCtrl(scanCode);

        if(ctrlOn) {
                if(scanCode == 0x2E)
                        terminateForeground();
                return;
        }
        if(scanCode >= 0x80){
                return;
        }
        if(shiftOn == 1){
                key = scanCodeToAscii_shiftOn[scanCode];
        }else{
                key = scanCodeToAscii_shiftOff[scanCode];
        }
        if (key != 0x00){
                bufferKey(key);
                lastPressed = key;
        }
}

void checkShift(uint8_t scanCode) {
        if(scanCode == 0x2A || scanCode == 0x36) {
                shiftOn = 1;
                return;
        }

        if((scanCode == 0xAA || scanCode == 0xB6) && shiftOn) {
                shiftOn = 0;
                return;
        }
        return;
}

void checkCtrl(uint8_t scanCode) {
        if(scanCode == 0x1D) {
                ctrlOn = 1;
                return;
        }

        if(scanCode == 0x9D && ctrlOn) {
                ctrlOn = 0;
                return;
        }
        return;
}

char getKey() {
        while(toBeRead == 0){
                waitingPid = getCurrentProcess();
                blockProcess(waitingPid);
                yieldProcess(waitingPid);
        }
        toBeRead--;
        char c = buffer[readingIdx];
        readingIdx = (readingIdx + 1) % BUFFER_SIZE;
        return c;
}

void emptyBuffer(){
        toBeRead = 0;
}

char getLastPressed(){
        return lastPressed;
}

void clearLastPressed(){
        lastPressed = '\0';
}