/*
Math Problems Ladder - GCD variations, LCM, Primes in range, Armstrong ranges
------------------------------------------------------------------------------
Real life example:
Scheduling systems compute LCM of periods ("every 3 days and every
4 days coincide every 12 days"); prime ranges power hash sizing.

DIAGRAM (LCM via GCD, primes in a range):
  lcm(4, 6) = 4 * 6 / gcd(4, 6) = 24 / 2 = 12
  primes 10..20: cross multiples of 2, 3 (<= sqrt(20)) -> 11 13 17 19

LADDER (easy -> hard inside this file):
  1. GCD of two numbers          (Euclid)
  2. GCD of an ARRAY             (fold gcd over elements)
  3. LCM of two / array          (a/g*b, fold)
  4. Prime factorization         (trial division, largest prime factor)
  5. Perfect numbers in a range  (reuse of number_checks logic)
  6. Reverse digits & overflow   (check before multiply by 10)

BEGINNER EXPLANATION:
- GCD(a, b) = GCD(b, a mod b) - Euclid's 2000-year-old shortcut.
  For an ARRAY, fold: g = gcd(g, next) - order doesn't matter.
- LCM = a / gcd * b (divide FIRST to avoid overflow).
- Prime factorization: divide out 2s, then odd candidates up to
  sqrt(remaining) - each divisor found is prime automatically.

SUPER SIMPLE EXAMPLE (factor 60):
60 / 2 = 30 (2^1), 30 / 2 = 15 (2^2), 15 % 2 != 0 stop.
try 3: 15 / 3 = 5 (3^1). try 5: 5 / 5 = 1 (5^1).
60 = 2^2 * 3 * 5.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- GCD(0, x) = x; GCD(x, 0) = x - the base case IS the answer.
- GCD of array containing 0s: gcd(0,0) = 0 - all zeros gives 0.
- LCM overflow: a * b can exceed 64-bit even when LCM fits - the
  divide-first form a / g * b is the standard fix.
- Factorization of 1: empty (1 has no prime factors).
- Reverse overflow: 1534236469 reversed exceeds INT_MAX - check
  before each * 10 step (or use long long and compare). This
  program reports overflow instead of silently wrapping.

DRY RUN (GCD(48, 18) array-style):
g = 48. fold 18: gcd(48, 18) -> 48 % 18 = 12 -> gcd(18, 12) ->
6 -> gcd(12, 6) -> gcd(6, 0) = 6.
LCM = 48 / 6 * 18 = 144.

TIPS:
- LCM/GCD fold is how you compute "when do all cycles sync?" -
  calendar and gear-ratio problems are direct applications.
- Prime factorization powers: divisor counting (product of
  (exponent+1)), gcd normalization of fractions, and RSA's core.
- The overflow-checked reverse is a famous interview problem -
  the pre-check `if (rev > (INT_MAX - d) / 10)` is the pattern.
*/

#include <stdio.h>
#include <limits.h>

long long gcdTwo(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long lcmTwo(long long a, long long b) {
    long long g = gcdTwo(a, b);
    return a / g * b;
}

int main() {
    int choice;
    int n;

    printf("Math Problems Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. GCD of an array\n");
        printf("2. LCM of an array\n");
        printf("3. Prime factorization\n");
        printf("4. Perfect numbers in a range\n");
        printf("5. Reverse digits (overflow-safe)\n");
        printf("6. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1 || choice == 2) {
            printf("How many numbers (1-20)? ");
            scanf("%d", &n);
            if (n < 1 || n > 20) {
                printf("Invalid count\n");
                continue;
            }
            printf("Enter %d positive integers: ", n);
            long long v;
            long long g = 0;
            long long l = 1;
            int zeroFlag = 0;
            for (int i = 0; i < n; i++) {
                scanf("%lld", &v);
                if (v == 0) {
                    zeroFlag = 1;
                    continue;
                }
                if (choice == 1) {
                    g = gcdTwo(g, v);
                } else {
                    l = lcmTwo(l, v);
                }
            }
            if (choice == 1) {
                printf("GCD = %lld\n", g);
            } else {
                printf("LCM = %lld%s\n", l, zeroFlag ? " (0s ignored)" : "");
            }
        } else if (choice == 3) {
            long long v;
            printf("Enter number (>1): ");
            scanf("%lld", &v);
            if (v < 2) {
                printf("1 and below have no prime factorization\n");
                continue;
            }
            printf("%lld = ", v);
            int first = 1;
            long long p = 2;
            while (p * p <= v) {
                int power = 0;
                while (v % p == 0) {
                    v = v / p;
                    power = power + 1;
                }
                if (power > 0) {
                    if (!first) {
                        printf(" * ");
                    }
                    printf("%lld^%d", p, power);
                    first = 0;
                }
                p = p + 1;
            }
            if (v > 1) {
                if (!first) {
                    printf(" * ");
                }
                printf("%lld^1", v);
            }
            printf("\n");
        } else if (choice == 4) {
            int lo;
            int hi;
            printf("Range lo hi: ");
            scanf("%d %d", &lo, &hi);
            printf("Perfect numbers: ");
            int found = 0;
            for (int num = lo; num <= hi && num > 0; num++) {
                int sum = 1;
                for (int d = 2; (long long)d * d <= num; d++) {
                    if (num % d == 0) {
                        sum = sum + d;
                        if (d != num / d) {
                            sum = sum + num / d;
                        }
                    }
                }
                if (sum == num && num > 1) {
                    printf("%d ", num);
                    found = 1;
                }
            }
            if (!found) {
                printf("(none in range)");
            }
            printf("\n");
        } else if (choice == 5) {
            int v;
            printf("Enter non-negative integer: ");
            scanf("%d", &v);
            int original = v;
            int reversed = 0;
            int overflow = 0;
            while (v > 0) {
                int d = v % 10;
                if (reversed > (INT_MAX - d) / 10) {
                    overflow = 1;
                    break;
                }
                reversed = reversed * 10 + d;
                v = v / 10;
            }
            if (overflow) {
                printf("Reversed %d overflows INT_MAX\n", original);
            } else {
                printf("Reversed: %d (palindrome: %s)\n",
                       reversed, reversed == original ? "YES" : "NO");
            }
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
