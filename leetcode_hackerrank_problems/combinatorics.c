/*
Combinatorics - nCr, nPr, Catalan, Grid Paths (Basic -> Advanced)
-----------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 62  - Unique Paths (binomial coefficient)
  LeetCode 96  - Unique Binary Search Trees (Catalan numbers)
  LeetCode 118 - Pascal's Triangle
  HackerRank "nCr" / "Handshake" style counting tasks

PROBLEM DEFINITION:
Count selections without listing them: ordered arrangements nPr,
unordered selections nCr, balanced bracket / grid monotone paths
(Catalan), and the number of monotone lattice paths across an r x c
grid - all computable in exact integer arithmetic.

REAL-LIFE EXAMPLE:
Lottery odds (how many tickets are possible?), staffing a shift from
a pool of candidates, and counting valid bracket sequences in a
compiler front end are all these four formulas.

DIAGRAM (grid paths on a 2x2 grid of cells = 3x3 lattice points):
  (0,0) -> (2,2) moving only right/down:
    RRDD  RDRD  RDDR  DRRD  DRDR  DDRR                 = 6 paths
  C(2+2, 2) = C(4,2) = 6                (choose 2 of 4 steps)
  Catalan(3) = C(6,3) - C(6,2) = 20 - 15 = 5
    ((())) (()()) (())() ()(()) ()()()                 = 5 bracket strings

PSEUDO CODE:
  Pascal's rule: C(n,k) = C(n-1,k-1) + C(n-1,k),  C(n,0)=C(n,n)=1
  nPr(n,r) = n! / (n-r)!  =  nCr(n,r) * r!
  Catalan(n) = C(2n,n) / (n+1)
  GridPaths(rows,cols) = C(rows+cols, rows)

SAMPLE INPUT & DRY RUN:
  2                       <- how many nCr tests
  5 2                     <- nCr(5,2) = 10, nPr(5,2) = 20
  6 0                     <- nCr(6,0) = 1, nPr(6,0) = 1 (the empty pick)
  1                       <- how many grid-path tests
  3 2                     <- paths across a 3x2 grid = C(5,3) = 10
  nCr(5,2) = C(5,2) = 10   (5*4/(2*1))
  nPr(5,2) = 10 * 2 = 20
  Grid paths for 3 rows and 2 columns = C(5,3) = 10.
  Catalan numbers 0..7: 1 1 2 5 14 42 132 429.

SPECIAL CASES:
  - k > n: nCr = nPr = 0 (no valid selection) - not an error.
  - Negative inputs are rejected, and r > n is reported as 0 rather than
    treated as an error (the pair simply selects nothing).
  - All arithmetic is exact integer: nCr is built by multiplying and
    dividing in a stated order, avoiding intermediate overflow.
  - Catalan(0) = 1 (the empty sequence), Catalan(20) = 6564120420.
  - Pascal table size 32 overflows long long at C(67,33); the program
    therefore caps n at 62 and validates the bound.
  - nPr overflows long long far sooner than nCr: 21! = 5.1 * 10^19
    already exceeds 9.22 * 10^18, so nPr is only trustworthy while the
    running product stays below LLONG_MAX. The loop therefore SATURATES
    and reports the exact result only when it fits; it prints
    "overflow" otherwise instead of silently wrapping.
  - n <= 62, r <= 62, grid dimensions <= 30, count of tests <= 1000.

COMPLEXITY:
  Pascal table O(n^2) build, O(1) per query; a single nCr is O(k)
  with the multiplicative formula, Catalan is O(1) after the table.

VALIDATION:
  buildBinomials(n) is called before every query, then rebuilt to
  MAX_N for the Catalan section; nCr(0,0) = 1 and nCr(n,0) = 1 are
  filled by the base cases. nPr returns 0 for r > n and clamps to
  LLONG_MAX with a flag when the product would overflow.

TIPS:
  Never compute factorials then divide - overflow. Multiply first,
  divide exactly, and keep the running value integral (the /i step in
  the sample code). For huge n and a prime modulus use the modular
  inverse from 02-recursion-and-mathematics/modular-inverse.c.
*/

#include <stdio.h>
#include <limits.h>

#define MAX_N 62
#define MAX_TESTS 1000

long long binomial[MAX_N + 1][MAX_N + 1];

void buildBinomials(int n) {
    for (int i = 0; i <= n; i++) {
        binomial[i][0] = 1;
        binomial[i][i] = 1;
        for (int j = 1; j < i; j++) {
            binomial[i][j] = binomial[i - 1][j - 1] + binomial[i - 1][j];
        }
    }
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return binomial[n][r];
}

/* Returns nPr(n, r). Sets *overflow to 1 when the exact value does not
 * fit in long long, in which case LLONG_MAX is returned as a sentinel. */
long long nPr(int n, int r, int* overflow) {
    *overflow = 0;
    if (r < 0 || r > n) return 0;
    long long result = 1;
    for (int i = 0; i < r; i++) {
        long long factor = (long long)(n - i);
        if (result > LLONG_MAX / factor) {
            *overflow = 1;
            return LLONG_MAX;
        }
        result *= factor;
    }
    return result;
}

long long catalan(int n) {
    if (n == 0) return 1;
    return binomial[2 * n][n] / (n + 1);
}

int main(void) {
    int n;
    int r;
    int rows;
    int cols;

    printf("Combinatorics - nCr, nPr, Catalan, Grid Paths\n\n");
    printf("How many nCr tests (1-%d)? ", MAX_TESTS);
    int tests;
    if (scanf("%d", &tests) != 1 || tests < 1 || tests > MAX_TESTS) {
        printf("Invalid test count\n");
        return 1;
    }
    printf("Enter 'n r' (0 <= r <= n <= %d):\n", MAX_N);
    for (int i = 0; i < tests; i++) {
        if (scanf("%d %d", &n, &r) != 2 || n < 0 || n > MAX_N) {
            printf("Invalid n\n");
            return 1;
        }
        if (r < 0 || r > MAX_N) {
            printf("Invalid r (0 <= r <= %d)\n", MAX_N);
            return 1;
        }
        buildBinomials(n);
        int overflow = 0;
        long long arrangements = nPr(n, r, &overflow);
        printf("nCr(%d,%d) = %lld, nPr(%d,%d) = ", n, r, nCr(n, r), n, r);
        if (overflow) {
            printf("overflow (exceeds %lld; use the modular variant)\n",
                   LLONG_MAX);
        } else {
            printf("%lld\n", arrangements);
        }
    }

    buildBinomials(MAX_N);
    printf("\nCatalan numbers 0..7:");
    for (int i = 0; i <= 7; i++) printf(" %lld", catalan(i));
    printf("\n");

    printf("\nHow many grid-path tests (1-%d)? ", MAX_TESTS);
    if (scanf("%d", &tests) != 1 || tests < 1 || tests > MAX_TESTS) {
        printf("Invalid test count\n");
        return 1;
    }
    printf("Enter 'rows cols' (1..30 each):\n");
    for (int i = 0; i < tests; i++) {
        if (scanf("%d %d", &rows, &cols) != 2 ||
            rows < 1 || rows > 30 || cols < 1 || cols > 30) {
            printf("Invalid grid size\n");
            return 1;
        }
        printf("Monotone paths across a %dx%d grid = %lld\n", rows, cols,
               nCr(rows + cols, rows));
    }
    return 0;
}
