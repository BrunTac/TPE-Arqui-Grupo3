GLOBAL cpuVendor
GLOBAL getHs
GLOBAL getMins
GLOBAL getSecs
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
	
