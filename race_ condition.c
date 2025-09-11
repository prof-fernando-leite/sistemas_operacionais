// race_condition.c
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 10
#define ITERATIONS 1000000

// Variável compartilhada
int shared_counter = 0;
atomic_int atomic_counter = 0;

void *incrementer(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        shared_counter++; // Race condition!
    }
    return NULL;
}

void *atomic_incrementer(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        atomic_fetch_add(&atomic_counter, 1);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    printf("Testando race condition...\n");
    
    // Teste com race condition
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, incrementer, NULL);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Resultado com race condition: %d (esperado: %d)\n", 
           shared_counter, NUM_THREADS * ITERATIONS);
    
    // Teste com atomic
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, atomic_incrementer, NULL);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Resultado com atomic: %d (esperado: %d)\n", 
           atomic_counter, NUM_THREADS * ITERATIONS);
    
    return 0;
}
