#include <stdio.h>
#define PRINT_SIZEOF(x) printf("sizeof(" #x ") = %zu\n", sizeof(x))
int main(){
    //q 159
    PRINT_SIZEOF(void*);
    //PRINT_SIZEOF(void);
    PRINT_SIZEOF(0);
    PRINT_SIZEOF('x');
    PRINT_SIZEOF("hello");

    int x=10;
    PRINT_SIZEOF(x=90);

    int arr[10];
    printf("n = %lu\n", sizeof(arr)/sizeof(arr[0]));

    
    return 0;
}