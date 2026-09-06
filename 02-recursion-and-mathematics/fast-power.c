/*
 PROBLEM DEFINITION:
 --------------------
 Fast Exponentiation (also called Exponentiation by Squaring or Binary
 Exponentiation) computes base^exponent in O(log N) multiplications
 instead of O(N). The idea:
     If n is even:  b^n = (b^(n/2))^2
     If n is odd:   b^n = b * (b^(n/2))^2
 Each step HALVES the exponent, giving logarithmic performance. It also
 handles modular exponentiation (b^n mod m), which is essential in
 cryptography where numbers are astronomically large.

 REAL-LIFE EXAMPLE - Cryptography (RSA) & Doubling Games:
 ---------------------------------------------------------
 RSA encryption computes messages^hugeKey mod prime - exponents with
 hundreds of digits. Naive multiplication (10^100 steps) is impossible;
 binary exponentiation finishes in ~log2(key) steps (~hundreds).
 The same halving idea appears in paper folding: fold once -> 2 layers,
 twice -> 4, k times -> 2^k. To reach 2^n layers you don't add sheets
 one by one - you keep DOUBLING, n halvings of the exponent.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of the exponent in binary. To compute b^13:
     13 = 1101 in binary = 8 + 4 + 1
     b^13 = b^8 * b^4 * b^1
 Repeatedly square b (b, b^2, b^4, b^8...) and multiply the running
 answer whenever the current binary bit is 1.

 1. RECURSIVE VIEW:
     power(b, n):
        If n == 0: return 1                      (base case)
        half = power(b, n / 2)
        If n is even: return half * half         (square the half)
        Else:         return b * half * half     (extra factor of b)

 2. ITERATIVE VIEW (bit scanning):
     result = 1
     While n > 0:
        If n is odd:  result = result * b
        b = b * b                 (square the base)
        n = n / 2                 (shift to next binary digit)

 3. MODULAR VERSION:
     Apply % m after every multiplication to keep numbers small:
        result = (result * b) % m
        b = (b * b) % m

 WHY IT'S FAST:
     Naive:  b^1024 needs 1023 multiplications.
     Fast:   only 10 squarings (1024 = 2^10 -> log2 steps).
     For n = 10^18: naive is impossible; fast needs ~60 steps.

 PSEUDO CODE:
 ------------
 START
   Function FastPower(b, n):
     If n == 0: Return 1
     half = FastPower(b, n / 2)
     If n is even: Return half * half
     Else:         Return b * half * half

   Function ModPower(b, n, m):
     result = 1
     b = b mod m
     While n > 0:
         If n is odd:
             result = (result * b) mod m
         b = (b * b) mod m
         n = n / 2
     Return result
 END

 SAMPLE INPUT & DRY RUN (2^13, iterative):
 -------------------------------------------
 n = 13 (binary 1101), result = 1, b = 2

 Iteration 1: n=13 odd  -> result = 1*2 = 2;  b = 4;  n = 6
 Iteration 2: n=6  even -> skip;              b = 16; n = 3
 Iteration 3: n=3  odd  -> result = 2*16 = 32; b = 256; n = 1
 Iteration 4: n=1  odd  -> result = 32*256 = 8192; b = 65536; n = 0

 Answer: 8192. Multiplications used: ~6 (vs 12 for naive loop).

 Trace matches theory: 13 = 1101 -> used b^1, b^4, b^8 (bits 0,2,3).

 TIME COMPLEXITY: O(log N) multiplications
 SPACE COMPLEXITY: O(1) iterative, O(log N) recursion stack

 COMMON USE CASES:
 - RSA, Diffie-Hellman, and all public-key cryptography
 - Modular arithmetic in competitive programming
 - Computing large powers of matrices (Fibonacci in O(log n))
 - Repeated-squaring tricks in numerical methods
DIAGRAM (power(3, 13) by binary exponentiation, 13 = 1101):
  base=3, result=1
  bit 1 (n odd)  -> result *= 3        result = 3
  base = 9
  bit 0 (n even) -> skip               base squared each step
  bit 1          -> result *= 9        result = 27
  base = 81
  bit 1          -> result *= 81       result = 2187 = 3^13
  4 steps instead of 12 multiplications.

*/

#include <stdio.h>

/*
 * powerNaive - Simple loop multiplication. O(N) - the baseline.
 */
long long powerNaive(long long base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

/*
 * powerFast - Recursive exponentiation by squaring. O(log N).
 * Halves the exponent every call - like folding the problem in half.
 */
long long powerFast(long long base, int exp, int* mults) {
    if (exp == 0) {
        return 1;
    }

    long long half = powerFast(base, exp / 2, mults);

    if (exp % 2 == 0) {
        (*mults)++;
        return half * half;
    } else {
        (*mults) += 2;
        return base * half * half;
    }
}

/*
 * modPower - Iterative modular exponentiation. O(log N).
 * Keeps every intermediate value reduced mod m so results never overflow.
 * This is THE function at the heart of RSA.
 */
long long modPower(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}

/*
 * gcd - Euclidean algorithm, used for the Fermat little-theorem demo.
 */
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main() {
    long long base;
    long long exp;
    long long mod;

    printf("Fast Exponentiation - Power in O(log N)\n\n");

    printf("Enter base: ");
    scanf("%lld", &base);

    printf("Enter exponent (0-62 for plain result): ");
    scanf("%lld", &exp);

    if (exp < 0) {
        printf("Negative exponents not supported\n");
        return 1;
    }

    /* STEP: naive vs fast with multiplication counts (small exponents) */
    if (exp <= 18) {
        int fastMults = 0;
        long long r1 = powerNaive(base, (int)exp);
        long long r2 = powerFast(base, (int)exp, &fastMults);

        printf("\n%s^%lld = %lld\n", "base", exp, r1);
        printf("Naive multiplications: %lld\n", exp);
        printf("Fast multiplications:  %d  (O(log N))\n", fastMults);
        if (r1 != r2) {
            printf("WARNING: results differ!\n");
        }
    } else {
        printf("\nExponent too large for plain 64-bit result (%lld^%lld overflows)\n",
               base, exp);
    }

    /* STEP: modular exponentiation - works for huge exponents */
    printf("\nEnter modulus m (for base^exp mod m): ");
    scanf("%lld", &mod);

    if (mod <= 0) {
        printf("Invalid modulus\n");
        return 1;
    }

    printf("base^exp mod m = %lld\n", modPower(base, exp, mod));

    return 0;
}
