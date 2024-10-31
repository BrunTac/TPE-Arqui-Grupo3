GLOBAL cpuVendor
GLOBAL getHours
GLOBAL getMinutes
GLOBAL getSeconds
GLOBAL getRegisters
GLOBAL showRegisters
GLOBAL inb
GLOBAL outb
section .text

EXTERN printRegistersCall
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


getHours:
	mov al, 0x04
	out 70h, al
	in al, 71h
	movzx rax, al
	ret

getMinutes:
	mov al, 0x02
	out 70h, al
	in al, 71h
	movzx rax, al
	ret

getSeconds:
	mov al, 0
	out 70h, al
	in al, 71h
	movzx rax, al
	ret

showRegisters:
	push rax
	mov rax, [rsp+8]
	mov [registerState], rax
	pop rax ; hasta aca es para guardar RIP

	push rax
	mov rax, [rsp+16]
	mov [registerState+8], rax
	pop rax ; hasta aca es para guardar CS

	mov [registerState+8*2], rax
	mov [registerState+8*3], rbx
	mov [registerState+8*4], rcx
	mov [registerState+8*5], rdx
	mov [registerState+8*6], rsp
	mov [registerState+8*7], rbp
	mov [registerState+8*8], rdi
	mov [registerState+8*9], rsi
	mov [registerState+8*10], r8
	mov [registerState+8*11], r9
	mov [registerState+8*12], r10
	mov [registerState+8*13], r11
	mov [registerState+8*14], r12
	mov [registerState+8*15], r13
	mov [registerState+8*16], r14
	mov [registerState+8*17], r15

	push rax
	pushfq
	pop rax
	mov [registerState+8*18], rax
	pop rax ; hasta aca para guardar rflags

	mov rdi, registerState
	call printRegistersCall

	ret

outb:
    mov dx, di            
    mov al, sil           
    out dx, al            
    ret

inb:
    mov dx, di            
    in al, dx             
    movzx rax, al         
    ret                   

SECTION .bss
	registerState resq 19