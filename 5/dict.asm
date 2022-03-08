global find_word

extern string_equals
section .text
find_word: ; (char* key, last* entry). rets found entry addr, or 0  (rdi,rsi)->rax
    xor rax,rax
.loop:
    test rsi,rsi
    jz .err

    push rsi
    push rdi
    add rsi,8
    call string_equals ; (buffer* rdi, buffer* rsi) -> rax
    cmp rax,1
    jz .ok
    pop rdi
    pop rsi
    mov rsi, [rsi]
    jmp .loop

.ok:
    pop rdi
    pop rsi
    mov rax, rsi ; record contains: *prev,key_str,0,val_str,0
    ret

.err:
    xor rax,rax
    ret


    