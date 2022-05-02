// atomics c11
#include<stdatomic.h>
#include<pthread.h>
#include<stdio.h>
// https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
// https://gcc.gnu.org/onlinedocs/gcc-4.9.3/gcc/_005f_005fatomic-Builtins.html#g_t_005f_005fatomic-Builtins

struct Shared {
   _Atomic(int) x;
   char pad[128];
   int n;
   char pad1[128];
   const char *str_mr;
   void* (*f1)(void*);
   void* (*f2)(void*);
};

#define STR(X) #X
#define T1(MR1)\
void* f1##__##MR1(void* _){\
    struct Shared *sh = (struct Shared*)_;\
    for(int i=0; i<1000000;i++){\
        sh->n=i;\
        atomic_store_explicit(&sh->x, i, MR1 );\
    }\
    return NULL;\
}

#define T2(MR2)\
void* f2##__##MR2(void* _){\
    int last, cur, cnt=0, cnt1=0;\
    struct Shared *sh = (struct Shared*)_;\
    last = atomic_load_explicit(&sh->x, MR2);\
    for(int i=0; i<1000000;i++){\
        int tmp = sh->n;\
        cur = atomic_load_explicit(&sh->x,  MR2);\
        if(last > cur) cnt++;\
        if(tmp < last) cnt1++;\
        last = cur;\
    }\
    printf("%s :{outoforder: %d, unprotected: %d}\n",sh->str_mr, cnt, cnt1);\
    return NULL;\
}

#define T(MR) T1(MR) T2(MR)
#define TT(MR1,MR2) \
{\
    .x = ATOMIC_VAR_INIT(0),\
    .n = 0,\
    .f1 = &f1##__##MR1,\
    .f2 = &f2##__##MR2,\
    .str_mr = STR(MR1)"-"STR(MR2)\
}


// declare functions with template
T(memory_order_relaxed)
T(memory_order_seq_cst)
T1(memory_order_release)    T2(memory_order_acquire)
                            T2(memory_order_consume)

// shared vars per test
struct Shared shared[] = {
    TT(memory_order_relaxed, memory_order_relaxed),
    TT(memory_order_seq_cst, memory_order_seq_cst),
    TT(memory_order_release, memory_order_acquire),
    TT(memory_order_release, memory_order_consume)
};

#define SZ(X) sizeof(X)/sizeof(X[0])

int main(){

    // first lets see that memory_order_relaxed indeed shows newer versions of var, and not mixing with old, but not protecting other shared mem
    pthread_t t1,t2;

    for(int i=0; i<SZ(shared); i++){
        printf("----------- %s\n", shared[i].str_mr);

        pthread_create(&t1, NULL, shared[i].f1, &shared[i]);
        pthread_create(&t2, NULL, shared[i].f2, &shared[i]);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        puts("-----------");
    }

#if 0  
    _Atomic(int) y;
    atomic_init(&y, 1);

    //atomic_thread_fence(memory_order_seq_cst);
    atomic_fetch_add_explicit(&x,1,memory_order_relaxed); 
    atomic_fetch_add_explicit(&x,1,memory_order_consume);
    atomic_fetch_add_explicit(&x,1,memory_order_acquire);
    atomic_fetch_add_explicit(&x,1,memory_order_release);
    atomic_fetch_add_explicit(&x,1,memory_order_acq_rel);
    atomic_fetch_add_explicit(&x,1,memory_order_seq_cst);


    __asm__ volatile ("movb %%al, 1":::"%al", "memory"); // clobber al; i just wanna see this instruction exactly here in objdump

    atomic_store_explicit(&x, 15, memory_order_relaxed);
    //atomic_store_explicit(&x, 15, memory_order_consume); - no store
    //atomic_store_explicit(&x, 15, memory_order_acquire); - no store
    atomic_store_explicit(&x, 16, memory_order_release); 
    //atomic_store_explicit(&x, 15, memory_order_acq_rel); - no store
    atomic_store_explicit(&x, 17, memory_order_seq_cst); // this creates mfence after it, an above do not

    __asm__ volatile ("movb %%al, 2":::"%al", "memory"); // clobber al; i just wanna see this instruction exactly here in objdump

    __asm__ volatile ("movb %%al, 3":::"%al", "memory"); // clobber al; i just wanna see this instruction exactly here in objdump

    atomic_load_explicit(&x, memory_order_relaxed);
    atomic_load_explicit(&x, memory_order_consume);
    atomic_load_explicit(&x, memory_order_acquire);
    //atomic_load_explicit(&x, memory_order_release); - not load
    //atomic_load_explicit(&x, memory_order_acq_rel); - rel is not load
    atomic_load_explicit(&x, memory_order_seq_cst);

    __asm__ volatile ("movb %%al, 4":::"%al", "memory"); // clobber al; i just wanna see this instruction exactly here in objdump
#endif
    return 0;
}
