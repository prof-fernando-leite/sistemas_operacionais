//
// Created by mister-storm on 03/09/2025.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define NUM_THREADS 4

void *worker(void *arg) {
    long id = (long)arg;
    volatile int counter = 0;


    for (int i = 0; i < 100000000; i++) {
        counter += i % 100;
    }

    printf("Thread %ld finished\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
/*
# Compilar com debug symbols
gcc -g -o context_switch context_switch.c -lpthread

# Medir context switches
sudo perf stat -e context-switches,cpu-migrations
 ./context_switch

# Profile do scheduler
sudo perf record -e sched:sched_switch,sched:sched_stat_wait
 ./context_switch

sudo perf script | head -20
*/

