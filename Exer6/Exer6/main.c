//
//  main.c
//  Exer6
//
//  Created by Logan Christopher on 10/21/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

struct timespec t = {1, 0};


void *thread1(void *vargp) {
    pthread_detach(pthread_self());
    int even = 0;
    while (even < 60) {
        printf("Thread: 1 // Num: %d\n", even+=2);
        nanosleep(&t, NULL);
    }
    pthread_exit(NULL);
}

void *thread2(void *vargp) {
    pthread_detach(pthread_self());
    int odd = 1;
    while (odd < 60) {
        printf("Thread: 2 // Num: %d\n", odd+=2);
        nanosleep(&t, NULL);
    }
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    pthread_t tid[2];
    
    printf("Creating Thread 1 (even)...\n");
    pthread_create(&tid[0], NULL, thread1, NULL);
    
    printf("Creating Thread 2 (odd)...\n");
    pthread_create(&tid[1], NULL, thread2, NULL);
    
    pthread_exit(NULL);
}
