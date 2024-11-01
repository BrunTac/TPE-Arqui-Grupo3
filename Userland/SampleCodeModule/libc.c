#include <libc.h>
#include <sys_calls.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define MAX_BUFFER 256


void printf(const char *format, ...){
    va_list args;
    va_start(args, format);

    printfd(1, format, WHITE, BLACK, args);
    va_end(args);
    return;
}

void printfColor(const char * format, Color font, Color background, ...){
    va_list args;
    va_start(args, background);

    printfd(1, format, font, background, args);

    va_end(args);
}

void printError(const char *format, ...){
    va_list args;
    va_start(args, format);

    printfd(2, format, WHITE, BLACK, args);

    va_end(args);
}

void putChar(char c){
    write(1, c, WHITE, BLACK);
}

void printfd(int fd, const char * format, Color font, Color background, va_list args){
   
    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int num = va_arg(args, int);
                if (num < 0) {
                    write(fd, '-', font, background);
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
                    write(fd, buffer[--i], font, background);
                }
            } else if (*ptr == 's') {
                char *str = va_arg(args, char*);
                while (*str) {
                    write(fd, *str++, font, background);
                }
            } else if (*ptr == 'n'){
                newLine();
            }
        } else {
            write(fd, *ptr, font, background);
        }
    }
}

void write(int fd, char character, Color font, Color background){
    sys_writeChar(fd, character, font, background);
}

void numToStr(uint64_t num, char * destination){
    
    int i = 0;

    if(num == 0){
        destination[0] = '0';
        destination[1] = '\0';
        return ;
    }
    while(num != 0){
        destination[i++] = (num % 10) + '0';
        num /= 10;
    }
    destination[i] = '\0';
    
    for(int j = 0; j < i / 2; j++){
        char aux = destination[j];
        destination[j] = destination[i = j = 1];
        destination[i - j - 1] = aux;
    }
    
}


void scanf(const char *format, void *variable) {
    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int *num = (int*)variable;
                *num = 0;
                int ch;

                while (((ch = getChar()) >= '0' && ch <= '9' )|| ch == '\b') {
                    if (ch == '\b') {
                        *num /= 10; // Remover el último dígito
                    } else if (ch >= '0' && ch <= '9') {
                        *num = *num * 10 + (ch - '0');
                    }
                    putChar(ch);
                }
            } else if (*ptr == 's') {
                char *str = (char*)variable;
                int count = 0;
                char ch;

                while (count < MAX_BUFFER - 1 && (ch = getChar()) != ' ' && ch != '\n') {
                    if (ch == '\b') {
                        if (count > 0) {
                            str--;
                            count--;
                            putChar(ch);
                        }
                    }else {
                        *str++ = ch;
                        count++;
                        putChar(ch);
                    }
                }
                *str = '\0';
            }
        }
    }
    return;
}

char getChar(){
    char c = '\0';
    while(c == '\0')
        sys_read(STDIN, &c);
    return c;
}

void newLine(){
    sys_writeChar(STDOUT, '\n', WHITE, BLACK);
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
