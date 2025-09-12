#include <stdio.h>
#include <stdlib.h>

#define TLB_SIZE 4

typedef struct {
    int page;
    int frame;
} TLB_Entry;

TLB_Entry tlb[TLB_SIZE];
int tlb_index = 0;

void initialize_tlb() {
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].page = -1;
        tlb[i].frame = -1;
    }
}

int search_tlb(int page) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page == page) {
            printf("TLB hit! Página %d -> Quadro %d\n", page, tlb[i].frame);
            return tlb[i].frame;
        }
    }
    printf("TLB miss para página %d\n", page);
    return -1;
}

void add_to_tlb(int page, int frame) {
    tlb[tlb_index].page = page;
    tlb[tlb_index].frame = frame;
    tlb_index = (tlb_index + 1) % TLB_SIZE;
    printf("Adicionado à TLB: Página %d -> Quadro %d\n", page, frame);
}

int main() {
    initialize_tlb();

    // Simular algumas buscas
    int page_requests[] = {1, 2, 1, 3, 4, 2, 5};
    int n = sizeof(page_requests) / sizeof(page_requests[0]);

    for (int i = 0; i < n; i++) {
        int page = page_requests[i];
        int frame = search_tlb(page);
        if (frame == -1) {
            // Simular busca na tabela de páginas (hardcoded)
            int new_frame = page * 10; // Exemplo fictício
            add_to_tlb(page, new_frame);
        }
    }

    return 0;
}
/*
gcc tlb_simulator.c -o tlb
./tlb
*/