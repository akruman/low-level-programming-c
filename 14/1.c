#include <stdarg.h>
#include <stdio.h>
#include <string.h>
void print(int y, unsigned long first, ...){
    va_list args;
    unsigned long i;
    va_start(args,first);
    while((i = va_arg(args, unsigned long))){ // fetch args after "first"
        printf("%lu ",i);
    }

    va_end(args);


}

// stack grows down
// but local vars grow up 
void f(int *px){
    int y;
    printf("%p %p %ld",(void*)px, (void*)&y, (char*)&y - (char*)px); // &y < &x
}
void g(){
    int x;
    f(&x);
}


int main(){
    print(999,5,6,7,0);
    g();

    return 0;
}