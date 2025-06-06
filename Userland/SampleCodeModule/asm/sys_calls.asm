GLOBAL sys_time
GLOBAL sys_read
GLOBAL sys_writeInPos
GLOBAL sys_writeChar
GLOBAL sys_clear
GLOBAL sys_saveRegisters
GLOBAL sys_scrHeight
GLOBAL sys_scrWidth
GLOBAL sys_sleep
GLOBAL sys_beep
GLOBAL sys_drawSquare
GLOBAL sys_changeFont
GLOBAL sys_readLastPressed
GLOBAL sys_ticksElapsed
GLOBAL sys_getFontWidth
GLOBAL sys_showRegisters
GLOBAL sys_clearLastPressed
GLOBAL sys_createProcess
GLOBAL sys_exitProcess
GLOBAL sys_waitpid
GLOBAL sys_openSem
GLOBAL sys_waitSem
GLOBAL sys_postSem
GLOBAL sys_closeSem

section .text

sys_writeInPos:
    mov rax, 1
    int 80h
    ret

sys_time:
    mov rax, 2
    int 80h
    ret

sys_read:
    mov rax, 3
    int 80h
    ret

sys_writeChar:
    mov rax, 4
    int 80h
    ret

sys_clear:
    mov rax, 5
    int 80h
    ret

sys_saveRegisters:
    mov rax, 6
    int 80h
    ret

sys_drawSquare:
    mov rax, 7
    int 80h
    ret    

sys_scrHeight:
    mov rax, 8
    int 80h
    ret

sys_scrWidth:
    mov rax, 9
    int 80h
    ret

sys_sleep:
    mov rax, 10
    int 80h
    ret

sys_beep:
    mov rax, 11
    int 80h
    ret

sys_readLastPressed:
    mov rax, 12
    int 80h
    ret

sys_ticksElapsed:
    mov rax, 13
    int 80h
    ret

sys_changeFont:
    mov rax, 14
    int 80h
    ret

sys_getFontWidth:
    mov rax, 15
    int 80h
    ret

sys_showRegisters:
    mov rax, 16
    int 80h
    ret

sys_clearLastPressed:
    mov rax, 17
    int 80h
    ret

sys_createProcess:
    mov rax, 18
    int 80h
    ret

sys_exitProcess:
    mov rax, 19
    int 80h
    ret

sys_waitpid:
    mov rax, 20
    int 80h
    ret

sys_openSem:
    mov rax, 21
    int 80h
    ret

sys_waitSem:
    mov rax, 22
    int 80h
    ret

sys_postSem:
    mov rax, 23
    int 80h
    ret

sys_closeSem:
    mov rax, 24
    int 80h
    ret
