/*
Array Hard Problems - First Missing Positive, Majority Element II,
Product of Array Except Self, Disappeared Numbers (Advanced)
----------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 41  - First Missing Positive (O(n) time, O(1) extra space)
  LeetCode 229 - Majority Element II (> n/3 candidates, Boyer-Moore)
  LeetCode 238 - Product of Array Except Self (division forbidden)
  LeetCode 448 - Find All Numbers Disappeared in an Array
  related: HackerRank "Minimum Swaps 2" (the same place-each-value-at-
           its-own-index idea; fully solved in hackerrank-tricky-classics.c)

PROBLEM DEFINITION:
Four array questions that each hide a trap. The obvious approach
(sort, hash, or divide) costs O(n log n) time or O(n) extra space; the
intended answers instead reuse the array as a lookup table - place each
value at its own index, mark a seen index with a sign flip, or build the
output in two directional passes. All four run in O(n) time.

REAL-LIFE EXAMPLE:
Validating a sequential ticket, invoice or packet run: "which receipt
number is the first one missing?" is First Missing Positive, and "which
numbers never appeared at all?" is Disappeared Numbers. Both must run
over millions of records without allocating a second table.

DIAGRAM (First Missing Positive on [2,3,-1,1,2,2,5,4], n = 8):
  index:           0  1  2  3  4  5  6  7
  start:           2  3 -1  1  2  2  5  4
  a value v in 1..n belongs at index v-1, so swap it home:
      v=2 -> index 1 ; v=3 -> index 2 ; v=1 -> index 0 ; v=5 -> index 4
  after placement: 1  2  3  4  5  2 -1  2
  indices 0..4 hold exactly 1..5; index 5 holds a duplicate 2, so the
  first index with a[i] != i+1 is i = 5 and the answer is 6.

PSEUDO CODE:
  FirstMissingPositive(a, n):
      for i in 0..n-1:
          while a[i] in [1..n] and a[a[i]-1] != a[i]: swap them
      for i in 0..n-1: if a[i] != i+1 return i+1
      return n+1                     (1..n were all present)
  MajorityII(a, n):                  (Boyer-Moore for > n/3)
      two candidates + two counters; a value matching neither cancels
      both; SURVIVORS MUST STILL BE COUNTED (the trick cannot prove
      anything on its own)
  ProductExceptSelf(a, n):
      pass 1 (left to right): out[i] = product of a[0..i-1]
      pass 2 (right to left): out[i] *= running product of a[i+1..n-1]
  Disappeared(a, n):
      mark every value v seen with a boolean flag (see SPECIAL CASES:
      the famous sign-flip rewrite is only valid when every value is
      guaranteed to be in 1..n)
      every index never marked is a missing number -> report i+1

SAMPLE INPUT & DRY RUN:
  8                       <- part 1..3: how many numbers
  2 3 -1 1 2 2 5 4        <- the array (note the -1: a real value here)
  8                       <- part 4: size of the SECOND array
  1 2 3 4 5 1 2 3         <- its values, all inside 1..8 (LeetCode 448 rule)
  First missing positive: 1..5 are all present -> 6.
  Majority needs more than 8/3 = 2 copies: only 2 (three times) -> [2].
  Product except self: -240 -160 480 -480 -240 -240 -96 -120.
  Disappeared numbers in [1..8]: 6 7 8.
  (Parts 1-3 and part 4 read SEPARATE arrays: the LeetCode 448 guarantee
  "every value is in 1..m" cannot be imposed on the part-1 array, which
  deliberately contains -1, duplicates and out-of-range values.)

SPECIAL CASES:
  - Values <= 0 or > n cannot help First Missing Positive, so they are
    treated as holes and simply skipped.
  - If 1..n are all present the answer is n+1, which is larger than
    every value in the array - a valid answer, not an error.
  - Majority Element II can finish with ZERO or TWO answers, so the
    candidates must be counted; a solution that prints one candidate
    blindly is wrong for arrays like [1,2,3].
  - Duplicates stop the placement loop: swapping when the target slot
    already holds the same value would spin forever.
  - -1 is a real value here, so the Disappeared pass must take the
    absolute value BEFORE using a number as an index.
  - The product can overflow, so this program bounds |value| <= 20 and
    n <= 12 (20^12 ~ 4.1e15 still fits in a 64-bit integer).

COMPLEXITY:
  O(n) time for every part. Extra space is O(1) outside the required
  output arrays: the placements and sign flips happen inside the given
  array itself.

TIPS:
  The idea to memorise is "the array already has n slots, so it can
  store facts about the range 1..n". Ask whether the input values live
  in a small known range; if they do, indices are free memory.
  Never trust a Boyer-Moore candidate until you have counted it.
*/

#include <stdio.h>

#define MAX_N 12
#define VALUE_LIMIT 20

