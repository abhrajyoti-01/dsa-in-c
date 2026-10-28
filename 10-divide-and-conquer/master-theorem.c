/*
Master Theorem - Solving Divide-and-Conquer Recurrences (Intermediate)
---------------------------------------------------------------------
PROBLEM DEFINITION:
Given a recurrence  T(n) = a * T(n / b) + n^d  (a >= 1, b >= 2, d >= 0),
state its growth rate using the Master Theorem, then CHECK the answer
numerically: evaluate the recurrence exactly at n = b^j and show that
T(n) divided by the predicted bound settles to a constant.

REAL-LIFE EXAMPLE:
Before coding a divide-and-conquer algorithm, an engineer estimates its
cost from three numbers: how many subproblems (a), how much smaller
each is (b), and how much work splitting/combining takes (n^d). That
estimate decides between merge sort, Karatsuba or Strassen in practice.

DIAGRAM (recursion tree for T(n) = a T(n/b) + n^d):
  level 0:            n^d                     work n^d
  level 1:   (n/b)^d ... a copies ...         work a * (n/b)^d
  level 2:   a^2 copies of (n/b^2)^d          work n^d * (a / b^d)^2
  ...        ratio per level r = a / b^d
  r < 1: the ROOT dominates     -> Theta(n^d)              (case 1)
  r = 1: every level is equal   -> Theta(n^d * log n)      (case 2)
  r > 1: the LEAVES dominate    -> Theta(n^(log_b a))      (case 3)

PSEUDO CODE:
  Compare a with b^d (integers, no rounding problems):
      a < b^d: answer n^d
      a = b^d: answer n^d log n
      a > b^d: answer n^(log_b a), log_b a = log2(a) / log2(b)
  Check: T(1) = 1; T(b^j) = a * T(b^(j-1)) + (b^j)^d
         print T(n) / bound(n) for growing n = b^j
  log2(x) without <math.h>: halve x into [1, 2), then repeatedly square
  it; each squaring that reaches 2 contributes the next binary digit.

SAMPLE INPUT & DRY RUN:
  5
  2 2 1
  1 2 0
  3 2 1
  7 2 2
  1 2 1
  2 2 1 (merge sort):    a = 2 = 2^1        -> case 2: Theta(n log n)
  1 2 0 (binary search): a = 1 = 2^0        -> case 2: Theta(log n)
  3 2 1 (Karatsuba):     a = 3 > 2^1 = 2    -> case 3: Theta(n^1.585)
  7 2 2 (Strassen):      a = 7 > 2^2 = 4    -> case 3: Theta(n^2.807)
  1 2 1 (quickselect, good pivots): 1 < 2   -> case 1: Theta(n)
  Merge sort check: T(2^j) = 2^j (j + 1), so T(n) / (n log2 n)
  = (j + 1) / j -> 2.0000, 1.5000, 1.2500, ... approaching 1.

SPECIAL CASES:
  - d = 0 means constant work per call; case 2 then gives Theta(log n).
  - The ratio converges slowly in case 2 ((j+1)/j) and in cases 1 and 3
    to a constant other than 1 - that constant is what Theta hides.
  - Driving functions that are not polynomials (n log n, n / log n)
    fall in the "gaps" of the basic theorem; e.g. T(n) = 2T(n/2) +
    n log n is Theta(n log^2 n) by the extended version.
  - 1 <= a <= 1000, 2 <= b <= 1000, 0 <= d <= 5, at most 20 inputs.
    The check stops at n <= 10^15 so every value fits in a double.

COMPLEXITY:
  O(log_b N) arithmetic per recurrence for the numeric check.

TIPS:
  Memorize the comparison "a versus b^d" - it is the whole theorem for
  polynomial work. Read the result as "who dominates: root, all levels
  equally, or leaves". Recurrences that do not shrink by a factor
  (T(n) = T(n-1) + n) need the substitution or recursion-tree method.
*/

#include <stdio.h>

#define MAX_INPUTS 20
#define N_LIMIT 1e15

double log2Approx(double x) {
    double result = 0.0;
    while (x >= 2.0) {
        x /= 2.0;
        result += 1.0;
    }
    double bit = 0.5;
    for (int i = 0; i < 40; i++) {
        x *= x;
        if (x >= 2.0) {
            x /= 2.0;
            result += bit;
        }
        bit /= 2.0;
    }
    return result;
}

double powerDouble(double base, int exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; i++) result *= base;
    return result;
}

/* Compares a with b^d without overflow: returns -1, 0 or +1. */
int compareWithPower(long long a, long long b, int d) {
    long long value = 1;
    for (int i = 0; i < d; i++) {
        value *= b;
        if (value > a) return -1;
    }
    return a < value ? -1 : (a > value ? 1 : 0);
}

void printPolynomialPart(int d) {
    if (d == 1) printf("n");
    else if (d > 1) printf("n^%d", d);
}

int main(void) {
    int count;

    printf("Master Theorem - T(n) = a T(n/b) + n^d\n\n");
    printf("How many recurrences (1-%d)? ", MAX_INPUTS);
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_INPUTS) {
        printf("Invalid count\n");
        return 1;
    }
    for (int r = 0; r < count; r++) {
        long long a;
        long long b;
        int d;
        printf("Recurrence %d: a b d: ", r + 1);
        if (scanf("%lld %lld %d", &a, &b, &d) != 3 || a < 1 || a > 1000 ||
            b < 2 || b > 1000 || d < 0 || d > 5) {
            printf("Invalid recurrence\n");
            return 1;
        }

        int relation = compareWithPower(a, b, d);
        double exponent = log2Approx((double)a) / log2Approx((double)b);
        printf("\nT(n) = %lld T(n/%lld) + ", a, b);
        if (d == 0) printf("1");
        else printPolynomialPart(d);
        printf("\n  compare a = %lld with b^d = %lld^%d = %.0f:  a %s b^d;  "
               "log_%lld(%lld) = %.3f\n", a, b, d, powerDouble((double)b, d),
               relation < 0 ? "<" : (relation == 0 ? "=" : ">"), b, a,
               exponent);
        printf("  case %d: T(n) = Theta(", relation < 0 ? 1 : (relation == 0 ? 2 : 3));
        if (relation < 0) {
            if (d == 0) printf("1");
            else printPolynomialPart(d);
        } else if (relation == 0) {
            printPolynomialPart(d);
            printf(d == 0 ? "log n" : " log n");
        } else {
            printf("n^%.3f", exponent);
        }
        printf(")\n");

        printf("  check T(n) / bound:");
        double t = 1.0;
        double n = 1.0;
        int printed = 0;
        int nextRow = 1;
        int j = 0;
        while (n * b <= N_LIMIT) {
            j++;
            n *= b;
            t = a * t + powerDouble(n, d);
            int last = n * b > N_LIMIT;
            if (j == nextRow || last) {
                double bound;
                if (relation < 0) bound = powerDouble(n, d);
                else if (relation == 0) bound = powerDouble(n, d) * j;
                else bound = powerDouble((double)a, j);
                printf("%s n=%.0f: %.4f", printed % 3 == 0 ? "\n   " : "", n,
                       t / bound);
                printed++;
                if (j == nextRow) nextRow *= 2;
            }
        }
        printf("\n");
    }
    return 0;
}
