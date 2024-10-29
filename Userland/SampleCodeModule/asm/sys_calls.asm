GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_clear
GLOBAL sys_show_registers

section .text

sys_write:
    mov rax, 4
    int 80h
    ret


sys_read:
    mov rax, 3
    int 80h

    ret

sys_clear:
    ret

sys_show_registers:
    ret


