/*
Pascal's Triangle and Binomial Coefficients
-------------------------------------------
What this program solves:
Builds Pascal's triangle and answers "C(n, k)?" three ways:
DP table (Pascal's rule), multiplicative formula, and the
recursive definition - showing why the last one is exponentially
bad. The bridge from 01-fundamentals patterns to DP (folder 11).

Real life example:
"n choose k" counts lottery combinations and network redundancy
paths; each entry is the DP table of combinations built bottom-up.

DIAGRAM (each cell = cell above + cell above-left):
  1
  1 1
  1 2 1
  1 3 3 1
  1 4 6 4 1    e.g. 6 = 3 + 3  -> C(4,2)

BEGINNER EXPLANATION:
Each entry is the SUM of the two entries above it (Pascal's rule:
C(n,k) = C(n-1,k-1) + C(n-1,k)). The edges are 1. The table row n
gives the coefficients of (a+b)^n. It also counts combinations:
C(n,k) = "choose k items from n".

SUPER SIMPLE EXAMPLE:
C(5, 2): how many pairs from {A,B,C,D,E}? 10. Pascal row 5:
1 5 10 10 5 1 - third entry is 10. Direct formula:
5*4/2! = 10.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- C(n, 0) = C(n, n) = 1 always (choose nothing / choose all).
- k > n: 0 ways (impossible) - the formula must return 0, not
  garbage.
- Naive recursion C(n,k) = C(n-1,k-1) + C(n-1,k) recomputes
  massively - C(30,15) alone makes millions of calls. That is the
  same blow-up as naive fibonacci; DP table fixes it identically.
- Overflow: C(34, 17) exceeds 64-bit. The multiplicative formula
  computed INTERLEAVED (multiply then divide at each step) stays
  exact longer than factorial ratios, but still overflows past
  ~C(62,31). The program caps inputs.
- k > n/2: use symmetry C(n,k) = C(n, n-k) to halve the work.

DRY RUN (DP table, n = 5):
row0: 1
row1: 1 1
row2: 1 2 1
row3: 1 3 3 1
row4: 1 4 6 4 1
row5: 1 5 10 10 5 1
C(4,2) = row4[2] = 6 = 4*3/2 (multiplicative agrees).

TIPS:
- Pascal's rule IS a DP recurrence in disguise - same table,
  same base cases, same fill order as knapsack-style DP later.
- Multiplicative formula: result = result * (n-k+i) / i computed
  i = 1..k in that exact order guarantees exact integer division
  at every step (prefix products are integers).
- C(n,k) counts: teams, lattice paths (right/down moves), hands
  of cards - and appears in Miller-Rabin's binomial analysis.
*/

#include <stdio.h>

#define MAX_N 30

long long pascal[MAX_N + 1][MAX_N + 1];

long long binomialRecursive(int n, int k, long long* calls) {
    *calls = *calls + 1;
    if (k == 0 || k == n) {
        return 1;
    }
    if (k < 0 || k > n) {
        return 0;
    }
    return binomialRecursive(n - 1, k - 1, calls) +
           binomialRecursive(n - 1, k, calls);
}

int main() {
    int n;
    int k;

    printf("Pascal's Triangle / Binomial Coefficients\n\n");

    /* STEP: DP table build */
    for (int i = 0; i <= MAX_N; i++) {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    printf("First rows of Pascal's triangle:\n");
    for (int i = 0; i <= 8; i++) {
        printf("  ");
        for (int j = 0; j <= i; j++) {
            printf("%lld ", pascal[i][j]);
        }
        printf("\n");
    }

    printf("\nEnter n and k for C(n, k): ");
    scanf("%d %d", &n, &k);

    if (n < 0 || n > MAX_N || k < 0 || k > n) {
        if (k > n) {
            printf("C(%d, %d) = 0 (cannot choose more than available)\n",
                   n, k);
        } else {
            printf("n must be 0..%d\n", MAX_N);
        }
        return 0;
    }

    printf("DP table:            C(%d, %d) = %lld\n",
           n, k, pascal[n][k]);

    /* STEP: multiplicative formula cross-check */
    long long mult = 1;
    int kk = (k > n - k) ? n - k : k;
    for (int i = 1; i <= kk; i++) {
        mult = mult * (n - kk + i) / i;
    }
    printf("Multiplicative:      C(%d, %d) = %lld\n", n, k, mult);

    /* STEP: naive recursion with call counter (only for small n) */
    if (n <= 20) {
        long long calls = 0;
        long long rec = binomialRecursive(n, k, &calls);
        printf("Naive recursion:     C(%d, %d) = %lld in %lld calls!\n",
               n, k, rec, calls);
        printf("  (DP table built ALL %d rows with ~%d additions)\n",
               MAX_N + 1, MAX_N * MAX_N / 2);
    } else {
        printf("Naive recursion skipped for n > 20 (exponential!)\n");
    }

    return 0;
}
