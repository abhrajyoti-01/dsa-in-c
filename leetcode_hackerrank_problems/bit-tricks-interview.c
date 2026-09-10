/*
Bit Tricks Interview Problems - Single Number, Counting Bits, Power of
Two, Subsets via Bits, XOR Pairing (Intermediate -> Advanced)
--------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 136 - Single Number (XOR cancels pairs)
  LeetCode 137 - Single Number II (every other value appears 3 times)
  LeetCode 191 - Number of 1 Bits (popcount)
  LeetCode 338 - Counting Bits 0..n (DP on bits)
  LeetCode 231 - Power of Two
  LeetCode 78  - Subsets generated with a bitmask (no recursion)
  HackerRank "Lonely Integer" and "Maximizing XOR" families

PROBLEM DEFINITION:
Six bit-level classics. Each replaces an array/hash/counting step with
arithmetic on the binary representation: XOR cancels pairs, a single
subtraction isolates the lowest set bit, and a bitmask counter
enumerates subsets without recursion.

REAL-LIFE EXAMPLE:
Parity and checksum digits, hardware register flags (one bit per
feature), and permission masks in file systems are bit tricks used
every day; the XOR pairing trick also finds the odd device in a paired
inventory.

DIAGRAM (single number in [4,1,2,1,2]):
  4 ^ 1 = 5; 5 ^ 2 = 7; 7 ^ 1 = 6; 6 ^ 2 = 4      -> 4 survives
  because x ^ x = 0 and x ^ 0 = x, order is irrelevant.

  count bits of 13 = 1101:
    n & 1 = 1 (count 1)   n >>= 1 -> 110
    n & 1 = 0 (count 1)   n >>= 1 -> 11
    n & 1 = 1 (count 2)   n >>= 1 -> 1
    n & 1 = 1 (count 3)   n >>= 1 -> 0    -> popcount(13) = 3
  faster: n &= n - 1 clears the lowest set bit; 13 -> 12 -> 8 -> 0 =
  3 iterations (one per SET bit, not per bit position).

PSEUDO CODE:
  SingleNumber(a): xor everything; the unpaired value survives
  SingleNumberII(a): count each bit position modulo 3 and rebuild
  Popcount(n): while n: n &= n - 1; count++
  CountBitsUpTo(n): dp[i] = dp[i >> 1] + (i & 1)
  IsPowerOfTwo(n): n > 0 && (n & (n - 1)) == 0
  SubsetsViaBits(a, n): for mask = 0..2^n - 1, emit the elements
      whose bit is set

SAMPLE INPUT & DRY RUN:
  5
  4 1 2 1 2
  7
  5 1 3 1 3 1 3
  1000
  3
  10 20 30
  Single number = 4 (diagram). Single Number II with 5 appearing once
  and 1, 3 appearing three times = 5. countBits(0..1000) stores all
  1001 values and cross-checks each against popcount; countBits(16) = 1.
  The last part enumerates the 8 subsets of {10,20,30} by mask.

SPECIAL CASES:
  - Single Number requires exactly one odd-frequency value; a single
    element works too (answer is that element).
  - Single Number II needs 32 bit positions; values are limited to
    0..10^6 so every input fits comfortably in 20 bits.
  - Counting bits: the DP table holds n + 1 entries. The validated
    maximum is VALUE_LIMIT (10^6), and the array is sized for exactly
    that - asking for a larger n is REJECTED instead of writing out of
    bounds (the earlier draft accepted 10^6 while sizing the array for
    1000, an overflow bug now impossible to trigger).
  - All shifts use unsigned types: `1u << 31` is well defined, whereas
    `1 << 31` on a 32-bit signed int overflows and is undefined
    behaviour. That is why every mask/bit variable here is unsigned.
  - IsPowerOfTwo: 0 is NOT a power of two; the check n > 0 handles it.
  - Subsets: the full per-mask listing is printed while n <= 8 (at most
    256 lines); for larger n (up to 12) only the COUNT is printed, so
    the console stays readable. Either way the enumeration runs and
    verifies the identity sum of popcounts = n * 2^(n-1), which is a
    free correctness check on the popcount routine itself.
  - Subset values may be any int: they are stored as unsigned and cast
    back on printing, so enumeration is a bit-cast, not a value hack.
  - n <= 1000 values, each 0..10^6 for the bit-counting parts.

COMPLEXITY:
  O(n) per linear scan; popcount is O(number of set bits);
  count-bits DP is O(n); subset enumeration is O(2^n * n).

VALIDATION:
  A self-check compares the DP table against a direct popcount for
  every value 0..n, and prints MISMATCH if any entry disagrees. The
  subset output re-derives each popcount with the same routine so a
  broken popcount cannot hide.

TIPS:
  `n & (n - 1)` and `n & -n` are the two workhorses: the first clears
  the lowest set bit, the second ISOLATES it. Memorise both. For
  counting bits in bulk, the DP dp[i] = dp[i >> 1] + (i & 1) is the
  pattern LeetCode 338 expects, not repeated popcounts.
*/

#include <stdio.h>

#define MAX_N 1000
#define VALUE_LIMIT 1000000
#define SUBSET_PRINT_LIMIT 12

int value[MAX_N];
int bitCount[VALUE_LIMIT + 1];
unsigned int subsetValues[SUBSET_PRINT_LIMIT];

