global _start

section .text
_start:
    mov ax, 1

    mov rax, 60 ; exit(10)
    mov rdi,10
    syscall