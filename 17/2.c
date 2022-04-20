#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
/*
compiler bar
asm volatile(""::: "memory")

1. write barrier - writes dont cross  sfence
2. read barrier - reads dont cross    lfence
3. data dependancy:   char *p = & struct {int x};  p->x;  read of x depends on read of p
4. general barrier: - reads/writes dont cross   mfence
5. acquire - read shmem, following reads/writes dont cross back:                 |acquire| r/w
6. release - write shmem, preceding reads/writes dont cross fwd:    r/w |release| 

compiler bar + membar
asm volatile("mfence" ::: "memory")

function call (not optimized in external module) is compiler barr
thread create,join, mutex - compiler & membar

*/
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

volatile int x = 0; // to avoid loop optimizations
void* f1(void* _){
    pthread_mutex_lock(&m1); // if remove locks hellgrind detects err
    for(int i=0;i<10000;i++)x++;
    pthread_mutex_unlock(&m1);
    return NULL;
}
void* f2(void* _){
    pthread_mutex_lock(&m1);
    for(int i=0;i<10000;i++)x++;
    pthread_mutex_unlock(&m1);
    return NULL;
}

pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
bool sent = false;
void* f3(void* _){
    printf("f3\n");
    pthread_mutex_lock(&m2);
    printf("f3 locked\n");
    while (!sent)pthread_cond_wait( &condvar, &m2 ); // unlocks m2, waits for cond, after getting it locks m2; while needed cause we can wake up on signal
    printf("f3 after wait\n");
    pthread_mutex_unlock(&m2);
    printf("f3 unlocked\n");
    return NULL;
}
void* f4(void* _){
    printf("f4\n");
    pthread_mutex_lock(&m2);
    printf("f4 locked\n");
    sent = true;
    pthread_mutex_unlock(&m2);
    printf("f4 unlocked\n");
    printf("f4 signal\n");
    pthread_cond_signal( &condvar ); //signal cond, so pthread_cond_wait can wake and lock back
    sleep(4); 
    printf("f4 end\n");
    
    return NULL;
}

// pthread_spin_init with real-time scheduling policies (SCHED_FIFO, or possibly SCHED_RR)

int main(){
    pthread_t t1,t2;
    if(pthread_create(&t1, NULL, &f1, NULL))goto error;
    if(pthread_create(&t2, NULL, &f1, NULL))goto error;
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%d\n",x);

    pthread_t t3,t4;
    if(pthread_create(&t3, NULL, &f3, NULL))goto error;
    sleep(1);
    if(pthread_create(&t4, NULL, &f4, NULL))goto error;
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);


error:
    return 0;
}