/* Counts the set bits of an UNSIGNED value: no sign-extension issues. */
int popcount(unsigned int n) {
    int count = 0;
    while (n != 0) {
        n &= n - 1;                     /* clear the lowest set bit */
        count++;
    }
    return count;
}

int main(void) {
    int n;

    printf("Bit Tricks Interview Problems\n\n");
    printf("== Single Number. How many values (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (0..%d, every value paired except one):\n", n,
           VALUE_LIMIT);
    unsigned int single = 0u;
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &value[i]) != 1 || value[i] < 0 ||
            value[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        single ^= (unsigned int)value[i];
    }
    printf("Single number (odd frequency) = %u\n", single);
    printf("Popcount of that value = %d\n", popcount(single));

    printf("\n== Single Number II. How many values (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (0..%d; every value repeats 3 times except\n",
           n, VALUE_LIMIT);
    printf("one value that appears exactly once):\n");
    unsigned int bits[32];
    for (int i = 0; i < 32; i++) bits[i] = 0u;
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &value[i]) != 1 || value[i] < 0 ||
            value[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        unsigned int current = (unsigned int)value[i];
        for (int b = 0; b < 32; b++) {
            if ((current & (1u << b)) != 0u) bits[b]++;
        }
    }
    unsigned int unique = 0u;
    for (int b = 0; b < 32; b++) {
        if (bits[b] % 3u != 0u) unique |= 1u << b;
    }
    printf("Value appearing once = %u\n", unique);
    printf("Bit positions with a non-multiple-of-3 count:");
    for (int b = 0; b < 32; b++) {
        if (bits[b] % 3u != 0u) printf(" %d", b);
    }
    printf("\n");

    printf("\n== Counting bits. Show counts for 0..n, n (0-%d): ",
           VALUE_LIMIT);
    if (scanf("%d", &n) != 1 || n < 0 || n > VALUE_LIMIT) {
        printf("Invalid n: the DP table holds %d entries (0..%d)\n",
               VALUE_LIMIT + 1, VALUE_LIMIT);
        return 1;
    }
    bitCount[0] = 0;
    for (int i = 1; i <= n; i++) {
        bitCount[i] = bitCount[i >> 1] + (i & 1);
    }
    printf("countBits(0..%d):", n);
    for (int i = 0; i <= n && i <= 32; i++) printf(" %d", bitCount[i]);
    if (n > 32) printf(" ... %d", bitCount[n]);
    printf("\n");
    printf("Cross-check with popcount for every value 0..%d: ", n);
    int mismatches = 0;
    for (int i = 0; i <= n; i++) {
        if (bitCount[i] != popcount((unsigned int)i)) mismatches++;
    }
    printf("%s\n", mismatches == 0 ? "all match" : "MISMATCH FOUND");
    int powerCount = 0;
    printf("Values in 0..%d that are powers of two:", n);
    for (int i = 1; i <= n; i++) {
        if (((unsigned int)i & ((unsigned int)i - 1u)) == 0u) {
            powerCount++;
            if (powerCount <= 16) printf(" %d", i);
        }
    }
    printf(" (total %d)\n", powerCount);
    printf("Is 0 a power of two? no (n > 0 is required by the test)\n");

    printf("\n== Subsets via bitmask. How many elements (0-%d)? ",
           SUBSET_PRINT_LIMIT);
    if (scanf("%d", &n) != 1 || n < 0 || n > SUBSET_PRINT_LIMIT) {
        printf("Invalid element count\n");
        return 1;
    }
    printf("Enter %d integers (any 32-bit values):\n", n);
    for (int i = 0; i < n; i++) {
        int raw;
        if (scanf("%d", &raw) != 1) {
            printf("Invalid value\n");
            return 1;
        }
        subsetValues[i] = (unsigned int)raw;
    }
    unsigned int total = 1u << n;       /* 1u, so n = 31 would still be safe */
    printf("Total subsets = 2^%d = %u\n", n, total);
    int printed = n <= 8;               /* full list only for small inputs */
    unsigned long long popcountSum = 0; /* must equal n * 2^(n-1) */
    for (unsigned int mask = 0u; mask < total; mask++) {
        int ones = popcount(mask);
        popcountSum += (unsigned long long)ones;
        if (printed) {
            printf("  mask %2u -> {", mask);
            int first = 1;
            for (int b = 0; b < n; b++) {
                if ((mask & (1u << b)) != 0u) {
                    printf("%s%d", first ? "" : ", ",
                           (int)subsetValues[b]);
                    first = 0;
                }
            }
            printf("}   (popcount %d)\n", ones);
        }
    }
    if (!printed) {
        printf("  (per-mask listing suppressed for n = %d; the count %u\n",
               n, total);
        printf("   is the result LeetCode 78 returns)\n");
    }
    unsigned long long expected = (n == 0)
        ? 0ULL
        : (unsigned long long)n << (n - 1);   /* n * 2^(n-1) */
    printf("Sum of all popcounts = %llu, expected n * 2^(n-1) = %llu -> %s\n",
           popcountSum, expected,
           popcountSum == expected ? "PASS" : "MISMATCH");
    printf("Every mask 0..2^n-1 enumerates exactly the 2^n subsets.\n");
    return 0;
}
