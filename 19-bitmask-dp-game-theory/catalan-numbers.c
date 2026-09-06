/*
catalan Numbers - Counting Balanced Structures
----------------------------------------------
What this program solves:
Generates the catalan sequence 1, 1, 2, 5, 14, 42, ... which counts
countless combinatorial structures: balanced parentheses, binary
tree shapes, triangulations, mountain ranges, non-crossing handshakes.

Real life example:
Counting valid bracket sequences, binary tree shapes and mountain
ranges - UI parsers verify nesting with the same recurrence.

DIAGRAM (C(n) = sum C(i)*C(n-1-i): split at the matched pair):
  C3 = C0*C2 + C1*C1 + C2*C0 = 2 + 1 + 2 = 5
  ((())) (()()) (())() ()(()) ()()()

BEGINNER EXPLANATION (the simplest way to think):
How many ways can n pairs of parentheses be BALANCED? The answer
is catalan(n). The recurrence is a "split at the first matching
pair" argument: fix the OUTERMOST '(' and its matching ')'. Inside
them: a balanced sequence of i pairs. After them: j pairs with
i + j = n - 1. So catalan(n) = sum over i of catalan(i)*catalan(n-1-i)
- the same "split around a root" pattern as counting binary trees!

SUPER SIMPLE EXAMPLE (n = 3):
Balanced strings of 3 pairs: ((())), (()()), (())(), ()(()), ()()()
= 5 = catalan(3). Same count: 5 distinct binary tree shapes with
3 nodes, 5 ways to triangulate a pentagon, 5 mountain ranges.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- catalan(0) = 1: the empty arrangement - one way to do nothing.
- Growth is explosive: catalan(30) = 3814986502092304 (fits long
  long); catalan(35) OVERFLOWS 64 bits - the program caps at 33.
- The naive binomial formula C(2n, n)/(n+1) needs division; the
  multiplicative recurrence C(n+1) = C(n)*2(2n+1)/(n+2) computes
  it with only integer multiply/divide - each step divides evenly,
  no fractions ever appear (worth proving once).
- Pascal-style DP gives the same numbers via the convolution sum.

DRY RUN (DP build):
C(0) = 1
C(1) = C(0)*C(0) = 1
C(2) = C(0)*C(1) + C(1)*C(0) = 2
C(3) = C(0)*C(2) + C(1)*C(1) + C(2)*C(0) = 2+1+2 = 5
C(4) = 5 + 2 + 2 + 5 = 14  (1*5 + 2*2 + 5*1, plus C(0)C(3)+C(3)C(0))
Sequence: 1 1 2 5 14 42 132 429 1430 4862 ...

TIPS:
- Recognition signal: any counting problem whose answer is
  "split at the first/root element and multiply the two sides"
  is catalan. Interviews test this exact recognition.
- Related closed form: catalan(n) = C(2n, n) - C(2n, n+1)
  (the ballot/reflection proof - paths that cross the diagonal).
- Applications in DAA: number of BST shapes with n keys = catalan(n)
  - connects directly to optimal-bst.c in folder 11.
*/

#include <stdio.h>

#define MAX_CATALAN 33

int main() {
    int n;
    long long catalan[MAX_CATALAN + 1];

    printf("catalan Numbers\n\n");

    /* STEP: DP recurrence C(n) = sum C(i)*C(n-1-i) */
    catalan[0] = 1;
    for (int i = 1; i <= MAX_CATALAN; i++) {
        catalan[i] = 0;
        for (int j = 0; j < i; j++) {
            catalan[i] = catalan[i] + catalan[j] * catalan[i - 1 - j];
        }
    }

    printf("Table (DP recurrence):\n");
    for (int i = 0; i <= 10; i++) {
        printf("  catalan(%2d) = %lld\n", i, catalan[i]);
    }

    printf("\nEnter n (0-%d) for a specific value: ", MAX_CATALAN);
    scanf("%d", &n);

    if (n < 0 || n > MAX_CATALAN) {
        printf("n must be 0..%d (larger overflows 64-bit)\n", MAX_CATALAN);
        return 1;
    }

    printf("catalan(%d) = %lld\n", n, catalan[n]);

    /* STEP: multiplicative formula cross-check */
    long long mult = 1;
    for (int i = 0; i < n; i++) {
        mult = mult * 2 * (2 * i + 1) / (i + 2);
    }
    printf("Multiplicative formula agrees: %s (%lld)\n",
           (mult == catalan[n]) ? "YES" : "NO", mult);

    /* STEP: what the numbers count */
    printf("\nCatalan(%d) also counts:\n", n);
    printf("  - balanced parentheses strings with %d pairs\n", n);
    printf("  - binary tree shapes with %d nodes\n", n);
    printf("  - triangulations of a %d-gon\n", n + 2);
    printf("  - monotonic lattice paths not crossing the diagonal\n");

    return 0;
}
