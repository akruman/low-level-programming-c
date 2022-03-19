#include<stdio.h>
#define F(x) (_Generic( (x), int: "%d", double: "%f", default: "%x" ))
#define print(x) printf(F(x), x); puts("");
int main(){
    print(1);
    print(1.0);
    char a[10];
    char *pa = a;
    printf("%p %p %p %p\n",pa,a, (void*)&pa, (void*)&a);
    return 0;
}