BITS 64

global _start

_start:
    call    .next           ; push rip sur la stack
.next:
    pop     rbx             ; rbx = adresse de .next à runtime

    mov     rax, 1
    mov     rdi, 1
    lea     rsi, [rbx + message - .next]    ; adresse relative
    mov     rdx, 13
    syscall

    mov     rax, 60
    xor     rdi, rdi
    syscall

message: db "Hello, World", 10