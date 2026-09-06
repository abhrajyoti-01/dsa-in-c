/*
Josephus Problem - Circular Elimination
---------------------------------------
What this program solves:
n people stand in a circle; every k-th person is eliminated until
one survivor remains. Shows the simulation with a circular list
AND the O(n) math recurrence - two levels of mastery in one file.

Real life example:
Round-robin elimination (every k-th player drops out) in games and
token-ring networks where a token removal order must be predicted.

DIAGRAM (n = 5, k = 2 - every 2nd person leaves):
  circle: 1 2 3 4 5   start counting at 1
  eliminate 2 -> 1 3 4 5
  eliminate 4 -> 1 3 5
  eliminate 1 -> 3 5
  eliminate 5 -> 3      survivor = 3

BEGINNER EXPLANATION (the simplest way to think):
People hold hands in a circle. Start counting at person 0: count
k people, the k-th one sits down (eliminated). Keep counting from
the NEXT person, wrapping around the circle as needed. Last one
standing wins.

SUPER SIMPLE EXAMPLE (n = 5, k = 3):
Circle: 0 1 2 3 4
count 0,1,2 -> eliminate 2. Circle: 0 1 3 4
count 3,4,0 -> eliminate 0. Circle: 1 3 4
count 1,3,4 -> eliminate 4. Circle: 1 3
count 1,3,1 -> eliminate 1. Survivor: 3
(so for n=5, k=3 the answer is index 3 - the 4th person, 0-based)

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- k = 1: every 1st person is eliminated in order -> survivor is
  the last person, index n-1.
- k = 2: survivor follows the famous "power of two shuffle": write
  n = 2^m + r; survivor = 2*r (0-based). E.g. n=5: r=1 -> 2.
- n = 1: survivor is 0 - no elimination needed.
- Indexing trap: the math recurrence is 0-BASED; many textbook
  answers are 1-BASED (add 1). Mixing them up is the classic bug.

DRY RUN (recurrence, n = 5, k = 3):
J(1) = 0                                  (one person survives)
J(2) = (J(1) + 3) % 2 = (0+3) % 2 = 1
J(3) = (J(2) + 3) % 3 = (1+3) % 3 = 1
J(4) = (J(3) + 3) % 4 = (1+3) % 4 = 0
J(5) = (J(4) + 3) % 5 = (0+3) % 5 = 3   <- matches simulation!

TIPS:
- The recurrence J(n) = (J(n-1) + k) % n says: "after the first
  elimination, the circle of n-1 people is just a RELABELED copy -
  shift indices by k." That single sentence is the whole proof.
- Simulation with a circular array is O(n*k); the recurrence is
  O(n). For huge n use the k=2 closed form or segment-tree based
  methods.
- Interview favorite: they often ask for the 1-based answer of the
  k=2 case - remember survivor = 2*r + 1 when people are 1-based.
*/

#include <stdio.h>

#define MAX_N 1000

/*
 * josephusSimulation - Eliminates every k-th on a circular array.
 */
int josephusSimulation(int n, int k) {
    int alive[MAX_N];
    for (int i = 0; i < n; i++) {
        alive[i] = 1;
    }

    int remaining = n;
    int index = 0;
    int count = 0;

    printf("Elimination order:\n");
    while (remaining > 1) {
        if (alive[index]) {
            count = count + 1;
            if (count == k) {
                alive[index] = 0;
                remaining = remaining - 1;
                count = 0;
                printf("  eliminated %d (remaining %d)\n", index, remaining);
            }
        }
        index = (index + 1) % n;
    }

    for (int i = 0; i < n; i++) {
        if (alive[i]) {
            return i;
        }
    }
    return -1;
}

/*
 * josephusRecurrence - O(n) math solution, 0-based.
 */
int josephusRecurrence(int n, int k) {
    int survivor = 0;
    for (int m = 2; m <= n; m++) {
        survivor = (survivor + k) % m;
    }
    return survivor;
}

int main() {
    int n;
    int k;

    printf("Josephus Problem\n\n");

    printf("Number of people n (1-%d)? ", MAX_N);
    scanf("%d", &n);

    printf("Count k (every k-th is eliminated)? ");
    scanf("%d", &k);

    if (n < 1 || n > MAX_N || k < 1) {
        printf("Invalid input\n");
        return 1;
    }

    int sim = josephusSimulation(n, k);
    printf("\nSimulation survivor: index %d (person %d)\n", sim, sim + 1);

    int math = josephusRecurrence(n, k);
    printf("Recurrence survivor: index %d (person %d)\n", math, math + 1);

    printf("Both agree: %s\n", (sim == math) ? "YES" : "NO");

    return 0;
}
