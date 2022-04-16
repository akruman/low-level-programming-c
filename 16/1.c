#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
int nanosleep(const struct timespec *req, struct timespec *rem);
int main(){
    const size_t sz = 1024*1024*64;
    char *p = malloc(sz);
    uint32_t sum=0;
    //__builtin_prefetch() https://www.daemon-systems.org/man/__builtin_prefetch.3.html
    char *q;
    for(int i=1;i<100;i++){
        q = p + sz/i; // jumping
        #ifdef PREFETCH
        #define READ 0 // write 1
        for(int j=0;j<10;j++)
        __builtin_prefetch(p + sz/(i+j),READ,0);
        #endif
        sum += (uint32_t)*q;
        struct timespec t;
        t.tv_sec = 0; t.tv_nsec=1;
        nanosleep(&t,0);
    }
    printf("%u",sum);
    free(p);
    return 0;
}