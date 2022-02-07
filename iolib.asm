global _start
global exit

section .text
exit: ; Accepts an exit code and terminates current process.
    mov rax, 60 ; exit, rdi has exit code
    syscall

string_length: ; Accepts a pointer to a string and returns its length.
    xor rax,rax
.loop:
    cmp byte[rdi+rax],0
    je .end
    inc rax
    jmp .loop
.end:
    ret

print_string: ;Accepts a pointer to a null-terminated string and prints it to stdout
    ; need to save rdi, but its not modified inside
    call string_length
    mov rdx, rax ; count
    mov rax,1 ; write
    mov rsi, rdi ; buf
    mov rdi,1 ; stdout
    syscall 
    ret

print_char: ;Accepts a character code directly as its first argument and prints it to stdout .
    mov rdx,1 ; count=1
    mov rax,1 ; write
    push rdi ; https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf
    mov rsi, rsp
    mov rdi,1 ; stdout
    syscall
    pop rdi
    ret

print_newline: ;Prints a character with code 0xA.
    mov rdi, 0xA
    call print_char
    ret

print_uint: ;Outputs an unsigned 8-byte integer in decimal format.
    push rbp
    mov rbp, rsp

    dec rsp
    mov byte[rsp], 0 ; zero terminate

    mov rcx, 10
    mov rax, rdi
.divloop:
    xor rdx, rdx ; 0
    div rcx ; rax = rdx:rax/10; rdx = rdx:rax mod 10

    or dl, 0x30 ; convert to ascii
    dec rsp
    mov [rsp], dl

    test rax,rax
    jnz .divloop

    mov rdi, rsp
    call print_string

    mov rsp, rbp
    pop rbp
    ret

print_int: ;Output a signed 8-byte integer in decimal format.
    cmp rdi, 0
    jge print_uint
    neg rdi
    push rdi
    mov rdi, 45 ; -
    call print_char

    pop rdi
    call print_uint
    ret

section .data
    s1: db "asdf",0
    s2: 
section .text
_start:
    mov rdi, s1
    call print_string
    mov rdi, 65
    call print_char
    call print_newline

    mov rdi, -1 ; 2**64-1
    call print_uint

    call print_newline

    mov rdi, -123456
    call print_int

    mov rdi, 1
    call exit