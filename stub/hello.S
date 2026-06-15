BITS 64

global _start

section .text
_start:
	call	.next
.next:
	pop	rbx

	mov	rax, 1				; write()
	mov	rdi, 1
	lea 	rsi, [rbx + (message - .next)]
	mov	rdx, 14
	syscall

	mov	rax, 0xFFFFFFFFFFFFFFFF		; OEP
	jmp	rax

message: db "Hello, World!", 10
