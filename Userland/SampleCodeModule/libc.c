#include <libc.h>
#include <stdarg.h>
#include <sys_calls.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define MAX_BUFFER 256 // esto seria el maximo de un string que le puedo pasar a scanf?



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
    return;
}

void putchar(char character){
    sys_write_char(STDOUT, character, WHITE, BLACK);
}

char getchar(){
    return sys_read(STDIN);
}

void newLine(){
    sys_write_char(STDOUT, '\n', WHITE, BLACK);
}

void printfColor(const char * format, Color color, ...){
    va_list args;
    va_start(args, color);

    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int num = va_arg(args, int);
                if (num < 0) {
                    putcharColor('-', color);
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
                    putcharColor(buffer[--i], color);
                }
            } else if (*ptr == 's') {
                char *str = va_arg(args, char*);
                while (*str) {
                    putcharColor(*str++, color);
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

void putcharColor(char character, Color font, Color background){
    sys_write_char(STDOUT, character, font, background);
}   

 int strcmp(const char * s1, const char * s2){

	int r1 = 0;
	int r2 = 0;

	while(s1[r1] != '\0' && s2[r2] != '\0'){
		if(s1[r1] != s2[r2]){
			return s1[r1] - s2[r2];
		}
		r1++;
		r2++;
	}

	return s1[r1] - s2[r2];
	

}

 void strcpy(char * destination, const char * source){

	int w = 0;

	while(source[w] != '\0'){
		destination[w] = source[w];
		w++;
	}

	destination[w] = '\0';
 }
