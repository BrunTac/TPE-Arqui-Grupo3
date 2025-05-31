GLOBAL setNewProcessStack

extern pushState

functionWrapper:
    call rdx
    mov rax, 19
    int 80h

setNewProcessStack:
    push rbp
    mov rbp, rsp
    
    mov rsp, rdx
    push 0x0
    push rdx
    push 0x202
    push 0x8
    push functionWrapper
    
    mov r15, rbp
    mov rbp, rdx
    mov rdx, rcx

    pushState

    mov rbp, r15
    mov rax, rsp
    
    mov rsp, rbp
    pop rbp
    ret
