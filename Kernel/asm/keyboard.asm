section .text

GLOBAL readKey

readKey:
    in al, 60h
    ret