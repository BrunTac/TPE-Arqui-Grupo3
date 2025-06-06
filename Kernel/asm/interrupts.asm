
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL int_20h

EXTERN getStackBase
EXTERN main
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysCallDispatcher
EXTERN scheduler
EXTERN timer_handler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro pushStateNoRAX 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateNoRAX 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1

	mov rdi, %1 ; pasaje de parametro
	
	; guardo el estado de mis registros en un arreglo de enteros de 64 bits

	push rax
	mov rax, [rsp+8]
	mov [registerStateExc], rax
	pop rax ; hasta aca es para guardar RIP

	push rax
	mov rax, [rsp+16]
	mov [registerStateExc+8], rax
	pop rax ; hasta aca es para guardar CS

	mov [registerStateExc+8*2], rax
	mov [registerStateExc+8*3], rbx
	mov [registerStateExc+8*4], rcx
	mov [registerStateExc+8*5], rdx
	mov [registerStateExc+8*6], rsp
	mov [registerStateExc+8*7], rbp
	mov [registerStateExc+8*8], rdi
	mov [registerStateExc+8*9], rsi
	mov [registerStateExc+8*10], r8
	mov [registerStateExc+8*11], r9
	mov [registerStateExc+8*12], r10
	mov [registerStateExc+8*13], r11
	mov [registerStateExc+8*14], r12
	mov [registerStateExc+8*15], r13
	mov [registerStateExc+8*16], r14
	mov [registerStateExc+8*17], r15

	push rax
	pushfq
	pop rax
	mov [registerStateExc+8*18], rax
	pop rax ; hasta aca para guardar rflags
	
	mov rsi, registerStateExc ; se lo paso como parametro a exceptionDispatcher

	call exceptionDispatcher

	call getStackBase
	mov rsp, rax
	call main

	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushState

	call timer_handler

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	mov al, 20h
	out 20h, al

	popState

	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Sys_calls
_irq60Handler:
	pushStateNoRAX

	push r9
	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax ; shift registers value to have the syscall id as first argument
	call sysCallDispatcher

	pop r9

	popStateNoRAX

	iretq

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

int_20h:
	int 20h 
	ret

SECTION .bss
	aux resq 1
	registerStateExc resq 19