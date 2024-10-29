section .text

GLOBAL opcodeError

opcodeError:
    db 0xFF, 0xFF, 0xFF
    ret