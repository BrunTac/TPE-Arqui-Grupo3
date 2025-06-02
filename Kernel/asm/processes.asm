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
    
    push 0x00
    push 0x00
    push 0x00
    push rcx
    push rdx
	push rdi
	push rsi
	push 0x00
	push 0x00
	push 0x00
	push 0x00
	push 0x00
	push 0x00
	push 0x00
	push 0x00
    
    mov rax, rsp
    
    mov rsp, rbp
    pop rbp
    ret
