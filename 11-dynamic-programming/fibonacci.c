/*
 PROBLEM DEFINITION:
 --------------------
 The Fibonacci sequence is defined as:
     F(0) = 0,  F(1) = 1
     F(n) = F(n-1) + F(n-2)   for n >= 2

 This program demonstrates WHY Dynamic Programming matters, by running
 three implementations side by side and comparing their behavior:
   1. Naive recursion        - exponential blowup
   2. Memoization (Top-Down) - recursion + cache
   3. Tabulation (Bottom-Up) - iterative table
 plus an O(1)-space optimized version and a Matrix Exponentiation teaser.

 REAL-LIFE EXAMPLE - Rabbit Population & Caching in Software:
 -------------------------------------------------------------
 Fibonacci originally modeled rabbit breeding: each pair needs one month
 to mature, then produces a new pair monthly - pairs grow as 1, 1, 2, 3,
 5, 8, 13... The naive recursion is like a clerk who, for every order,
 re-derives ALL previous orders from scratch instead of keeping records.
 Memoization is giving the clerk a filing cabinet (cache): look before
 computing. Tabulation is a factory assembly line: compute rung by rung
 from the bottom, never repeating work. Caches, spreadsheets, and build
 systems all exploit these same two ideas.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 1. NAIVE RECURSION - the wasteful clerk:
        fib(n) = fib(n-1) + fib(n-2)
    Problem: fib(5) recomputes fib(3) twice, fib(2) three times...
    Call count roughly DOUBLES with each increment -> O(2^n).
    fib(50) would need ~2^50 calls (over a quadrillion) - unusable.

 2. MEMOIZATION (Top-Down DP) - recursion with a memory:
    - Keep an array memo[] initialized to -1 ("unknown").
    - Before computing fib(k), check memo[k]:
        If already computed, return the stored answer instantly.
        Otherwise compute once, store in memo[k], return.
    - Each subproblem solved exactly once -> O(n).

 3. TABULATION (Bottom-Up DP) - build the ladder from the ground:
    - table[0] = 0, table[1] = 1
    - For i = 2..n: table[i] = table[i-1] + table[i-2]
    - No recursion at all -> O(n) time, O(n) space, no stack risk.

 4. SPACE OPTIMIZATION - keep only the last two values:
    - We never need the whole table, only table[i-1] and table[i-2].
    - Track two variables, overwrite as we go -> O(n) time, O(1) space.

 PSEUDO CODE:
 ------------
 START
   Function fibNaive(n):
     If n <= 1: Return n
     Return fibNaive(n-1) + fibNaive(n-2)

   Function fibMemo(n, memo[]):
     If n <= 1: Return n
     If memo[n] != -1: Return memo[n]
     memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo)
     Return memo[n]

   Function fibTable(n):
     table[0] = 0, table[1] = 1
     For i = 2 to n:
         table[i] = table[i-1] + table[i-2]
     Return table[n]

   Function fibOptimized(n):
     If n <= 1: Return n
     prev = 0, curr = 1
     For i = 2 to n:
         next = prev + curr
         prev = curr
         curr = next
     Return curr
 END

 SAMPLE INPUT & DRY RUN (n = 6, Tabulation):
 --------------------------------------------
 table[0] = 0
 table[1] = 1
 i=2: table[2] = 1 + 0 = 1
 i=3: table[3] = 1 + 1 = 2
 i=4: table[4] = 2 + 1 = 3
 i=5: table[5] = 3 + 2 = 5
 i=6: table[6] = 5 + 3 = 8

 Sequence: 0 1 1 2 3 5 8

 Naive recursion call tree for fib(5):
                     fib(5)
                   /        \
              fib(4)          fib(3)
             /      \        /      \
        fib(3)    fib(2)  fib(2)  fib(1)      <- fib(3), fib(2) repeated!
        /    \    /    \   /    \
   fib(2) fib(1) ...    ...    ...           <- exponential waste
   Total calls: 15 for n=5; for n=40: 331,160,281 calls!

 With memoization: only 6 computations for n=5 - each solved once.

 TIME COMPLEXITY:
 - Naive recursion:      O(2^n)    - exponential, unusable beyond n~40
 - Memoization:          O(n)      - each subproblem once
 - Tabulation:           O(n)      - each cell filled once
 - Space-optimized:      O(n) time, O(1) space

 COMMON USE CASES:
 - Introducing DP thinking (the classic gateway problem)
 - Caching strategies (memoization = cache lookups)
 - Spreadsheet recalculation (tabulation order)
 - Stock span/path counting variants, stair-climbing problems
DIAGRAM (naive vs DP for fib(5)):
  naive:             fib(5)          recomputes fib(3) etc.
                    /      \
               fib(4)      fib(3)      O(2^n)
              /     \      /    \
          fib(3) fib(2) fib(2) fib(1)
  DP table: 0 1 1 2 3 5  each cell = sum of previous two -> O(n)

*/

