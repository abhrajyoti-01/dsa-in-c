/*
Digit DP - Counting Numbers by Their Digits up to 10^18 (Advanced)
-----------------------------------------------------------------
PROBLEM DEFINITION:
Count the integers x in [L, R] whose digit sum is divisible by K, for
L and R as large as 10^18. Looping over the range is impossible, but
the answer depends only on the digits, so we build numbers digit by
digit from the left and remember (position, digit sum mod K) states.
The "tight" flag tracks whether the prefix so far equals the prefix of
the upper bound, which limits the next digit.

REAL-LIFE EXAMPLE:
Lottery and ticket systems call a ticket "lucky" by a digit rule; a
bank validating account numbers asks how many numbers in a block pass
a checksum. Both need counts over astronomically large ranges.

DIAGRAM (counting x in [0, 100] with K = 5; bound digits 1 0 0):
  position 0:  digit 0 (< 1, free)  -> any 2 digits 00..99: 20 numbers
               digit 1 (= 1, tight) -> position 1 must be <= 0
  position 1:  digit 0 (tight)      -> position 2 must be <= 0
  position 2:  digit 0 (tight)      -> the number 100, sum 1: not counted
  count(100) = 20

PSEUDO CODE:
  Count(N): numbers in [0, N] whose digit sum % K == 0
      digits = decimal digits of N, most significant first
      return Ways(0, 0, tight = true)
  Ways(pos, rem, tight):
      if pos == number of digits: return 1 if rem == 0 else 0
      if not tight and memo[pos][rem] known: return it
      limit = tight ? digits[pos] : 9
      total = sum over d = 0..limit of
              Ways(pos + 1, (rem + d) % K, tight and d == limit)
      if not tight: memo[pos][rem] = total
      return total
  Answer(L, R) = Count(R) - Count(L - 1)       (Count(-1) = 0)

SAMPLE INPUT & DRY RUN:
  3
  1 100 5
  0 0 7
  1 1000000000000000000 9
  [1, 100], K = 5: Count(100) = 20 (diagram), Count(0) = 1 (zero
      itself, digit sum 0) -> 19: 5 14 19 23 28 32 37 41 46 50 55 64 69
      73 78 82 87 91 96
  [0, 0], K = 7: zero has digit sum 0, divisible by 7 -> 1
  [1, 10^18], K = 9: digit sum divisible by 9 <=> number divisible by
      9, so the answer must be floor(10^18 / 9) = 111111111111111111
  Ranges of size <= 10^6 are also re-checked by brute force.

SPECIAL CASES:
  - Leading zeros do not change a digit sum, so "007" and "7" are the
    same number and need no special "started" flag here (rules such as
    "no two equal adjacent digits" DO need one).
  - L = 0 uses Count(-1) = 0.
  - K = 1 counts every number: the answer is R - L + 1.
  - 0 <= L <= R <= 10^18, 1 <= K <= 100, at most 100 queries.

COMPLEXITY:
  O(D * K * 10) per Count with D <= 19 digits: about 19,000 steps for
  K = 100. Memory O(D * K).

TIPS:
  Template for any "count numbers in [L, R] with property P": state =
  (position, tight, whatever P needs to remember). Memoize only the
  non-tight states; the tight path is a single chain. Typical states:
  digit sum, remainder of the number itself, last digit, count of a
  digit, "has started" for leading-zero-sensitive rules.
*/

#include <stdio.h>
#include <string.h>

#define MAX_K 100
#define MAX_DIGITS 19
#define MAX_QUERIES 100
#define LIMIT 1000000000000000000LL
#define BRUTE_LIMIT 1000000LL

int digits[MAX_DIGITS + 1];
int length;
int modulus;
long long memo[MAX_DIGITS + 1][MAX_K];

long long ways(int pos, int remainder, int tight) {
    if (pos == length) return remainder == 0;
    if (!tight && memo[pos][remainder] >= 0) return memo[pos][remainder];
    int limit = tight ? digits[pos] : 9;
    long long total = 0;
    for (int d = 0; d <= limit; d++) {
        total += ways(pos + 1, (remainder + d) % modulus, tight && d == limit);
    }
    if (!tight) memo[pos][remainder] = total;
    return total;
}

long long countUpTo(long long n) {
    if (n < 0) return 0;
    int reversed[MAX_DIGITS + 1];
    length = 0;
    do {
        reversed[length++] = (int)(n % 10);
        n /= 10;
    } while (n > 0);
    for (int i = 0; i < length; i++) digits[i] = reversed[length - 1 - i];
    memset(memo, -1, sizeof(memo));
    return ways(0, 0, 1);
}

long long bruteForce(long long low, long long high) {
    long long count = 0;
    for (long long x = low; x <= high; x++) {
        long long value = x;
        int sum = 0;
        while (value > 0) {
            sum += (int)(value % 10);
            value /= 10;
        }
        if (sum % modulus == 0) count++;
    }
    return count;
}

int main(void) {
    int q;

    printf("Digit DP - Numbers in [L, R] with Digit Sum Divisible by K\n\n");
    printf("How many queries (1-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_QUERIES) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        long long low;
        long long high;
        printf("Query %d: L R K: ", i + 1);
        if (scanf("%lld %lld %d", &low, &high, &modulus) != 3 || low < 0 ||
            high > LIMIT || low > high || modulus < 1 || modulus > MAX_K) {
            printf("Invalid query (need 0 <= L <= R <= 10^18, 1 <= K <= %d)\n",
                   MAX_K);
            return 1;
        }
        long long upper = countUpTo(high);
        long long lower = countUpTo(low - 1);
        long long answer = upper - lower;
        printf("[%lld, %lld], K = %d: Count(R) = %lld, Count(L-1) = %lld "
               "-> %lld", low, high, modulus, upper, lower, answer);
        if (high - low < BRUTE_LIMIT) {
            printf(bruteForce(low, high) == answer ? "  (brute force: same)"
                                                   : "  (brute force: MISMATCH)");
        }
        printf("\n");
    }
    return 0;
}
