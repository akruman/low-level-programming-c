// atomics c11
#include<stdatomic.h>
int main(){

    _Atomic(int) x = ATOMIC_VAR_INIT(11);
    _Atomic(int) y;
    atomic_init(&y, 1);

    atomic_thread_fence(memory_order_seq_cst);
    atomic_fetch_add(&x,1);
    return 0;
}
