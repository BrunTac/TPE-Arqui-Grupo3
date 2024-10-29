#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern Color WHITE;
extern Color BLACK;

static void sys_write(int fd, char c, Color font, Color background) {

    c = getKey();

}