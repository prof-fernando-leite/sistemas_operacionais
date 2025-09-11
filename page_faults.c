//
// Created by mister-storm on 03/09/2025.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sched.h>

#define SIZE (1024 * 1024 * 1024) // 1GB

void pin_to_cpu(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);
}

int main() {
    printf("PID: %d\n", getpid());
    pin_to_cpu(0); // Fixar na CPU 0 para medição consistente

    printf("=== FASE 1: Alocação de memória ===\n");
    printf("Alocando 1GB...\n");

    char *memory = malloc(SIZE);
    if (!memory) {
        perror("malloc failed");
        return 1;
    }

    printf("Memória alocada virtualmente em %p\n", memory);
    printf("Pressione Enter para começar acesso...");
    getchar();

    printf("\n=== FASE 2: Primeiro Acesso (Minor Faults) ===\n");
    printf("Acessando cada página pela primeira vez...\n");

    for (int i = 0; i < SIZE; i += 4096) {
        memory[i] = 'A';
    }

    printf("Primeiro acesso completo!\n");
    printf("Pressione Enter para segundo acesso...");
    getchar();

    printf("\n=== FASE 3: Segundo Acesso (Sem Faults) ===\n");
    printf("Acessando memória já residente...\n");

    for (int i = 0; i < SIZE; i += 4096) {
        memory[i] = 'B';
    }

    printf("Segundo acesso completo!\n");
    printf("Pressione Enter para liberar memória...");
    getchar();

    free(memory);

    printf("\n=== FASE 4: Forçar Major Faults ===\n");
    printf("Alocando e acessando até swap...\n");

    // Alocar mais memória do que temos físicamente
    size_t huge_size = 2 * SIZE; // 2GB - forçar swap
    char *huge_memory = malloc(huge_size);

    if (huge_memory) {
        printf("Forçando major faults...\n");
        for (size_t i = 0; i < huge_size; i += 4096) {
            huge_memory[i] = 'C';
        }
        free(huge_memory);
    } else {
        printf("Não foi possível alocar 2GB (normal em sistemas com pouca RAM)\n");
    }

    printf("Exercício completo!\n");
    return 0;
}

/*
compila
 gcc -D_GNU_SOURCE -g -o page_faults page_faults.c
./page_faults
TERMINAL 2:

# Achar o PID
ps aux | grep page_faults

# Monitorar faults em tempo real
watch -n 1 'echo "Minor: $(grep minor /proc/[PID]/stat | cut -d" " -f10), Major: $(grep major /proc/[PID]/stat | cut -d" " -f12)"'

# Alternativa: usar pidstat
pidstat -r 1 [PID]

# Ou usar vmstat para ver swap
vmstat 1
*/