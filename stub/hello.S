BITS 64
global _start

section .text
_start:
    lea     rbx, [rel _start]
    push    rdx

    mov     rax, 1
    mov     rdi, 1
    lea     rsi, [rbx + (message - _start)]
    mov     rdx, 14
    syscall

    pop     rdx

    mov     rax, [rel segment]
    add     rax, rbx
    mov     rcx, [rel size]
    lea     rsi, [rel key]
    mov     r10, [rel key_size]
    xor     r9, r9

decryption_loop:
    test    rcx, rcx
    jz      decryption_done
    mov     r11b, [rsi + r9]
    xor     byte [rax], r11b
    inc     rax
    inc     r9
    cmp     r9, r10
    jl      no_wrap
    xor     r9, r9

no_wrap:
    dec     rcx
    jmp     decryption_loop

decryption_done:
    mov     rax, [rel segment]
    add     rax, rbx
    

decompression_done:
    mov     rax, [rel oep]
    add     rax, rbx
    jmp     rax

oep:        dq 0x4141414141414141
segment:    dq 0x4242424242424242
size:       dq 0x4343434343434343
key:        times 32 db 0x44
key_size:   dq 0x4545454545454545
oss:        dq 0x4646464646464646

message: db "....WOODY....", 10
