/*
Modular Inverse - Division on a Clock (Intermediate)
---------------------------------------------------
PROBLEM DEFINITION:
Find x in [0..m-1] such that a*x mod m = 1. Such an inverse exists
exactly when gcd(a, m) = 1. Unlike ordinary division, it may not exist.

REAL-LIFE EXAMPLE:
Modular arithmetic appears in cyclic schedules and cryptography.
An inverse undoes multiplication when only the remainder is stored.
This small teaching program is NOT a cryptographic implementation.

DIAGRAM:
  modulo 11: 3 --multiply by 4--> 12 --remainder--> 1
  Bezout:    3*4 + 11*(-1) = 1
  Therefore inverse(3, 11) = 4.

PSEUDO CODE:
  Normalize a = (a mod m + m) mod m
  ExtendedGcd(a, b):
      If b == 0: return (a, 1, 0)
      (g, x1, y1) = ExtendedGcd(b, a mod b)
      Return (g, y1, x1 - (a / b)*y1)
  (g, x, y) = ExtendedGcd(a, m)
  If g != 1: report no inverse
  Otherwise: inverse = (x mod m + m) mod m

SAMPLE INPUT & DRY RUN:
  3 11
  Euclid: (3,11) -> (11,3) -> (3,2) -> (2,1) -> (1,0).
  Back-substitution gives 1 = 3*4 - 11; inverse = 4.
  Verification: (3*4) mod 11 = 1.

SPECIAL CASES:
  - 6 modulo 9 has gcd 3: no inverse, even though both are nonzero.
  - 0 has no inverse for any permitted modulus.
  - Negative a is normalized: inverse(-3, 11) = 7.
  - Composite moduli work too: inverse(3, 10) = 7.
  - Require 2 <= m <= 10^9 and -10^9 <= a <= 10^9.
    Products and Bezout calculations fit in long long at these bounds.

COMPLEXITY:
  O(log m) time and O(log m) recursive stack space.

TIPS:
  Fermat's a^(m-2) shortcut requires a PRIME modulus and a nonzero residue.
  Extended Euclid works for any modulus coprime to a. Study gcd-euclid
  first; this lesson applies its coefficients to modular division.
*/

#include <stdio.h>

#define VALUE_LIMIT 1000000000LL

long long extendedGcd(long long a, long long b, long long* x, long long* y) {
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

int main(void) {
    long long a;
    long long m;
    long long x;
    long long y;

    printf("Modular Inverse - Extended Euclidean Algorithm\n\n");
    printf("Enter a and modulus m (a within +/-10^9, m from 2 to 10^9): ");
    if (scanf("%lld %lld", &a, &m) != 2 ||
        a < -VALUE_LIMIT || a > VALUE_LIMIT || m < 2 || m > VALUE_LIMIT) {
        printf("Invalid number or modulus\n");
        return 1;
    }

    long long normalized = (a % m + m) % m;
    long long g = extendedGcd(normalized, m, &x, &y);
    printf("Normalized residue: %lld\n", normalized);
    printf("Bezout: %lld*(%lld) + %lld*(%lld) = %lld\n",
           normalized, x, m, y, g);
    if (g != 1) {
        printf("No modular inverse: gcd = %lld (must be 1)\n", g);
    } else {
        long long inverse = (x % m + m) % m;
        printf("Inverse: %lld\n", inverse);
        printf("Verification: (%lld * %lld) mod %lld = %lld\n",
               normalized, inverse, m, (normalized * inverse) % m);
    }
    return 0;
}
