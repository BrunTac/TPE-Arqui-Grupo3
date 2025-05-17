GLOBAL setNewProcessStack

extern pushState

setNewProcessStack:
    push rdx
    push 0x202
    push 0x8
    push rcx
    pushState
    ret
