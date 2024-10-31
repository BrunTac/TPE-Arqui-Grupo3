GLOBAL sys_time
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_show_registers
GLOBAL sys_scrHeight
GLOBAL sys_scrWidth
GLOBAL sys_sleep
GLOBAL sys_beep
GLOBAL sys_drawSquare

section .text

sys_time:
    mov rax, 2
    int 80h
    ret

sys_read:
    mov rax, 3
    int 80h
    ret

sys_write:
    mov rax, 4
    int 80h
    ret

sys_clear:
    mov rax, 5
    int 80h
    ret

sys_show_registers:
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
