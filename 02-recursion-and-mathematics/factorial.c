/*
Factorial - Recursion vs Iteration (Recursion Basics)
-----------------------------------------------------
What this program solves:
Computes n! both recursively and iteratively, showing the call
stack growing and shrinking - THE first program to understand
recursion before touching trees, D&C, and backtracking.

Real life example:
Folding a paper chain: n! counts every ORDER of n items - seating
arrangements, password permutations, race rankings all grow
factorially.

DIAGRAM (call stack for factorial(4)):
  fact(4) = 4 * fact(3)
                fact(3) = 3 * fact(2)
                              fact(2) = 2 * fact(1)
                                            fact(1) = 1  <- base case
  unwinds: 2*1=2 -> 3*2=6 -> 4*6=24

BEGINNER EXPLANATION (the simplest way to think):
n! = n x (n-1) x (n-2) x ... x 1. The recursive insight:
"n! is just (n-1)! times n" - a smaller copy of the same problem.
Recursion = a function that calls a SMALLER version of itself until
it hits a question it can answer without asking again (base case).

SUPER SIMPLE EXAMPLE:
4! = 4 x 3!
   = 4 x (3 x 2!)
   = 4 x 3 x (2 x 1!)
   = 4 x 3 x 2 x 1 = 24
Each "x 3!" step is one stack frame; 1! = 1 is the base case that
stops the descent and answers start flowing back up.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- 0! = 1, not 0. The empty product is defined as 1 (there is one
  way to arrange zero items: do nothing).
- 1! = 1.
- Negative input: factorial is undefined - the program rejects it.
- n >= 21 overflows a 64-bit long long (21! > 2^63). The program
  caps input at 20. For bigger n you need big-number arithmetic.
- Recursion depth: n nested calls sit on the call stack at once -
  fine for n = 20, dangerous for n = 100000 (stack overflow). The
  iterative loop has no such limit.

DRY RUN (recursive factorial(4)):
fact(4) -> needs fact(3)
  fact(3) -> needs fact(2)
    fact(2) -> needs fact(1)
      fact(1) -> base case, returns 1        (deepest point)
    fact(2) = 2 * 1 = 2
  fact(3) = 3 * 2 = 6
fact(4) = 4 * 6 = 24
The call stack grows 4 deep, then unwinds - watch the printed
indentation for this exact shape.

TIPS:
- Every recursion needs TWO things: a base case (when to stop) and
  progress toward it (n-1 here). Forgetting the base case = stack
  overflow - the #1 beginner recursion bug.
- Recursion cost here: n multiplications either way; recursion
  adds stack overhead. Choose iteration when the problem is just a
  straight line; choose recursion when the problem branches (trees,
  mazes, D&C).
- Trailing zeros in n! count = floor(n/5) + floor(n/25) + ... -
  how many times 5 divides the product. Classic interview twist.
*/

#include <stdio.h>

/*
 * factorialRecursive - n! via self-call with base case.
 */
long long factorialRecursive(int n) {
    if (n <= 1) {
        printf("  base case: fact(%d) = 1\n", n);
        return 1;
    }
    printf("  calling fact(%d), needs fact(%d) first\n", n, n - 1);
    long long smaller = factorialRecursive(n - 1);
    printf("  returning fact(%d) = %d * fact(%d) = %lld\n",
           n, n, n - 1, n * smaller);
    return (long long)n * smaller;
}

/*
 * factorialIterative - Simple loop, no stack usage.
 */
long long factorialIterative(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int main() {
    int n;

    printf("Factorial - Recursion vs Iteration\n\n");

    printf("Enter n (0-20): ");
    scanf("%d", &n);

    if (n < 0 || n > 20) {
        printf("n must be 0..20 (21! overflows 64-bit)\n");
        return 1;
    }

    printf("\n--- Recursive trace ---\n");
    long long r1 = factorialRecursive(n);
    printf("fact(%d) = %lld\n\n", n, r1);

    long long r2 = factorialIterative(n);
    printf("--- Iterative ---\n");
    printf("fact(%d) = %lld\n", n, r2);

    printf("\nBoth agree: %s\n", (r1 == r2) ? "YES" : "NO");

    /* STEP: trailing zeros bonus */
    int zeros = 0;
    long long p = 5;
    while (p <= n) {
        zeros = zeros + (int)(n / p);
        p = p * 5;
    }
    printf("Trailing zeros in %d!: %d\n", n, zeros);

    return 0;
}
