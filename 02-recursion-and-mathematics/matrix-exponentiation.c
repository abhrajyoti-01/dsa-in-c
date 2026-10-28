/*
Matrix Exponentiation - Linear Recurrences in O(k^3 log n) (Intermediate -> Advanced)
-------------------------------------------------------------------------------------
PROBLEM DEFINITION:
Compute the n-th term of a linear recurrence of order k,
    f(n) = c1*f(n-1) + c2*f(n-2) + ... + ck*f(n-k)   (mod 1000000007)
for n as large as 10^18. Looping n times is impossible; raising a k x k
"companion" matrix to the n-th power by repeated squaring needs only
O(log n) matrix multiplications.

REAL-LIFE EXAMPLE:
Population models (rabbits breeding every month), counting tilings of a
2 x n floor, and counting passwords under "no three equal symbols in a
row" rules are all linear recurrences. A planner asking about month
10^18 needs the answer instantly, not after 10^18 loop iterations.

DIAGRAM (Fibonacci, k = 2):
  [ f(n+1) ]   [ 1 1 ] [ f(n)   ]          [ f(n+1) ]         [ f(1) ]
  [ f(n)   ] = [ 1 0 ] [ f(n-1) ]   so     [ f(n)   ] = M^n * [ f(0) ]

  General companion matrix (k = 3, coefficients c1 c2 c3):
      [ c1 c2 c3 ]     row 0 builds the next term
      [ 1  0  0  ]     rows 1.. shift the window down by one place
      [ 0  1  0  ]

PSEUDO CODE:
  If n < k: return f(n) directly
  Build M (k x k): M[0][j] = c(j+1); M[i][i-1] = 1 for i >= 1
  Result = identity; Base = M; e = n
  While e > 0:
      If e is odd: Result = Result * Base
      Base = Base * Base;  e = e / 2
  state(0) = [f(k-1), ..., f(1), f(0)]
  f(n) = last entry of Result * state(0)

SAMPLE INPUT & DRY RUN:
  2
  1 1
  0 1
  3
  10
  50
  1000000000000000000
  Fibonacci: M = [[1,1],[1,0]], state(0) = [f(1), f(0)] = [1, 0].
  n = 10 = 1010 in binary, so Result = M^2 * M^8:
      M^2 = [[2,1],[1,1]]   M^4 = [[5,3],[3,2]]   M^8 = [[34,21],[21,13]]
      M^10 = [[89,55],[55,34]];  M^10 * [1, 0] = [89, 55]  ->  f(10) = 55
  n = 50: f(50) = 12586269025, printed mod 10^9+7 as 586268941
  n = 10^18: only 60 squarings -> 209783453
  For n <= 10^6 the program re-checks the answer with a plain loop.

SPECIAL CASES:
  - n < k returns the given initial value without any matrix work.
  - k = 1 is a geometric sequence: f(n) = c1^n * f(0).
  - Zero coefficients are fine (e.g. f(n) = f(n-2) uses c1 = 0).
  - All arithmetic is mod 10^9+7; products of two residues fit in
    long long (< 1.1 * 10^18), so no overflow.
  - 1 <= k <= 10, 0 <= coefficients, initial values < 10^9+7,
    0 <= n <= 10^18, at most 100 queries.

COMPLEXITY:
  O(k^3 log n) time per query, O(k^2) space.
  Plain loop: O(n k) - hopeless for n = 10^18.

TIPS:
  The same squaring loop as fast-power, but on matrices. Any recurrence
  with constant coefficients fits: add a row of 1s to carry a constant
  term, or more rows to carry sums. Compare fibonacci (the teaser) and
  fast-power (the scalar version).
*/

#include <stdio.h>
#include <string.h>

#define MAX_K 10
#define MOD 1000000007LL
#define MAX_QUERIES 100
#define N_LIMIT 1000000000000000000LL
#define LOOP_CHECK_LIMIT 1000000LL

typedef struct {
    long long cell[MAX_K][MAX_K];
} Matrix;

int order;
long long coefficient[MAX_K];
long long initial[MAX_K];

Matrix multiply(const Matrix *a, const Matrix *b) {
    Matrix product;
    memset(&product, 0, sizeof(product));
    for (int i = 0; i < order; i++) {
        for (int t = 0; t < order; t++) {
            if (a->cell[i][t] == 0) continue;
            for (int j = 0; j < order; j++) {
                product.cell[i][j] = (product.cell[i][j] +
                                      a->cell[i][t] * b->cell[t][j]) % MOD;
            }
        }
    }
    return product;
}

Matrix power(Matrix base, long long exponent) {
    Matrix result;
    memset(&result, 0, sizeof(result));
    for (int i = 0; i < order; i++) result.cell[i][i] = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiply(&result, &base);
        base = multiply(&base, &base);
        exponent >>= 1;
    }
    return result;
}

long long matrixTerm(long long n) {
    if (n < order) return initial[n];
    Matrix companion;
    memset(&companion, 0, sizeof(companion));
    for (int j = 0; j < order; j++) companion.cell[0][j] = coefficient[j];
    for (int i = 1; i < order; i++) companion.cell[i][i - 1] = 1;

    Matrix p = power(companion, n);
    long long value = 0;
    for (int j = 0; j < order; j++) {
        value = (value + p.cell[order - 1][j] * initial[order - 1 - j]) % MOD;
    }
    return value;
}

/* Plain O(n k) loop over a circular window; buffer[i % k] holds f(i). */
long long loopTerm(long long n) {
    long long buffer[MAX_K];
    for (int i = 0; i < order; i++) buffer[i] = initial[i];
    if (n < order) return initial[n];
    for (long long i = order; i <= n; i++) {
        long long next = 0;
        for (int j = 1; j <= order; j++) {
            next = (next + coefficient[j - 1] * buffer[(i - j) % order]) % MOD;
        }
        buffer[i % order] = next;
    }
    return buffer[n % order];
}

int main(void) {
    int q;

    printf("Matrix Exponentiation - Linear Recurrences\n\n");
    printf("Order k of the recurrence (1-%d): ", MAX_K);
    if (scanf("%d", &order) != 1 || order < 1 || order > MAX_K) {
        printf("Invalid order\n");
        return 1;
    }
    printf("Coefficients c1..c%d (0..%lld): ", order, MOD - 1);
    for (int i = 0; i < order; i++) {
        if (scanf("%lld", &coefficient[i]) != 1 || coefficient[i] < 0 ||
            coefficient[i] >= MOD) {
            printf("Invalid coefficient\n");
            return 1;
        }
    }
    printf("Initial values f(0)..f(%d) (0..%lld): ", order - 1, MOD - 1);
    for (int i = 0; i < order; i++) {
        if (scanf("%lld", &initial[i]) != 1 || initial[i] < 0 ||
            initial[i] >= MOD) {
            printf("Invalid initial value\n");
            return 1;
        }
    }
    printf("How many queries (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        long long n;
        printf("Query %d: n (0..10^18): ", i + 1);
        if (scanf("%lld", &n) != 1 || n < 0 || n > N_LIMIT) {
            printf("Invalid n\n");
            return 1;
        }
        long long value = matrixTerm(n);
        printf("f(%lld) mod %lld = %lld", n, MOD, value);
        if (n <= LOOP_CHECK_LIMIT) {
            printf(loopTerm(n) == value ? "  (loop check: same)"
                                        : "  (loop check: MISMATCH)");
        }
        printf("\n");
    }
    return 0;
}
