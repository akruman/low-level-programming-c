section .data
codes: 
    db '0123456789ABCDEF'

section .text
global _start
_start:
    mov rax, 0x1122334455667788

    mov rdi, 1 ; fd=stdout
    mov rdx, 1 ; 1 char
    mov rcx, 64
.loop:
    push rax
    sub rcx, 4
    sar rax, cl ; more correct shr https://www.felixcloutier.com/x86/sal:sar:shl:shr
    and rax, 0xf

    lea rsi, [codes + rax] ; buf <- codes[(rax>>rcx) && 0x0f]
    mov rax, 1 ; write

    push rcx ; used in syscall
    syscall  ; rax <- retcode
    pop rcx

    pop rax 
    test rcx, rcx
    jnz .loop

    mov rax, 60  ; exit (0)
    xor rdi, rdi ; 
    syscall

