/*
SOS DP - Sum over Subsets in O(n * 2^n) (Master)
-----------------------------------------------
PROBLEM DEFINITION:
Given a value f(S) for every subset S of n items (a bitmask), compute
for every mask M
    F(M) = sum of f(S) over all subsets S of M         (subset sums)
    G(M) = sum of f(S) over all supersets S of M       (superset sums)
Enumerating the submasks of every mask costs O(3^n). "Sum over
Subsets" DP adds one bit dimension at a time - like a prefix sum along
each of the n axes of an n-dimensional 0/1 cube - in O(n * 2^n).

Application in this program: given a list of numbers (bitmasks), answer
"how many numbers are SUBSETS of q?" and "how many are SUPERSETS of q?"
for each query mask q.

REAL-LIFE EXAMPLE:
Skills matching: each job needs a set of skills (bitmask), each
candidate has a set of skills. "How many jobs can this candidate do?"
= number of job masks that are subsets of the candidate's mask. A
recruiter answering this for every candidate uses SOS DP once.

DIAGRAM (n = 3; processing bit 0, then bit 1, then bit 2):
  mask:   000 001 010 011 100 101 110 111
  f:        0   1   0   1   1   1   0   1     (numbers 1 3 4 5 7)
  bit 0:    0   1   0   1   1   2   0   1     F[m] += F[m without bit 0]
  bit 1:    0   1   0   2   1   2   1   3
  bit 2:    0   1   0   2   1   3   1   5     = subsets of each mask
  e.g. F[101] = f(000) + f(001) + f(100) + f(101) = 0 + 1 + 1 + 1 = 3

PSEUDO CODE:
  F = f
  for bit = 0 .. n-1:
      for mask = 0 .. 2^n - 1:
          if mask has bit:  F[mask] += F[mask without bit]
  G = f
  for bit = 0 .. n-1:
      for mask = 0 .. 2^n - 1:
          if mask lacks bit: G[mask] += G[mask with bit]
  After processing bits 0..i, F[mask] sums the S that differ from
  mask only in bits 0..i and are subsets there - induction on i.

SAMPLE INPUT & DRY RUN:
  3
  5
  1 3 4 5 7
  3
  5
  3
  0
  q = 5 (101): subsets among the numbers: 1, 4, 5 -> 3
               supersets: 5, 7 -> 2
  q = 3 (011): subsets 1, 3 -> 2;  supersets 3, 7 -> 2
  q = 0 (000): no number is 0 -> 0 subsets; every number -> 5 supersets
  Work: SOS 3 * 2^2 = 12 additions per table vs 3^3 = 27 submask steps.

SPECIAL CASES:
  - Duplicate numbers count separately (f holds frequencies).
  - Mask 0 is a subset of everything; the full mask a superset of
    nothing but itself.
  - For n <= 12 the program re-checks every F[mask] by enumerating
    submasks the slow way.
  - 1 <= n <= 20 bits, up to 100000 numbers in 0..2^n - 1, up to 100
    queries.

COMPLEXITY:
  O(n * 2^n) time and O(2^n) memory per table; plain submask
  enumeration is O(3^n) (3^20 is 3.5 billion vs 21 million).

TIPS:
  Swap "+" for max, min, OR or gcd and the same loop computes those
  over subsets. Running the loop with "-" undoes it (Mobius inversion),
  which gives inclusion-exclusion over subsets. Classic use: count
  pairs (a, b) with a & b == 0 via F[complement of a].
*/

#include <stdio.h>

#define MAX_BITS 20
#define MAX_NUMBERS 100000
#define MAX_QUERIES 100
#define CHECK_BITS 12
#define TRACE_BITS 4

int subsetSum[1 << MAX_BITS];
int supersetSum[1 << MAX_BITS];
int frequency[1 << MAX_BITS];

void printBinary(int value, int bits) {
    for (int b = bits - 1; b >= 0; b--) putchar((value >> b) & 1 ? '1' : '0');
}

void printRow(const char *label, const int *table, int size) {
    printf("  %-7s", label);
    for (int mask = 0; mask < size; mask++) printf("%5d", table[mask]);
    printf("\n");
}

int main(void) {
    int bits;
    int count;
    int q;

    printf("SOS DP - Sum over Subsets\n\n");
    printf("Number of bits n (1-%d): ", MAX_BITS);
    if (scanf("%d", &bits) != 1 || bits < 1 || bits > MAX_BITS) {
        printf("Invalid bit count\n");
        return 1;
    }
    int size = 1 << bits;
    printf("How many numbers (1-%d)? ", MAX_NUMBERS);
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_NUMBERS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d numbers in 0..%d: ", count, size - 1);
    for (int i = 0; i < count; i++) {
        int x;
        if (scanf("%d", &x) != 1 || x < 0 || x >= size) {
            printf("Invalid number\n");
            return 1;
        }
        frequency[x]++;
    }

    for (int mask = 0; mask < size; mask++) {
        subsetSum[mask] = frequency[mask];
        supersetSum[mask] = frequency[mask];
    }
    if (bits <= TRACE_BITS) {
        printf("\n  mask:  ");
        for (int mask = 0; mask < size; mask++) {
            printf("%*s", 5 - bits, "");
            printBinary(mask, bits);
        }
        printf("\n");
        printRow("f:", frequency, size);
    }
    for (int bit = 0; bit < bits; bit++) {
        for (int mask = 0; mask < size; mask++) {
            if (mask & (1 << bit)) subsetSum[mask] += subsetSum[mask ^ (1 << bit)];
            else supersetSum[mask] += supersetSum[mask | (1 << bit)];
        }
        if (bits <= TRACE_BITS) {
            char label[32];
            snprintf(label, sizeof(label), "bit %d:", bit);
            printRow(label, subsetSum, size);
        }
    }

    long long sosWork = (long long)bits * (size / 2);
    long long naiveWork = 1;
    for (int i = 0; i < bits; i++) naiveWork *= 3;
    printf("\nAdditions per table: SOS %lld, submask enumeration %lld\n",
           sosWork, naiveWork);

    if (bits <= CHECK_BITS) {
        int same = 1;
        for (int mask = 0; mask < size && same; mask++) {
            int total = frequency[0];
            for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
                total += frequency[sub];
            }
            if (total != subsetSum[mask]) same = 0;
        }
        printf("Submask-enumeration check: %s\n", same ? "same" : "MISMATCH");
    }

    printf("\nHow many queries (1-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        int mask;
        printf("Query %d: mask (0..%d): ", i + 1, size - 1);
        if (scanf("%d", &mask) != 1 || mask < 0 || mask >= size) {
            printf("Invalid mask\n");
            return 1;
        }
        printf("q = %d (", mask);
        printBinary(mask, bits);
        printf("): %d subset%s, %d superset%s\n", subsetSum[mask],
               subsetSum[mask] == 1 ? "" : "s", supersetSum[mask],
               supersetSum[mask] == 1 ? "" : "s");
    }
    return 0;
}
