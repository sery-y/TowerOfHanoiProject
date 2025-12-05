#include <stdio.h>

#include <windows.h>

#define stackLength 65 // the maximum stack size


long long moves = 0; // the total number of moves(a global variable initially 0)

// the structure of the stack( a record)
typedef struct {
    int arr[stackLength];
    int top;
} stack;

// stack functions
// a function to initialize the stack
void init(stack *s) { s->top = -1; }

// a function to push a disk to the stack
void push(stack *s, int disk) {
    if (s->top >= stackLength - 1) {
        printf("Error: stack overflow\n");
        exit(1);
    }
    s->arr[++(s->top)] = disk;
}

// a function to pull a disk from the stack
int pop(stack *s) {
    if (s->top == -1) {
        printf("Error: pop from empty stack\n");
        exit(1);
    }
    return s->arr[(s->top)--];
}


//  print pegs 
void printPegs(stack *peg[3], char name[3]) {
    for (int p = 0; p < 3; p++) {
        printf("%c: ", name[p]);
        
        if (peg[p]->top == -1)
            printf("|");  // empty peg
        else {
            for (int i = 0; i <= peg[p]->top; i++)
                printf("%d ", peg[p]->arr[i]);
        }
        printf("\n");
    }
    printf("\n");
}

// moving the disk from a peg to another
void Move(stack *from, stack *to, char src, char dst, stack *pegs[3], char names[3]) {
    int disk = pop(from);
    push(to, disk);
    printf("Move disk %d from %c to %c\n", disk, src, dst);
    moves++;

    printPegs(pegs, names);  // print after movement
}

// recursive Hanoi 
void Hanoi(int n, stack *A, stack *C, stack *B, 
           stack *pegs[3], char names[3], char nameA, char nameC, char nameB) {

    if (n == 0) return;

    Hanoi(n - 1, A, B, C, pegs, names, nameA, nameB, nameC);
    Move(A, C,nameA, nameC, pegs, names);
    Hanoi(n - 1, B, C, A, pegs, names, nameB, nameC, nameA);
}

// main
int main() {
    stack A, B, C;
    init(&A); init(&B); init(&C);

    int n;
    printf("Enter number of disks: ");
    scanf("%d", &n);

    // fill the source stack A with the disks
    for (int i = n; i >= 1; i--)
        push(&A, i);

    // prepare peg pointers and names for printing
    stack *pegs[3] = { &A, &B, &C };
    char names[3] = { 'A', 'B', 'C' };

    // print initial state
    printPegs(pegs, names);

    // time measurement
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);// the time is recorded at the beginning

    Hanoi(n, &A, &C, &B, pegs, names, 'A', 'C', 'B');

    QueryPerformanceCounter(&end);// the time is recorded at the end
    double runtime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    printf("Runtime: %.9f seconds\n", runtime);
    printf("Moves   : %lld\n", moves);

    return 0;
}
