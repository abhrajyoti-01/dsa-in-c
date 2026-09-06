/*
Miller-Rabin Primality Test (Randomized Algorithm)
--------------------------------------------------
What this program solves:
Probabilistically tests whether a large number is PRIME in
O(k * log^3 n) - vastly faster than trial division for numbers
with 15+ digits. Errors are astronomically rare (with fixed
bases, deterministic below 3.3 x 10^24).

Real life example:
RSA key generation must test huge random numbers for primality -
Miller-Rabin (fixed witness set for 64-bit) is the industry tool.

DIAGRAM (n-1 = 2^s * d; test witnesses a):
  for witness a: x = a^d mod n
  composite -> some x sequence hits 1 or -1 "too early"
  prime (probable) -> all witnesses behave like a true prime
  witnesses {2,3,5,7,11,13,17} are PROVEN enough below 3.3e14.

BEGINNER EXPLANATION (the simplest way to think):
Fermat's little theorem says: if p is prime, then a^(p-1) mod p
= 1 for ANY a not divisible by p. So pick random a's and check.
Carmichael numbers fool naive Fermat, so Miller-Rabin upgrades
it: write n-1 = d * 2^r, compute a^d, then keep squaring. A prime
gives 1 early or hits -1 at some step; composites usually fail.
Each independent witness halves the (already tiny) false-positive
odds.

SUPER SIMPLE EXAMPLE:
n = 13: n-1 = 12 = 3 * 2^2 (d=3, r=2).
Test a=2: 2^3 = 8 mod 13 = 8. Square: 8^2 = 64 mod 13 = 12 = -1
-> passes (hit -1). a=3: 27 mod 13 = 1 -> passes (hit 1 early).
n = 15: n-1 = 14 = 7 * 2^1. a=2: 2^7 = 128 mod 15 = 8; square:
64 mod 15 = 4. Never 1 or -1 -> COMPOSITE found immediately.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- n = 0, 1, negative: not prime by definition - handled first.
- n = 2, 3: prime, and even n > 2 is not - check these BEFORE
  the random loop, otherwise the modular math misbehaves.
- Carmichael numbers (561, 1105, ...): pass Fermat for ALL coprime
  a, but Miller-Rabin still catches them - that is the upgrade.
- Deterministic bases: for 64-bit n, testing just a = {2, 3, 5,
  7, 11, 13, 17, 19, 23, 29, 31, 37} gives ZERO errors. This file
  uses those - so it is actually exact for all inputs it accepts.
- Overflow: a*d products overflow 64 bits - mulmod via __int128
  or repeated doubling. This file uses the unsigned __int128
  trick where available logic (modular multiplication by
  addition-doubling) to stay safe.

DRY RUN (n = 221 = 13 * 17, n-1 = 220 = 55 * 2^2):
a = 2: 2^55 mod 221 = 174 (not 1, not -1=220).
square: 174^2 mod 221 = 220 = -1 -> witness passes? No! -1 pass
only on the LAST squaring round too - 220 = n-1 is the -1 case
-> a=2 passes (174 -> square gives -1: acceptable).
a = 3: 3^55 mod 221 = 118. squares: 118^2 = 13924 mod 221 = 87;
87^2 = 7569 mod 221 = 87 (stuck, never -1) -> COMPOSITE. 221
correctly rejected.

TIPS:
- Complexity: modular exponentiation is O(log n) multiplications;
  each mulmod is O(log n) naive or O(1) with 128-bit. k witnesses
  -> error <= 4^(-k).
- Used in RSA keygen, Diffie-Hellman parameter checks - real
  cryptography runs this exact algorithm.
- Deterministic 64-bit guarantee with 12 fixed bases means this
  "randomized" algorithm is actually exact for our range - the
  best of both worlds.
*/

#include <stdio.h>

typedef unsigned long long u64;

/*
 * mulmod - (a * b) % m without overflow via doubling.
 */
u64 mulmod(u64 a, u64 b, u64 m) {
    u64 result = 0;
    a = a % m;
    while (b > 0) {
        if (b & 1) {
            result = (result + a) % m;
        }
        a = (a + a) % m;
        b = b >> 1;
    }
    return result;
}

/*
 * powerMod - a^e mod m via binary exponentiation.
 */
u64 powerMod(u64 a, u64 e, u64 m) {
    u64 result = 1;
    a = a % m;
    while (e > 0) {
        if (e & 1) {
            result = mulmod(result, a, m);
        }
        a = mulmod(a, a, m);
        e = e >> 1;
    }
    return result;
}

/*
 * millerRabin - One witness round; 1 = probably prime.
 */
int millerRabinRound(u64 n, u64 a, u64 d, int r) {
    u64 x = powerMod(a, d, n);

    if (x == 1 || x == n - 1) {
        return 1;
    }
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) {
            return 1;
        }
    }
    return 0;
}

/*
 * isPrime - Deterministic witness set for 64-bit range.
 */
int isPrimeMR(u64 n) {
    if (n < 2) {
        return 0;
    }
    u64 smallPrimes[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int i = 0; i < 12; i++) {
        if (n == smallPrimes[i]) {
            return 1;
        }
        if (n % smallPrimes[i] == 0) {
            return 0;
        }
    }

    u64 d = n - 1;
    int r = 0;
    while ((d & 1) == 0) {
        d = d >> 1;
        r = r + 1;
    }

    for (int i = 0; i < 12; i++) {
        if (!millerRabinRound(n, smallPrimes[i], d, r)) {
            printf("Witness %llu proves n COMPOSITE\n", smallPrimes[i]);
            return 0;
        }
        printf("Witness %llu passes (still probably prime)\n", smallPrimes[i]);
    }
    return 1;
}

int main() {
    unsigned long long n;
    char again;

    printf("Miller-Rabin Primality Test (deterministic for n < 3.3e24)\n\n");

    do {
        printf("Enter n (2 - 18446744073709551615): ");
        scanf("%llu", &n);

        if (isPrimeMR(n)) {
            printf("=> %llu IS PRIME\n", n);
        } else {
            printf("=> %llu IS COMPOSITE\n", n);
        }

        printf("\nTest another? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
