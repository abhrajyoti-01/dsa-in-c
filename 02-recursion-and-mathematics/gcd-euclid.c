/*
GCD - Euclidean Algorithm
-------------------------
What this program solves:
Computes the Greatest Common Divisor of two integers using the
Euclidean algorithm (modulo recursion), plus LCM and the extended
version showing Bezout coefficients.

Real life example:
Tiling a floor of 24 x 36 feet with the LARGEST square tiles: the
biggest square that tiles both dimensions exactly has side
gcd(24, 36) = 12 feet. Music (beat synchronization) and calendar
cycling (LCM) use the same math.

DIAGRAM (gcd(48, 18) - keep replacing with the remainder):
  48 mod 18 = 12
  18 mod 12 = 6
  12 mod 6  = 0  -> gcd = 6
  each step shrinks the numbers FAST (like a Fibonacci spiral).

How the process works:
1. gcd(a, b) = gcd(b, a mod b); gcd(a, 0) = a.
   Every step shrinks b below a, guaranteeing termination.
2. lcm(a, b) = a / gcd(a, b) * b.
3. Extended: gcd = a*x + b*y; coefficients come from the recursion
   unwinding: x = yPrev, y = xPrev - (a/b)*yPrev.

Pseudo code:
START
GCD(a, b): IF b == 0: RETURN a ELSE RETURN GCD(b, a mod b)
EXTENDED(a, b):
    IF b == 0: RETURN (a, 1, 0)
    (g, x1, y1) = EXTENDED(b, a mod b)
    RETURN (g, y1, x1 - (a/b)*y1)
END

Sample input and dry run:
gcd(48, 18): 48 mod 18 = 12 -> 18 mod 12 = 6 -> 12 mod 6 = 0
gcd = 6; lcm = 48/6*18 = 144; 6 = 48*(-1) + 18*(3)

Main logic to understand:
The remainder strictly decreases and any common divisor of (a, b)
also divides a mod b - so the problem shrinks without losing
solutions. O(log min(a, b)) steps - among the oldest and fastest
algorithms ever written (Euclid, ~300 BC).
*/

#include <stdio.h>

/*
 * gcd - Recursive Euclidean algorithm.
 */
long long gcd(long long a, long long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

/*
 * extendedGcd - Bezout coefficients via recursion.
 */
void extendedGcd(long long a, long long b, long long* g,
                 long long* x, long long* y) {
    if (b == 0) {
        *g = a;
        *x = 1;
        *y = 0;
        return;
    }
    long long x1;
    long long y1;
    extendedGcd(b, a % b, g, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
}

int main() {
    long long a;
    long long b;

    printf("GCD - Euclidean Algorithm\n\n");

    printf("Enter two positive integers: ");
    scanf("%lld %lld", &a, &b);

    if (a <= 0 || b <= 0) {
        printf("Positive integers only\n");
        return 1;
    }

    long long g = gcd(a, b);
    long long lcm = a / g * b;

    printf("\nGCD(%lld, %lld) = %lld\n", a, b, g);
    printf("LCM(%lld, %lld) = %lld\n", a, b, lcm);

    /* STEP: extended Euclid showing Bezout coefficients */
    long long g2;
    long long x;
    long long y;
    extendedGcd(a, b, &g2, &x, &y);
    printf("Bezout: %lld = %lld*(%lld) + %lld*(%lld)\n",
           g2, a, x, b, y);
    printf("Verification: a*x + b*y == gcd -> %s\n",
           (a * x + b * y == g2) ? "YES" : "NO");

    return 0;
}
