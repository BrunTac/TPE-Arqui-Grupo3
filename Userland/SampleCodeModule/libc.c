#include <libc.h>
#include <stdarg.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define MAX_BUFFER 100 // esto seria el maximo de un string que le puedo pasar a scanf?

// FALTA: incluir Color, printfcolor y putcharcolor. strcpy y strcmp
// en las que llame a sys_call, esta a determinar. depende de como las hagamos pero es una idea

void printf(const char *format, ...){
    va_list args;
    va_start(args, format);

    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int num = va_arg(args, int);
                if (num < 0) {
                    putchar('-');
                    num = -num;
                }
                // Convert int to string and print
                char buffer[10];
                int i = 0;
                do {
                    buffer[i++] = '0' + (num % 10);
                    num /= 10;
                } while (num > 0);
                while (i > 0) {
                    putchar(buffer[--i]);
                }
            } else if (*ptr == 's') {
                char *str = va_arg(args, char*);
                while (*str) {
                    putchar(*str++);
                }
            } else if (*ptr == 'n'){
                newLine();
            }
        } else {
            putchar(*ptr);
        }
    }
    va_end(args);
    return;
}


void scanf(const char *format, void * variable){
    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int *num = (int*)variable;
                *num = 0;
                int ch;
                while ((ch = getchar()) >= '0' && ch <= '9') {
                    *num = *num * 10 + (ch - '0');
                }
            } else if (*ptr == 's') {
                char *str = (char*)variable;
                int count = 0;
                char ch;
                while (count < MAX_BUFFER - 1 && (ch = getchar()) != ' ' && ch != '\n') { // que onda los checkeos de newline y space
                    *str++ = ch;
                    count++;
                }
                *str = '\0';
            }
        }
    }
    va_end(args);
    return;
}

void putchar(int character){
    sys_write(STDOUT, character);
}

char getchar(){
    sys_read(STDIN);
}

void newLine(){
    sys_write(STDOUT, '\n');
}

// void printfColor(const char * format, void * variable, Color color){

// }

// void putcharColor(int character, Color color){

// }   

// int strcmp(const char * s1, const char * s2){

// }

// void strcpy(char * destination, const chat * source){
    
// }
