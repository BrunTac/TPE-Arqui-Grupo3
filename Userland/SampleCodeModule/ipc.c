#include "ipc.h"
#include "libc.h"

void cat(){
    int charsInline = 0;
    char c;
    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                charsInline--;
                putChar(c);
            }
        }else{
            if(c == '\n'){
                charsInline = 0;
            }else{
                charsInline++;
            }
            putChar(c);
        }
    }
}

void wc(){
    char line[MAX_BUFFER];
    
    int lines = 0;
    int words = 0;
    int chars = 0;
    int charsInline = 0;
    char c;

    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                chars--;
                charsInline--;
                if(!isSpace(line[charsInline]) && (charsInline == 0 || isSpace(line[charsInline - 1]))){
                    words--;
                }
                putChar(c);
            }
        }else{
            if(c == '\n'){
                lines++;
                charsInline = 0;
            }else{
                if(!isSpace(c) && (charsInline == 0 || isSpace(line[charsInline - 1]))){
                    words++;
                }
                chars++;
                charsInline++;
            }
            putChar(c);
        }
    }
    if(chars > 0){
        lines++;
    }
    printf("%nlines: %d    words: %d    chars: %d%n", lines, words, chars);
}

void filter(){
    int charsInline = 0;
    char c;
    while((c = getChar()) != '\0'){
        if(c == '\b'){
            if(charsInline > 0){
                charsInline--;
                putChar(c);
            }
        }else{
            if(c == '\n'){
                charsInline = 0;
            }else{
                charsInline++;
            }
            if(!isVocal(c)){
                putChar(c);
            }
        }
    }
}