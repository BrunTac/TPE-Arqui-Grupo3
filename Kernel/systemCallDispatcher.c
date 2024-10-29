#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern Color WHITE;
extern Color BLACK;

static void sys_write(int fd, char c, Color font, Color background) {

    print(c, font, background);

}

static void sys_read(int fd, char * c) {

    *(c) = getKey();

}

static void sys_clear(){

    clear();

}

uint64_t sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2) {

    switch(id) {
        case 3:
            sys_read((int) arg1, (char *) arg2);
            return 3;
        case 4:
            sys_write((int) arg1,(char) arg2, WHITE, BLACK);
            return 4;
        case 5:
            sys_clear();
            return 5;    
    }
    return 0;
}
