#include <stdio.h>
#include <stdlib.h>

#define FRAMES 3

int frames[FRAMES];
int index = 0;

void initialize() {
    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
    }
}

void display() {
    printf("Quadros: ");
    for (int i = 0; i < FRAMES; i++) {
        if (frames[i] == -1)
            printf("[ ] ");
        else
            printf("[%d] ", frames[i]);
    }
    printf("\n");
}

int page_fault(int page) {
    for (int i = 0; i < FRAMES; i++) {
        if (frames[i] == page) return 0;
    }
    return 1;
}

void fifo(int page) {
    if (page_fault(page)) {
        printf("Page fault para página %d\n", page);
        frames[index] = page;
        index = (index + 1) % FRAMES;
    } else {
        printf ("Página %d já está na memória\n", page);
    }
    display();
}

int main() {
    initialize();
    int requests[] = {1, 2, 3, 4, 1, 2, 5};
    int n = sizeof(requests) / sizeof(requests[0]);

    for (int i = 0; i < n; i++) {
        printf("Referência %d: ", requests[i]);
        fifo(requests[i]);
    }

    return 0;
}

/*
gcc fifo.c -o fifo
./fifo
*/