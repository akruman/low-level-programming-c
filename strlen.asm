global _start
section .data
message: db 'asdc', 0

section .text

strlen: 
    xor rax,rax
.loop:
    cmp byte[rdi + rax], 0
    je .end
    inc rax
    jmp .loop
.end     
    ret

_start:
    mov rdi, message
    call strlen
    mov rdi, rax    ; mov ret, to prog retcode

    mov rax,60
    syscall