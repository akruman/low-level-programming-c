// playing with sem (book has bad example)
#include<semaphore.h>
#include<pthread.h>
#include<inttypes.h>
#include<stdio.h>
#include <unistd.h>

#define LOC printf("%s %d\n", __func__, __LINE__)

sem_t sem, sem_t3_main; // compiler and hw membar
uint64_t cnt1 = 0;
uint64_t cnt2 = 0;
void* f1(void* _){
    for(;cnt1<1000;++cnt1);
    sem_post(&sem); // work done 
    return NULL;
}
void* f2(void* _){
    for(;cnt2<1000;++cnt2);
    sem_post(&sem); // work done 
    return NULL;
}
void* f3(void* _){
    LOC;
    sem_post(&sem_t3_main); // allow main to proceed creating t1,t2

    LOC; int i; sem_getvalue(&sem, &i); printf("%d\n",i);
    sem_wait(&sem); sem_wait(&sem); // wait t1,t2 to finish work
    
    printf("End: counter1 = %" PRIu64 " counter2 = %" PRIu64 "\n",
            cnt1, cnt2 );
    return NULL;
}

int main(){
    pthread_t t1, t2, t3;    
    sem_init(&sem, 0, 0);
    sem_init(&sem_t3_main, 0, 0);
    
    LOC;
    pthread_create(&t3, NULL, &f3, NULL);
    LOC;
    sem_wait(&sem_t3_main); // wait for t3 to start
    sem_destroy(&sem_t3_main);
    LOC;
    pthread_create(&t1, NULL, &f1, NULL);
    pthread_create(&t2, NULL, &f2, NULL);

    pthread_join(t3, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&sem); // destory sem after its not used anymore
    return 0;
}