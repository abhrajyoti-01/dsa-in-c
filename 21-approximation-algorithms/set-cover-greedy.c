/*
Set Cover - Greedy ln(n)-Approximation (Advanced)
------------------------------------------------
PROBLEM DEFINITION:
A universe U = {1..u} and a family of sets, each with a cost, are
given. Choose sets whose union is U with minimum total cost. This is
NP-hard. The greedy algorithm repeatedly picks the set with the lowest
cost per NEWLY covered element; its total cost is at most H(d) times
the optimum, where d is the size of the largest set and
H(d) = 1 + 1/2 + ... + 1/d  (about ln d). For up to 20 sets the
program also finds the exact optimum by trying every combination.

REAL-LIFE EXAMPLE:
Choose which radio towers to rent so that every village has coverage,
or which vaccination clinics to open so that every district is
served, at minimum total cost. Test-suite minimisation (cover every
code path with the fewest tests) is the same problem.

DIAGRAM (sample, unit costs, U = {1..6}):
      A = {1,2,3}     B = {4,5,6}     C = {1,2,4,5}
      1  2  3  4  5  6
  A:  x  x  x
  B:           x  x  x
  C:  x  x     x  x
  greedy: C first (covers 4 new), then A (covers 3), then B (covers 6)
          -> 3 sets.  optimum: A + B -> 2 sets.

PSEUDO CODE:
  covered = empty; total = 0
  while covered != U:
      pick the set S minimizing cost(S) / |S - covered|
          (ties: lower index; sets adding nothing are skipped)
      if no set adds anything: U cannot be covered
      covered = covered + S;  total += cost(S)
  Guarantee: total <= H(max set size) * OPT
  (charge each set's cost equally to the elements it newly covers;
   each element is charged at most OPT / (elements still uncovered)).

SAMPLE INPUT & DRY RUN:
  6 3
  1 3 1 2 3
  1 3 4 5 6
  1 4 1 2 4 5
  Each set line is: cost, size, then the elements.
  step 1: cost per new element: A 1/3, B 1/3, C 1/4  -> pick C
  step 2: A adds {3}: 1/1;  B adds {6}: 1/1  -> tie, pick A
  step 3: only B adds anything ({6})          -> pick B
  greedy cost 3; exact optimum 2 (A + B); ratio 1.50
  bound H(4) = 1 + 1/2 + 1/3 + 1/4 = 2.08, and 1.50 <= 2.08

SPECIAL CASES:
  - If the union of all sets misses an element, no cover exists.
  - Duplicate elements in one set are counted once.
  - Equal ratios are compared exactly by cross-multiplying
    (cost_a * new_b vs cost_b * new_a), never with floating point.
  - 1 <= u <= 60 (bitmasks of 64 bits), 1 <= sets <= 60,
    costs 1..1000; exact search only for <= 20 sets.

COMPLEXITY:
  Greedy: O(sets * u) per step, at most u steps -> O(sets * u^2).
  Exact: O(2^sets * sets).

TIPS:
  Unless P = NP, no polynomial algorithm beats ln(n) by a constant
  factor, so greedy is essentially the best possible. Compare
  vertex-cover-approx (a special case: every edge is an element, every
  vertex a set of edges) and the exchange arguments in 09-greedy.
*/

#include <stdio.h>

#define MAX_UNIVERSE 60
#define MAX_SETS 60
#define EXACT_LIMIT 20
#define COST_LIMIT 1000

typedef unsigned long long Mask;

int universe;
int setCount;
Mask members[MAX_SETS];
int cost[MAX_SETS];
int setSize[MAX_SETS];

int popcount(Mask x) {
    int count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
}

/* Sets are named A, B, C, ... (S27, S28, ... beyond 26 sets). */
void printSetName(int i) {
    if (i < 26) printf("%c", 'A' + i);
    else printf("S%d", i + 1);
}

void printSet(Mask mask) {
    printf("{");
    int first = 1;
    for (int e = 1; e <= universe; e++) {
        if ((mask >> (e - 1)) & 1ULL) {
            printf(first ? "%d" : ",%d", e);
            first = 0;
        }
    }
    printf("}");
}

int main(void) {
    printf("Set Cover - Greedy Approximation\n\n");
    printf("Universe size u (1-%d) and number of sets (1-%d): ", MAX_UNIVERSE,
           MAX_SETS);
    if (scanf("%d %d", &universe, &setCount) != 2 || universe < 1 ||
        universe > MAX_UNIVERSE || setCount < 1 || setCount > MAX_SETS) {
        printf("Invalid size\n");
        return 1;
    }
    Mask full = (1ULL << universe) - 1;
    Mask reachable = 0;
    int largest = 0;
    for (int i = 0; i < setCount; i++) {
        int k;
        printf("Set ");
        printSetName(i);
        printf(": cost, size, elements (1..%d): ", universe);
        if (scanf("%d %d", &cost[i], &k) != 2 || cost[i] < 1 ||
            cost[i] > COST_LIMIT || k < 0 || k > universe) {
            printf("Invalid set\n");
            return 1;
        }
        for (int j = 0; j < k; j++) {
            int e;
            if (scanf("%d", &e) != 1 || e < 1 || e > universe) {
                printf("Invalid element\n");
                return 1;
            }
            members[i] |= 1ULL << (e - 1);
        }
        setSize[i] = popcount(members[i]);
        if (setSize[i] > largest) largest = setSize[i];
        reachable |= members[i];
    }
    if (reachable != full) {
        printf("\nNo cover exists; never covered: ");
        printSet(full & ~reachable);
        printf("\n");
        return 0;
    }

    Mask covered = 0;
    int greedyCost = 0;
    int step = 0;
    printf("\n");
    while (covered != full) {
        int best = -1;
        int bestNew = 0;
        for (int i = 0; i < setCount; i++) {
            int fresh = popcount(members[i] & ~covered);
            if (fresh == 0) continue;
            if (best < 0 || (long long)cost[i] * bestNew < (long long)cost[best] * fresh) {
                best = i;
                bestNew = fresh;
            }
        }
        step++;
        printf("step %d: pick set ", step);
        printSetName(best);
        printf(" (cost %d) covering %d new: ", cost[best], bestNew);
        printSet(members[best] & ~covered);
        printf("\n");
        covered |= members[best];
        greedyCost += cost[best];
    }

    double harmonic = 0.0;
    for (int i = 1; i <= largest; i++) harmonic += 1.0 / i;
    printf("\nGreedy total cost: %d\n", greedyCost);
    printf("Guarantee: <= H(%d) = %.2f times the optimum\n", largest, harmonic);

    if (setCount <= EXACT_LIMIT) {
        int bestCost = -1;
        unsigned int bestChoice = 0;
        for (unsigned int choice = 1; choice < (1u << setCount); choice++) {
            Mask unionMask = 0;
            int total = 0;
            for (int i = 0; i < setCount; i++) {
                if ((choice >> i) & 1u) {
                    unionMask |= members[i];
                    total += cost[i];
                }
            }
            if (unionMask == full && (bestCost < 0 || total < bestCost)) {
                bestCost = total;
                bestChoice = choice;
            }
        }
        printf("Exact optimum: %d using sets", bestCost);
        for (int i = 0; i < setCount; i++) {
            if ((bestChoice >> i) & 1u) {
                printf(" ");
                printSetName(i);
            }
        }
        printf("\nActual ratio: %.2f\n", (double)greedyCost / bestCost);
    }
    return 0;
}
