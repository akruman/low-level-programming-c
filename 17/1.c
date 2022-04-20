#include <stdio.h>
int main(){
    int x=111;
    __asm__ volatile("" ::: "memory"); // on O1 it removes 111 and 222 inits
    __asm__ volatile("mfence" ::: "memory"); // didnt help either, 111 optimized out
    x=222;
    int y=333;
    y=444;
    printf("%d %d",x,y);
    return 0;
}