/*
Chinese Remainder Theorem - Solving Systems of Congruences (Advanced)
--------------------------------------------------------------------
PROBLEM DEFINITION:
Find the smallest non-negative x satisfying every congruence
    x = a1 (mod m1),  x = a2 (mod m2),  ...,  x = ak (mod mk)
or report that no such x exists. The moduli do NOT have to be pairwise
coprime: congruences are merged two at a time with the extended
Euclidean algorithm, and a contradiction is detected exactly.

REAL-LIFE EXAMPLE:
Sun Tzu's soldier count: "in rows of 3, 2 are left over; in rows of 5,
3 are left; in rows of 7, 2 are left - how many soldiers?" Calendar
cycles (weekday + month), gear and planet alignments, and the RSA
decryption speed-up all solve the same kind of system.

DIAGRAM (merging two congruences):
  x = 2 (mod 3):  2   5   8  11  14  17  20  23 ...
  x = 3 (mod 5):  3   8  13  18  23 ...
                      ^ first common value 8  ->  x = 8 (mod 15)
  The merged congruence repeats every lcm(3, 5) = 15 steps.

PSEUDO CODE:
  r = 0, m = 1                          (x = 0 mod 1: every integer)
  For each congruence (a, mi):
      g, p = extendedGcd(m, mi)         (m*p + mi*q = g)
      If (a - r) is not divisible by g: no solution
      t = ((a - r) / g) * p  mod (mi / g)
      r = r + m * t;   m = lcm(m, mi) = m / g * mi
  Answer: x = r (mod m)

SAMPLE INPUT & DRY RUN:
  3
  2 3
  3 5
  2 7
  start: x = 0 (mod 1)
  merge 2 (mod 3): 1*1 + 3*0 = 1 so p = 1; t = 2*1 mod 3 = 2
                   x = 0 + 1*2 = 2 (mod 3)
  merge 3 (mod 5): 3*2 + 5*(-1) = 1 so p = 2; t = (3-2)*2 mod 5 = 2
                   x = 2 + 3*2 = 8 (mod 15)
  merge 2 (mod 7): 15*1 + 7*(-2) = 1 so p = 1; t = (2-8)*1 mod 7 = 1
                   x = 8 + 15*1 = 23 (mod 105)
  Answer: x = 23 (mod 105); check 23%3 = 2, 23%5 = 3, 23%7 = 2.

SPECIAL CASES:
  - Non-coprime moduli: x = 1 (mod 4), x = 3 (mod 6) -> x = 9 (mod 12).
  - Contradiction: x = 1 (mod 4), x = 2 (mod 6) has NO solution
    (the first says x is odd, the second says x is even).
  - Modulus 1 accepts every x; a repeated congruence changes nothing.
  - 1 <= k <= 20, 1 <= mi <= 10^9, 0 <= ai < mi. The combined modulus
    must stay <= 10^18; larger systems are reported, not overflowed.

COMPLEXITY:
  O(k log M) time (one extended Euclid per merge), O(1) extra space.

TIPS:
  Classic CRT formulas assume coprime moduli; the merge form above is
  the version to remember because it also handles the general case.
  Uses the same extended Euclid as modular-inverse. Every product is
  bounded before it is formed: (a-r)/g and p are reduced mod mi/g first.
*/

#include <stdio.h>

#define MAX_K 20
#define MODULUS_LIMIT 1000000000LL
#define COMBINED_LIMIT 1000000000000000000LL

long long extendedGcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    long long x1;
    long long y1;
    long long g = extendedGcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

long long normalize(long long value, long long modulus) {
    value %= modulus;
    return value < 0 ? value + modulus : value;
}

void printProduct(long long a, long long b) {
    printf(b < 0 ? "%lld*(%lld)" : "%lld*%lld", a, b);
}

int main(void) {
    int k;
    long long a[MAX_K];
    long long m[MAX_K];

    printf("Chinese Remainder Theorem - System of Congruences\n\n");
    printf("How many congruences (1-%d)? ", MAX_K);
    if (scanf("%d", &k) != 1 || k < 1 || k > MAX_K) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < k; i++) {
        printf("Congruence %d: remainder a and modulus m (x = a mod m): ",
               i + 1);
        if (scanf("%lld %lld", &a[i], &m[i]) != 2 || m[i] < 1 ||
            m[i] > MODULUS_LIMIT || a[i] < 0 || a[i] >= m[i]) {
            printf("Invalid congruence (need 1 <= m <= 10^9, 0 <= a < m)\n");
            return 1;
        }
    }

    long long r = 0;
    long long mod = 1;
    printf("\nstart: x = 0 (mod 1)\n");
    for (int i = 0; i < k; i++) {
        long long p;
        long long qCoefficient;
        long long g = extendedGcd(mod, m[i], &p, &qCoefficient);
        long long difference = a[i] - r;
        if (difference % g != 0) {
            printf("merge %lld (mod %lld): gcd %lld does not divide %lld\n",
                   a[i], m[i], g, difference);
            printf("\nNo solution: the congruences contradict each other.\n");
            return 0;
        }
        long long step = m[i] / g;
        if (mod / g > COMBINED_LIMIT / m[i]) {
            printf("\nCombined modulus would exceed 10^18 - stopping.\n");
            return 1;
        }
        long long t = normalize(difference / g, step) *
                      normalize(p, step) % step;
        long long previous = mod;
        r += mod * t;
        mod = mod / g * m[i];
        printf("merge %lld (mod %lld): ", a[i], m[i]);
        printProduct(previous, p);
        printf(" + ");
        printProduct(m[i], qCoefficient);
        printf(" = %lld, t = %lld -> x = %lld (mod %lld)\n", g, t, r, mod);
    }

    printf("\nAnswer: x = %lld (mod %lld)\n", r, mod);
    printf("Check:");
    for (int i = 0; i < k; i++) {
        printf(" %lld%%%lld=%lld", r, m[i], r % m[i]);
    }
    printf("\n");
    return 0;
}
