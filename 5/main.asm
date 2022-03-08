section .data
%include "words.inc"
global _start
extern find_word
extern read_word
extern exit
extern string_length
extern print_string

section .text
; enter key, the ^D^D (why twice?) and if key found value is printed
_start:
    sub rsp,255
    mov rdi,rsp
    mov rsi,254
    call read_word ; read word, not string
    test rax,rax
    jz .err
    mov rdi,rax
    lea rsi,[colon_head]
    call find_word
    test rax,rax
    jz .err
    
.ok:
    lea rdi, [rax + 8]
    push rdi
    call string_length
    pop rdi
    lea rdi, [rdi + rax+1]
    call print_string
    mov rdi, 0
    call exit
.err:
    mov rdi, 1
    call exit

