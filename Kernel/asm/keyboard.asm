
GLOBAL keyboard_read

section .text


keyboard_read:
	
	mov dx, 0x60
	in al, dx
	movxz rax, al
	ret

