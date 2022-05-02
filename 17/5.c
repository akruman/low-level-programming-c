// atomics c11
#include<stdatomic.h>
// https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
// https://gcc.gnu.org/onlinedocs/gcc-4.9.3/gcc/_005f_005fatomic-Builtins.html#g_t_005f_005fatomic-Builtins
int main(){
    // return to this try all the consitency levels

    _Atomic(int) x = ATOMIC_VAR_INIT(11);
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

    return 0;
}
