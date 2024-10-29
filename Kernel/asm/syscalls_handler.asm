section .text

EXTERN print
EXTERN getKey
EXTERN printcheck

GLOBAL syscalls_handler

syscalls_handler:
    mov rdi, r9
    
    cmp rax, 3
    je sys_read
    cmp rax, 4
    je sys_write
    
    ret


sys_read:
    cmp rdi, 0
    je .stdin

    ret

.stdin:
    call getKey
    ret

sys_write:
    pushState

    cmp rdi, 1
    je .stdout
    cmp rdi, 2
    je .stderr

    jmp .end


.stdout:
    
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    
    call print

    jmp .end

.stderr:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    call print

    jmp .end

.end:
    popState
    ret
