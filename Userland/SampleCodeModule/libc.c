#include <libc.h>
#include <sys_calls.h>

void printf(const char *format, ...){
    va_list args;
    va_start(args, format);

    print(format, WHITE, BLACK, args, 0);
    va_end(args);
    return;
}

void printfColor(const char * format, Color font, Color background, ...){
    va_list args;
    va_start(args, background);

    print(format, font, background, args, 0);

    va_end(args);
}

void printError(const char *format, ...){
    va_list args;
    va_start(args, format);

    print(format, WHITE, BLACK, args, 1);

    va_end(args);
}

void putChar(char c){
    write(c, WHITE, BLACK, 0);
}

void print(const char * format, Color font, Color background, va_list args, uint8_t isError){
   
    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int num = va_arg(args, int);
                if (num < 0) {
                    write('-', font, background, isError);
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
                    write(buffer[--i], font, background, isError);
                }
            } else if (*ptr == 's') {
                char *str = va_arg(args, char*);
                while (*str) {
                    write(*str++, font, background, isError);
                }
            } else if (*ptr == 'n'){
                newLine();
            }
        } else {
            write(*ptr, font, background, isError);
        }
    }
}

void write(char character, Color font, Color background, uint8_t isError){
    sys_writeChar(character, font, background, isError);
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
        destination[j] = destination[i - j - 1];
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
        sys_read(&c);
    return c;
}

void newLine(){
    sys_writeChar('\n', WHITE, BLACK, 0);
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


uint64_t strlen(const char *str) {
    uint64_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void tabPressed(int * ticks) {
    sys_ticksElapsed(ticks);
    sys_beep(700, 1);
    sys_beep(300, 1);
}

int isSpace(char c){
    return c == ' ' || c == '\t';
}

int isVocal(char c){
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}
 int atoi(const char *str) {
    int result = 0;
    int sign = 1;

    while (*str == ' ' || *str == '\t') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}
