/*
Sieve of Eratosthenes - Prime Generation
----------------------------------------
What this program solves:
Generates all primes up to n by iteratively marking multiples of
each prime as composite - the fastest simple method, O(n log log n).

Real life example:
Cryptography key generation needs large primes; sieves are the
standard way to build prime tables. Also used in competitive
programming for factorization and divisor-counting precomputation.

DIAGRAM (sieve up to 30 - cross multiples, keep survivors):
  2 3 4 5 6 7 8 9 10 ... 30
  cross multiples of 2: 4 6 8 10 12 ...
  cross multiples of 3: 6 9 12 15 ...   (already gone: 6, 12)
  cross 4? skipped - already crossed. stop at sqrt(30) ~ 5
  survivors: 2 3 5 7 11 13 17 19 23 29

How the process works:
1. isPrime[0..n] all true; 0 and 1 marked false.
2. For p from 2 while p*p <= n:
   if isPrime[p], mark p*p, p*p+p, ... up to n as composite.
3. Everything still marked prime is prime.
4. Early stop at sqrt(n): any composite <= n has a factor <= sqrt(n).

Pseudo code:
START
isPrime[2..n] = true
FOR p = 2..sqrt(n)
    IF isPrime[p]:
        FOR m = p*p, p*p+p, ... <= n: isPrime[m] = false
OUTPUT all i with isPrime[i]
END

Sample input and dry run:
n = 30 -> primes 2 3 5 7 11 13 17 19 23 29
(4 marked by 2, 6 by 2, 9 by 3, ...)

Main logic to understand:
Work happens only on multiples of primes: total markings
n/2 + n/3 + n/5 + ... = O(n log log n). The p*p start skips
redundant marking of smaller multiples. Memory O(n) bits/bools.
*/

#include <stdio.h>
#include <math.h>

#define MAX_N 1000000

int main() {
    static int isPrime[MAX_N + 1];
    int n;

    printf("Sieve of Eratosthenes\n\n");

    printf("Generate primes up to n (2-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 2 || n > MAX_N) {
        printf("Enter n between 2 and %d\n", MAX_N);
        return 1;
    }

    for (int i = 0; i <= n; i++) {
        isPrime[i] = 1;
    }
    isPrime[0] = 0;
    isPrime[1] = 0;

    /* STEP: cross out multiples of each prime */
    int limit = (int)sqrt((double)n);
    for (int p = 2; p <= limit; p++) {
        if (isPrime[p]) {
            for (long long m = (long long)p * p; m <= n; m = m + p) {
                isPrime[m] = 0;
            }
        }
    }

    printf("\nPrimes up to %d:\n", n);
    int count = 0;
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            printf("%d ", i);
            count = count + 1;
            if (count % 15 == 0) {
                printf("\n");
            }
        }
    }
    printf("\n\nTotal primes: %d\n", count);

    return 0;
}
