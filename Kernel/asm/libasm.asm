GLOBAL cpuVendor
GLOBAL getHs
GLOBAL getMins
GLOBAL getSecs
GLOBAL getRegisters
section .text
	
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


getHs
	mov al, 0x04
	out 70h, al
	in al, 71h
	movzx rax, al
	ret

getMins
	mov al, 0x02
	out 70h, al
	in al, 71h
	movzx rax, al
	ret
getSecs
	mov al, 0
	out 70h, al
	in al, 71h
	movzx rax, al
	ret
	
getRegisters
		saveRegisters
		mov rax, registerState
		ret 



%macro saveRegisters 0
	push rax
	call rip
	rip:
	pop rax
	mov [registerState], rax
	pop rax ; hasta aca es para guardar RIP

	mov [registerState+8], rax
	mov [registerState+16], rbx
	mov [registerState+24], rcx
	mov [registerState+32], rdx
	mov [registerState+40], rbp
	mov [registerState+48], rsi
	mov [registerState+56], rdi
	mov [registerState+64], rsp
	mov [registerState+72], r8
	mov [registerState+80], r9
	mov [registerState+88], r10
	mov [registerState+96], r11
	mov [registerState+104], r12
	mov [registerState+112], r13
	mov [registerState+120], r14
	mov [registerState+128], r15

	push rax
	pushfq
	pop rax
	mov [registerState+136], rax
	pop rax
%endmacro

SECTION .bss
	registerState resq 18