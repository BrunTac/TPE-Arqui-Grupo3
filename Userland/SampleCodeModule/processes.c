#include "processes.h"
#include "libc.h"
#include "sys_calls.h"
#include <stdint.h>

void loop(){
    int pid = sys_getPid();
    while(1){
        printf("This is my pid: %d\n", pid);
        sys_sleep(20);
    }
}

void ps() {
    ProcessInfo processes[MAX_PROCESSES];
    int count = sys_getProcessInfo(processes);

    for (int i = 0; i < count; i++) {
        printf("PID: %d | PPID: %d | PRI: %d | STATE: %s | NAME: %s | RSP: %d\n",
               processes[i].pid,
               processes[i].ppid,
               processes[i].priority,
               processes[i].status  == 0 ? "READY" : (processes[i].priority == 1 ? "BLOCKED" : "EXITED"),
               processes[i].name,
               processes[i].rsp);
    }
}

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
                line[charsInline++] = c;
            }
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