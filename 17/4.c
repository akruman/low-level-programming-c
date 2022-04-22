// check compiler & cpu reordering and how to prevent it
#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
sem_t sem1, sem2, sem_done;

int x,y,read0,read1;
void* f1(void* _){
    for(;;){
        sem_wait(&sem1);
        x=1;
        //__asm__ volatile ("":::"memory"); // disable compiler reorder 
        __asm__ volatile ("mfence":::"memory"); // disable compiler reorder & cpu 
        read0 = y;
        sem_post(&sem_done);
    }
}
void* f2(void* _){
    for(;;){
        sem_wait(&sem2);
        y=1;
        //__asm__ volatile ("":::"memory"); // disable compiler reorder 
        __asm__ volatile ("mfence":::"memory"); // disable compiler reorder & cpu 
        read1 = x;
        sem_post(&sem_done);
    }
}

int main(){
    pthread_t t1,t2;
    sem_init(&sem1,0,0);
    sem_init(&sem2,0,0);
    sem_init(&sem_done,0,0);
    pthread_create(&t1, NULL, f1, NULL);
    pthread_create(&t2, NULL, f2, NULL);

    for(int i=0; i<200000;++i){
        x=0;
        y=0;
        sem_post(&sem1);
        sem_post(&sem2);

        sem_wait(&sem_done);
        sem_wait(&sem_done);
        if(read0 == 0 && read1 == 0){
            printf("reorder instructions on %d", i);
            return 0;
        }
    }


    return 0;
}