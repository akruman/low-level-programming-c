section .data
message: db 'hello world', 10, 0
message_end:

section .text
global _start
_start:
    mov rax, 1 ; write
    mov rdi, 1 ; stdout
    mov rsi, message
    mov rdx, message_end - message
    syscall

    mov rax, 60 ; exit
    xor rdi, rdi ; 0
    syscall
