GLOBAL tryLock
GLOBAL unlock

tryLock:
  mov rax, 0
  mov al, 1
  xchg al, [rdi]
  cmp al, 0
  jne tryLock
  ret


unlock:
    mov byte [rdi], 0
    ret