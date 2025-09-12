#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;
    int free;
    struct Block *next;
} Block;

Block *head = NULL;

void initialize(int size) {
    head = (Block *)malloc(sizeof(Block));
    head->size = size;
    head->free = 1;
    head->next = NULL;
}

void display() {
    Block *curr = head;
    printf("Estado da Memória:\n");
    while (curr != NULL) {
        printf("[%d KB - %s] -> ", curr->size, curr->free ? "Livre" : "Alocado");
        curr = curr->next;
    }
    printf("NULL\n");
}

void allocate(int size, char algorithm) {
    Block *curr = head;
    Block *best = NULL;
    Block *worst = NULL;

    while (curr != NULL) {
        if (curr->free && curr->size >= size) {
            if (algorithm == 'F') { // First Fit
                break;
            } else if (algorithm == 'B') { // Best Fit
                if (best == NULL || curr->size < best->size) best = curr;
            } else if (algorithm == 'W') { // Worst Fit
                if (worst == NULL || curr->size > worst->size) worst = curr;
            }
        }
        curr = curr->next;
    }

    if (algorithm == 'B' && best != NULL) curr = best;
    if (algorithm == 'W' && worst != NULL) curr = worst;

    if (curr == NULL || !curr->free || curr->size < size) {
        printf("Não foi possível alocar %d KB\n", size);
        return;
    }

    int old_size = curr->size;
    curr->free = 0;
    curr->size = size;

    if (old_size > size) {
        Block *new_block = (Block *)malloc(sizeof(Block));
        new_block->size = old_size - size;
        new_block->free = 1;
        new_block->next = curr->next;
        curr->next = new_block;
    }

    printf("Alocado %d KB\n", size);
}

int main() {
    initialize(800); // Memória inicial de 800 KB
    display();

    int requests[] = {100, 200, 50, 300, 150};
    int n = sizeof(requests) / sizeof(requests[0]);

    printf("\n--- Usando First Fit ---\n");
    for (int i = 0; i < n; i++) {
        allocate(requests[i], 'F');
        display();
    }

    // Reinicializar para Best Fit
    head = NULL;
    initialize(800);
    printf("\n--- Usando Best Fit ---\n");
    for (int i = 0; i < n; i++) {
        allocate(requests[i], 'B');
        display();
    }

    // Reinicializar para Worst Fit
    head = NULL;
    initialize(800);
    printf("\n--- Usando Worst Fit ---\n");
    for (int i = 0; i < n; i++) {
        allocate(requests[i], 'W');
        display();
    }

    return 0;
}

/*
gcc allocator.c -o allocator
./allocator
*/