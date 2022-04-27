Any function can accept an unlimited number of arguments. The first six arguments are passed in 
rdi, rsi, rdx, rcx, r8, and r9, respectively. The rest is passed on to the stack in reverse order.

These registers are callee-saved: rbx, rbp, rsp, r12-r15
All other registers are caller-saved.

return values in: rax, rdx 



rax, rdx = f(rdi, rsi, rdx, rcx, r8, r9, stack)   {save rbx, rbp, rsp, r12-r15}


save rcx if needed
rax = syscall (rax, rdi, rsi, rdx, r10, r8, r9)   rcx used inside
https://filippo.io/linux-syscall-table/


mov byte[test], 1 ;1
mov word[test], 1 ;2
mov dword[test], 1 ;4
mov qword[test], 1 ;8

mov rax, [rbx + 4* rcx + 9]
lea rax, [rbx + rbx * 4]     ; rax = rbx * 5

https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf

8-byte register Bytes 0-3 Bytes 0-1 Byte 0
%rax %eax %ax %al
%rcx %ecx %cx %cl
%rdx %edx %dx %dl
%rbx %ebx %bx %bl
%rsi %esi %si %sil
%rdi %edi %di %dil
%rsp %esp %sp %spl
%rbp %ebp %bp %bpl
%r8 %r8d %r8w %r8b
%r9 %r9d %r9w %r9b
%r10 %r10d %r10w %r10b
%r11 %r11d %r11w %r11b
%r12 %r12d %r12w %r12b
%r13 %r13d %r13w %r13b
%r14 %r14d %r14w %r14b
%r15 %r15d %r15w %r15b

#view sections: linking
readelf -S [elffile]
# headers: execution
readelf -l [elffile]

sections:

.text ; code
.data ; inted globals
.rodata ; ro data
.bss ; zero inted space, inited on load
    resw, resb - to alloc space

.rel.text ; store relocation table, what locations to modify in text after load
.rel.data
.debug ; symbol table
.line ; code locaton to asm locations
.symtab ; symbol table

global x ; symbol defined here 
extern x ; defined somewhere else


objdump -D -M intel-mnemonic main.o ; dissasm intel style
readelf --relocs  main.o ; relocation table
objdump -tf main

to adjust sectino addrs see : Using ld, the gnu linker. Available: www.math.utah.edu/docs/info/ld_3.html . 1994.

c11
#include <stdalign.h>
alignof(int)
alignas(8) short x;

13
example projects:
www.gnu.org/software/gsl/
www.gnu.org/software/gsl/design/gsl-design.html
http://git.savannah.gnu.org/cgit/kazlib.git/

17

volatile - suppress r/w reorder for specific var, read/write access mem.

19.3
