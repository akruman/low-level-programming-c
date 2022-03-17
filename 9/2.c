#include <stddef.h>
typedef int (fint)(int x); // better without *fint, so we can use it in declaration

fint inc;

int inc(int x) {return x+1;}

long long scalar_product(const int *a, const int *b, size_t len) {
    long long ret =0;
    if(a && b){
        for(size_t i=0; i<len;++i)
            ret += (long long)a[i]* (long long)b[i];
    }
    return ret;
}

int main(){
    fint* fi = &inc;
    fi(5);
    return 0;
}