long long values[MAX_N];
long long products[MAX_N];
int present[MAX_N + 1];

/* LeetCode 41: place every value v in 1..n at index v-1. The while loop
 * may swap several times per index but each swap puts one value in its
 * final slot, so the total work is linear. */
int firstMissingPositive(long long a[], int n) {
    for (int i = 0; i < n; i++) {
        while (a[i] >= 1 && a[i] <= n) {
            int target = (int)a[i] - 1;
            if (a[target] == a[i]) break;   /* duplicate: already home */
            long long held = a[target];
            a[target] = a[i];
            a[i] = held;
        }
    }
    for (int i = 0; i < n; i++) {
        if (a[i] != i + 1) return i + 1;
    }
    return n + 1;
}

/* LeetCode 229: Boyer-Moore for "appears more than n/3 times". Two
 * candidates survive the pairing; both are then COUNTED, because the
 * cancellation phase alone proves nothing. */
void majorityElementII(const long long a[], int n, int out[], int* count) {
    long long cand1 = 0;
    long long cand2 = 0;
    int tally1 = 0;
    int tally2 = 0;
    for (int i = 0; i < n; i++) {
        if (tally1 > 0 && a[i] == cand1) {
            tally1++;
        } else if (tally2 > 0 && a[i] == cand2) {
            tally2++;
        } else if (tally1 == 0) {
            cand1 = a[i];
            tally1 = 1;
        } else if (tally2 == 0) {
            cand2 = a[i];
            tally2 = 1;
        } else {
            tally1--;
            tally2--;
        }
    }
    *count = 0;
    int seen1 = 0;
    int seen2 = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] == cand1) seen1++;
        if (a[i] == cand2) seen2++;
    }
    if (tally1 > 0 && seen1 > n / 3) out[(*count)++] = (int)cand1;
    if (tally2 > 0 && cand2 != cand1 && seen2 > n / 3) {
        out[(*count)++] = (int)cand2;
    }
}

/* LeetCode 238: prefix pass into the output array, then a suffix pass
 * multiplied on top. No division anywhere, so zeros are harmless. */
void productExceptSelf(const long long a[], int n, long long out[]) {
    long long running = 1;
    for (int i = 0; i < n; i++) {
        out[i] = running;
        running *= a[i];
    }
    running = 1;
    for (int i = n - 1; i >= 0; i--) {
        out[i] *= running;
        running *= a[i];
    }
}

int main(void) {
    int n;

    printf("Array Hard Problems - First Missing Positive, Majority II, "
           "Product, Disappeared\n\n");
    printf("How many numbers (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (-%d..%d; values outside 1..%d are allowed "
           "and treated as holes):\n", n, VALUE_LIMIT, VALUE_LIMIT, n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 || values[i] < -VALUE_LIMIT ||
            values[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }

    /* Part 1: first missing positive (works on a copy - it rearranges). */
    long long scratch[MAX_N];
    for (int i = 0; i < n; i++) scratch[i] = values[i];
    int firstMissing = firstMissingPositive(scratch, n);
    printf("\nFirst missing positive: %d\n", firstMissing);
    if (firstMissing == n + 1) {
        printf("(1..%d are all present, so the answer is n + 1)\n", n);
    }

    /* Part 2: majority element II. */
    int candidates[MAX_N];
    int candidateCount = 0;
    majorityElementII(values, n, candidates, &candidateCount);
    printf("Majority elements (more than %d/3 = %d copies):", n, n / 3);
    if (candidateCount == 0) printf(" none");
    for (int i = 0; i < candidateCount; i++) printf(" %d", candidates[i]);
    printf("\n");

    /* Part 3: product except self. */
    productExceptSelf(values, n, products);
    printf("Product of array except self:");
    for (int i = 0; i < n; i++) printf(" %lld", products[i]);
    printf("\n");

    /* Part 4: disappeared numbers in a second array of size m whose
     * values are all in 1..m (the exact LeetCode 448 guarantee). */
    int m;
    printf("\nDisappeared numbers (LeetCode 448)\n");
    printf("Size of the second array (1-%d)? ", MAX_N);
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers in 1..%d (duplicates allowed):\n", m, m);
    for (int i = 0; i <= m; i++) present[i] = 0;
    for (int i = 0; i < m; i++) {
        long long v;
        if (scanf("%lld", &v) != 1 || v < 1 || v > m) {
            printf("Invalid value (must be in 1..%d)\n", m);
            return 1;
        }
        present[(int)v] = 1;
    }
    printf("Disappeared numbers in 1..%d:", m);
    int missing = 0;
    for (int v = 1; v <= m; v++) {
        if (!present[v]) {
            printf(" %d", v);
            missing++;
        }
    }
    if (missing == 0) printf(" none (every value appears)");
    printf("\n");
    return 0;
}
