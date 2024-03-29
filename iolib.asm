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

read_char: ;Read one character from stdin and return it. If the end of input stream occurs, return 0.
    mov rdx,1 ; count=1
    mov rax,0 ; read

    push rdi
    lea rsi, [rsp-1] ; stack grows down
    mov rdi,0 ; stdin
    syscall ; end of stream rax rets 0
    test rax, rax
    jz .end

    xor rax,rax
    mov al, [rsp-1] ; return in rax
.end:
    pop rdi
    ret   

is_whitespace: ; if dil is white space ZF=1
    cmp dil, 0x20
    jz .end
    cmp dil, 0x10
    jz .end
    cmp dil, 0x09
.end:
    ret

is_eof: ; if dil is white space ZF=1
    cmp dil, 0
    ret

; why i need twice ^d ?
read_word: ; (buffer* rdi, size rsi)
    ;Accepts a buffer address and size as arguments. 
    ;Reads next word from stdin (skipping whitespaces into buffer). 
    ;Stops and returns 0 if word is too big for the buffer specified; otherwise returns a buffer address.
    ;This function should null-terminate the accepted string.

    push r12
    push r13
    push rdi

    test rsi,rsi
    jz .err

    mov r12, rdi ; pbuf
    lea r13, [rdi + rsi -1 ] ; pbuf+size-1, the last addr to write
    mov byte[r12], 0 ; null terminate

.whitespace:
    call read_char
    mov dil, al
    call is_whitespace
    jz .whitespace
    call is_eof
    jz .retbuf

.write_al:
    ; al has valid char to write
    cmp r12,r13
    je .err
    mov byte[r12], al
    inc r12
    mov byte[r12], 0
    call read_char
    mov dil, al
    call is_whitespace
    jz .retbuf
    call is_eof
    jz .retbuf
    jmp .write_al

.err:
    xor rax,rax ; ret 0 on err
    pop rdi
    pop r13
    pop r12
    ret
.retbuf:
    pop rdi
    mov rax,rdi
    pop r13
    pop r12
    ret


parse_uint: ; (buffer* rdi)
            ; Accepts a null-terminated string and tries to parse an unsigned number from its start.
            ;Returns the number parsed in rax, its characters count in rdx.
    xor rdx,rdx
    xor rax,rax
.loop:
    cmp byte[rdi],0
    jz .end
    cmp byte[rdi],'0'
    jl .end
    cmp byte[rdi],'9'
    jg .end
    inc rdx

    lea rax, [rax +  4*rax]; rax = rax*5
    shl rax,1 ; *2

    xor rcx,rcx
    mov cl, byte[rdi]
    sub cl, '0'
    add rax, rcx

    inc rdi
    jmp .loop

.end:
    ret

parse_int: ; (buffer* rdi)
        ; Accepts a null-terminated string and tries to parse a signed number from its start. Returns the number parsed in rax; 
        ; its characters count in rdx (including sign if any). No spaces between sign and digits are allowed.
    xor rdx,rdx
    xor rax,rax
    cmp byte[rdi],'-'
    jne .positive
    inc rdx
    inc rdi
    call parse_uint.loop
    neg rax
    ret
.positive:
    jmp parse_uint.loop


string_equals: ; (buffer* rdi, buffer* rsi) 
        ; Accepts two pointers to strings and compares them. Returns 1 if they are equal, otherwise 0.
    xor rax,rax
.loop:
    mov dl, byte[rdi]
    mov dh, byte[rsi]
    cmp dl,dh
    jne .end
    test dl,dl ; \0 both
    jz .equal
    inc rdi
    inc rsi
    jmp .loop

.equal:
    inc rax
.end:
    ret


string_copy: ; (src* rdi, dst* rsi, dst_len rdx) 
        ; Accepts a pointer to a string, a pointer to a buffer, and buffer’s length. 
        ; Copies string to the destination. The destination address is returned if the string fits the buffer; otherwise zero is returned.
    push rsi
    test rdx,rdx
    jz .end
.loop:
    cmp byte[rdi],0
    jz .end
    mov al,[rdi]
    mov [rsi],al
    inc rdi
    inc rsi
    dec rdx
    jnz .loop

.end:
    cmp rdx,0
    jae .ok
    xor rax,rax
    pop rsi
    ret
.ok:
    pop rsi
    mov rax,rsi
    ret

section .data
    s1: db "asdf",0
    s2: times 8 db 0
    s3: db '123456',0
    s4: db '-123456',0
    s5: db 'asd',0
    s6: db 'as1',0
    s7: db 'asdfg',0
    s8: times 8 db 0
    s9:
section .text
_start:
    jmp .test5
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

    call read_char
    mov rdi,rax
    call print_char ; print back the char read

    call print_newline

.test1:
    mov rdi, s2
    mov rsi, s3-s2
    call read_word
    test rax,rax
    jz .next
    mov rdi,s2
    call print_string
.next:
    call print_newline
.test2:
    mov rdi, s3
    call parse_uint
    mov rdi, rax
    call print_uint
.test3:
    mov rdi, s4
    call parse_int
    mov rdi, rax
    call print_int
.test4:
    mov rdi, s5
    mov rsi, s5
    call string_equals
    mov rdi, rax
    call print_uint

    mov rdi, s5
    mov rsi, s6
    call string_equals
    mov rdi, rax
    call print_uint
.test5:
    mov rdi, s7 
    mov rsi, s8
    mov rdx, s9-s8
    call string_copy
    test rax,rax
    jz .next1
    mov rdi, rax ; buf is long enough, has 0 at end
    call print_string
.next1:

    mov rdi, 1
    call exit