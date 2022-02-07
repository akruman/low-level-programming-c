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