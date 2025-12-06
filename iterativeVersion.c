#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>


// Noms des piquets
char pegName(int i) {
    return (i == 0) ? 'A' : (i == 1) ? 'B' : 'C';
}

// Fonction simple pour afficher l'état des piquets
void printSimplePegs(int* peg[3], int top[3]) {
    for (int p = 0; p < 3; p++) {
        printf("%c: ", pegName(p));
        if (top[p] == -1) {
            printf("|"); // piquet vide
        }
        else {
            for (int i = 0; i <= top[p]; i++)
                printf("%d ", peg[p][i]);
        }
        printf("\n");
    }
    printf("\n");
}

// Déplace un disque d'un piquet à un autre
void moveDisk(int* peg[3], int top[3], int from, int to) {
    peg[to][++top[to]] = peg[from][top[from]--];
}




int main() {
    int n;
    printf("Enter number of disks: ");
    scanf("%d", &n);

    // Total moves
    uint64_t total_moves = (1ULL << n) - 1;

    // Allocation des piquets
    int* peg[3];
    int top[3] = { -1, -1, -1 }; // initialment tous les piquets sont vide

    for (int i = 0; i < 3; i++)
        peg[i] = (int*)malloc(n * sizeof(int));

    // Initialisation du premier piquet (A)
    for (int i = 0; i < n; i++)
        peg[0][++top[0]] = n - i;

    // Échange B et C si n pair
    int pegOrder[3] = { 0, 1, 2 };
    if (n % 2 == 0) { int temp = pegOrder[1]; pegOrder[1] = pegOrder[2]; pegOrder[2] = temp; }

    // Chronomètre haute résolution
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    printf("Initial State:\n");
    printSimplePegs(peg, top);

    // Algorithme itératif réel (sans affichage)
    for (uint64_t move = 1; move <= total_moves; move++) {
        int from, to;

        if (move % 3 == 1) { from = pegOrder[0]; to = pegOrder[2]; }
        else if (move % 3 == 2) { from = pegOrder[0]; to = pegOrder[1]; }
        else { from = pegOrder[1]; to = pegOrder[2]; }

        // LA CONDITION DE  Alarger disk cannot be placed on a smaller one.
        int diskFrom = (top[from] >= 0) ? peg[from][top[from]] : n + 1;// LE CAS OU LE PIQUET FROM ET VIDE N+1
        /*   EX: from = vide, to = [1]
                diskFrom = n + 1 = 4
                diskTo = 1
                Test : 4 < 1 → faux
                Donc on tombe dans le else → on déplace forcément to → from.
                Ce qui est correct : si “from” est vide, tu dois prendre le disque du “to”
        
        */
       
        int diskTo = (top[to] >= 0) ? peg[to][top[to]] : n + 1;
        if (diskFrom < diskTo) { //met le disque du sommet de from sur to
            moveDisk(peg, top, from, to); // FROM --> to
            printf("Move disk %d from %c to %c\n", diskFrom, pegName(from), pegName(to));
        }
        else { //ela signifie que le disque au sommet de from est plus grand ou égal que celui au sommet de to. Dans ce cas la seule action légale est de déplacer le disque du sommet de to vers from
            moveDisk(peg, top, to, from); // TP --> FROM
            printf("Move disk %d from %c to %c\n", diskTo, pegName(to), pegName(from));
        }
        printSimplePegs(peg, top);

    }

    QueryPerformanceCounter(&end);
    double elapsed = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    printf("\nNumber of moves: %llu\n", total_moves);
    printf("RunTime: %.9f seconds\n", elapsed);

    // Libération mémoire
    for (int i = 0; i < 3; i++)
        free(peg[i]);

    return 0;
}