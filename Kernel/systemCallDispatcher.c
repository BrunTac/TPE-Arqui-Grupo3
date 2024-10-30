#include <videoDriver.h>
#include <keyboard.h>
#include <stdint.h>
#include <lib.h>


#define STDIN 0
#define STDOUT 1
#define STDERR 2

uint64_t * getRegisters();
uint64_t getSeconds();
uint64_t getMinutes();
uint64_t getHours();


extern Color WHITE;
extern Color BLACK;
extern Color RED;

static void sys_time(char time[3][3]){
    uintToBase(getHours(), time[0], 16);
    uintToBase(getMinutes(), time[1], 16);
    uintToBase(getSeconds(), time[2], 16); 
}



static void sys_write(int fd, char c, Color font, Color background) {
    if(fd == 1){
        print(c, font, background);
    }else if(fd == 2){
        print(c, RED, BLACK);
    }
    
}

static void sys_read(int fd, char * c) {

    *(c) = getKey();

}

static void sys_clear(){

    clear();

}

static void sys_printRegisters(){

    printRegisters();

}

static void sys_drawSquare(Color color, int x, int y){

    drawSquare(color, x , y);

}

void * sysCallDispatcher(uint64_t id, uint64_t arg1, uint64_t arg2, uint64_t arg3) {

    switch(id) {
        case 2:
            sys_time((char (*)[3])arg1);
            break ;
        case 3:
            sys_read((int) arg1, (char *) arg2);
            break ;
        case 4:
            sys_write((int) arg1,(char) arg2, WHITE, BLACK);
            break ;
        case 5:
            sys_clear();
            break ;
        case 6:
            sys_printRegisters();
            break ;
        case 7:
            sys_drawSquare((Color) arg1, (int)arg2, (int)arg3)
            break ;
        case 8:
            sys_scrHeight();
            break ;
        case 9:
            sys_scrWidth();
            break ;

    }
    return 0;
}
