/*
Tower of Hanoi (Recursion)
--------------------------
What this program solves:
Moves n disks from rod A to rod C using rod B as spare, never
placing a larger disk on a smaller one - printing every move.

Real life example:
The legend of the Brahmin priests moving 64 golden disks. The
pattern of "move n-1 aside, move the base, move n-1 back" shows up
in backup rotation schemes and disk defragmentation strategies.

DIAGRAM (n = 3, A -> C via B):
  A: [1 2 3]   B: [ ]     C: [ ]
  move 2 disks A->B, move 3 A->C, move 2 disks B->C
  A: [ ]  B: [1 2]  ->  ...  ->  C: [1 2 3]
  T(n) = 2*T(n-1) + 1 -> 2^n - 1 moves total.

How the process works:
1. Move n-1 disks A -> B (recurse).
2. Move the largest disk A -> C (one step).
3. Move n-1 disks B -> C (recurse).
4. Base case n == 1: move directly.

Pseudo code:
START
HANOI(n, from, to, spare):
    IF n == 1: PRINT move disk 1 from->to
    ELSE:
        HANOI(n-1, from, spare, to)
        PRINT move disk n: from -> to
        HANOI(n-1, spare, to, from)
END

Sample input and dry run:
n = 3 -> 7 moves:
A->C, A->B, C->B, A->C, B->A, B->C, C->A ... wait, exact order:
disk1 A->C, disk2 A->B, disk1 C->B, disk3 A->C,
disk1 B->A, disk2 B->C, disk1 A->C

Main logic to understand:
Optimal substructure with a twist: the largest disk can move only
when everything above it is parked on the spare rod. Moves = 2^n - 1
(a 64-disk legend takes 584 billion years at 1 move/second). Pure
recursion with no bookkeeping - elegance at its finest.
*/

#include <stdio.h>

long long moves = 0;

/*
 * hanoi - Recursively prints the move sequence.
 */
void hanoi(int n, char from, char to, char spare) {
    if (n == 1) {
        moves = moves + 1;
        printf("%3lld. Disk 1: %c -> %c\n", moves, from, to);
        return;
    }
    hanoi(n - 1, from, spare, to);
    moves = moves + 1;
    printf("%3lld. Disk %d: %c -> %c\n", moves, n, from, to);
    hanoi(n - 1, spare, to, from);
}

int main() {
    int n;

    printf("Tower of Hanoi\n\n");

    printf("Number of disks (1-15): ");
    scanf("%d", &n);

    if (n < 1 || n > 15) {
        printf("Keep it between 1 and 15 (2^n-1 moves!)\n");
        return 1;
    }

    printf("\nTotal moves required: 2^%d - 1 = %lld\n\n", n, (1LL << n) - 1);

    hanoi(n, 'A', 'C', 'B');

    return 0;
}
