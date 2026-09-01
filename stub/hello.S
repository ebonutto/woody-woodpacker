BITS 64
global _start

section .text
_start:
    lea     rbx, [rel _start]
    push    rdx                      ; sauvegarde rdx avant de le détruire

    mov     rax, 1
    mov     rdi, 1
    lea     rsi, [rbx + (message - _start)]
    mov     rdx, 14
    syscall

    pop     rdx                      ; restaure rdx pour l'OEP

    mov     rax, [rel oep]
    add     rax, rbx
    jmp     rax

oep:     dq 0x4242424242424242
message: db "Hello, World!", 10