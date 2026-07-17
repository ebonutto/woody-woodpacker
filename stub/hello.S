BITS 64
global _start

section .text
_start:
    lea     rbx, [rel _start]

    mov     rax, 1
    mov     rdi, 1
    lea     rsi, [rbx + (message - _start)]
    mov     rdx, 14
    syscall

    mov     rax, [rel oep]
    jmp     rax

oep:     dq 0x4242424242424242
message: db "Hello, World!", 10