#include <stdio.h>

#define MAX 100

/*
 * Global call counters to PROVE the complexity difference empirically.
 */
long long naiveCalls = 0;
long long memoCalls = 0;

/*
 * fibNaive - Pure recursion, no caching. Exponential time.
 * Recomputes the same subproblems again and again.
 */
long long fibNaive(int n) {
    naiveCalls++;
    if (n <= 1) {
        return n;
    }
    return fibNaive(n - 1) + fibNaive(n - 2);
}

/*
 * fibMemo - Top-Down DP: recursion + cache array.
 * memo[] holds -1 for "not computed yet".
 */
long long fibMemo(int n, long long memo[]) {
    memoCalls++;
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    return memo[n];
}

/*
 * fibTable - Bottom-Up DP: fill the table iteratively.
 * No recursion, no stack overflow risk.
 */
long long fibTable(int n, int printSteps) {
    long long table[MAX];
    table[0] = 0;
    table[1] = 1;

    if (printSteps) {
        printf("  table[0] = 0\n");
        printf("  table[1] = 1\n");
    }

    for (int i = 2; i <= n; i++) {
        table[i] = table[i - 1] + table[i - 2];
        if (printSteps) {
            printf("  table[%d] = %lld + %lld = %lld\n",
                   i, table[i - 1], table[i - 2], table[i]);
        }
    }

    return table[n];
}

/*
 * fibOptimized - O(1) space: keep only the last two values.
 * Analogy: climbing a ladder while remembering only the last two rungs.
 */
long long fibOptimized(int n) {
    if (n <= 1) {
        return n;
    }

    long long prev = 0;
    long long curr = 1;

    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

int main() {
    int n;

    printf("Fibonacci - Naive vs Memoization vs Tabulation\n\n");

    printf("Enter n (0-%d): ", MAX - 1);
    scanf("%d", &n);

    if (n < 0 || n >= MAX) {
        printf("Invalid n\n");
        return 1;
    }

    /* STEP: naive recursion with call count (slow for large n) */
    naiveCalls = 0;
    long long r1 = fibNaive(n);
    printf("\nNaive recursion: fib(%d) = %lld  (calls made: %lld)\n",
           n, r1, naiveCalls);

    /* STEP: memoization - top-down with cache */
    long long memo[MAX];
    for (int i = 0; i < MAX; i++) {
        memo[i] = -1;
    }

    memoCalls = 0;
    long long r2 = fibMemo(n, memo);
    printf("Memoization:     fib(%d) = %lld  (calls made: %lld)\n",
           n, r2, memoCalls);

    /* STEP: tabulation - bottom-up table */
    printf("Tabulation:      fib(%d) = %lld\n", n, fibTable(n, 0));

    /* STEP: O(1) space version */
    printf("Optimized:       fib(%d) = %lld\n", n, fibOptimized(n));

    return 0;
}
