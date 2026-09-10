/*
Divide & Conquer Hard - Reverse Pairs, Count Smaller After Self,
Counting Inversions (Advanced)
---------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 493 - Reverse Pairs (count i < j with a[i] > 2*a[j])
  LeetCode 315 - Count of Smaller Numbers After Self
  LeetCode 912 - Sort an Array (the merge sort the other two modify)
  HackerRank "Merge Sort: Counting Inversions"
  (the plain O(n^2) baseline lives in 10-divide-and-conquer/inversions-median.c)

PROBLEM DEFINITION:
One merge sort, three counting questions. While merging two already
sorted halves every element of the left half sits before every element
of the right half, so "how many pairs cross this boundary?" is one
monotone two-pointer scan - never a nested loop. Counting Inversions
counts a[i] > a[j], Reverse Pairs counts a[i] > 2*a[j], and Count of
Smaller Numbers After Self counts, for every index, how many later
values are strictly smaller. Each answer is a per-level scan added to
the merge, so all three stay O(n log n).

REAL-LIFE EXAMPLE:
Ranking anomalies: "how many label pairs are inverted against the
reference order?" (inversions), "how many pairs are severely out of
order - at least double?" (reverse pairs), and "how many later items
rank below this one?" (smaller after self) are the same boundary count
under three different comparisons.

DIAGRAM (inversions while merging [2,4] and [1,3,5]):
  left  = [2,4]   right = [1,3,5]
  i->2, j->1 : 2 > 1, so BOTH 2 and 4 form an inversion with 1
               inversions += (mid - i) = 2
  i->2, j->3 : 2 < 3 -> advance i
  i->4, j->3 : 4 > 3 -> inversions += (mid - i) = 1
  i->4, j->5 : 4 < 5 -> advance i; left exhausted
  total = 3 pairs (2,1) (4,1) (4,3); the merge order is not disturbed.
  Reverse Pairs uses its OWN scan first (a[i] > 2*a[j]) and only then
  merges - once the halves are merged the pair is gone forever.

PSEUDO CODE:
  SortAndCount(a, lo, hi):
      if hi - lo < 2 return 0
      mid = (lo + hi) / 2
      total  = SortAndCount(a, lo, mid) + SortAndCount(a, mid, hi)
      total += boundary scan          (two pointers, no nesting)
      merge the two halves into a buffer
      return total
  InversionsScan   : while both halves remain, if a[i] > a[j] then
                     total += mid - i; the merge that follows is stable
  ReversePairsScan : advance j while (2*a[j]) < a[i]; total += j - mid
  SmallerAfterSelf : each time a right element wins the merge, every
                     still-unmerged left element gains one smaller value
                     seen later -> add counters[ position[i] ] += 1

SAMPLE INPUT & DRY RUN:
  5
  2 4 1 3 5
  One run answers all three questions for [2,4,1,3,5]:
    inversions = 3                    (diagram above)
    reverse pairs = 1                 (only (2,1): 2 > 2*1, while
                                       4 > 2*3 is false)
    smaller after self = 1 2 0 0 0    (2 and 4 each have one smaller
                                       value later, 1 has two)
  Two more hand-checks you can type in instead:
    [1,3,2,3,1] -> reverse pairs 2, i.e. (index 1 -> index 4) and
                   (index 3 -> index 4), because 3 > 2*1 while
                   2 > 2*1 is false and equal values never count;
    [-5,-2,1,3] -> 0 inversions, 0 reverse pairs, 0 0 0 0 (ascending).

SPECIAL CASES:
  - Reverse Pairs compares a[i] > 2*a[j], NOT a[i] > a[j], and its scan
    must run BEFORE the merge; the doubled value needs a wider type
    (a[j] <= 10^9, so 2*a[j] overflows a 32-bit int).
  - Duplicates: inversions use strict '>' and smaller-after-self uses
    strict '<', so equal values are never counted. Lock the tie rule.
  - Sizes 0 and 1 have no pairs at all: the answer is 0. This program
    requires n >= 1 and, for n = 1, prints 0 inversions, 0 reverse pairs
    and a single 0 for smaller-after-self.
  - The recursion depth is log n, but the merge buffer is O(n); this
    program allocates it once in main and passes it down.
  - Values may be negative; no comparison needs absolute values.

COMPLEXITY:
  O(n log n) time and O(n) extra space for all three - the merge buffer.
  The O(n^2) baseline is computed only to cross-check the answer.

TIPS:
  "Count pairs with a property", where the property compares a[i] and
  a[j] and the halves are sorted, is one extra scan per merge level.
  The monotonicity (the pointer only moves forward) is the whole
  algorithm; write the scan as its own function so the merge stays
  readable and the cross-check stays honest.
*/

#include <stdio.h>

#define MAX_N 1000

int smaller[MAX_N];
int countSmallerFlag;

/* Merge two sorted halves and count, at this single boundary only:
 *   - inversions    : a[right] < a[left]  (each right element placed
 *                     before a left element is one inversion)
 *   - reverse pairs : a[left] > 2*a[right], counted by its OWN scan
 *                     BEFORE the halves are merged
 *   - smaller after : when a LEFT element is placed, every right element
 *                     already placed is strictly smaller and comes after
 *                     it, so smaller[pos[i]] += (j - mid) -- O(1) per
 *                     element instead of an inner loop.
 * pos[] travels with a[] so "after self" can be attributed to the
 * original index. */
void mergeHalves(long long a[], int pos[], long long tmp[], int tmpPos[],
                 int lo, int mid, int hi, long long* inversions,
                 long long* reversePairs) {
    int j = mid;
    for (int i = lo; i < mid; i++) {
        while (j < hi && a[j] * 2 < a[i]) j++;
        *reversePairs += j - mid;
    }

    int i = lo;
    j = mid;
    int out = lo;
    while (i < mid && j < hi) {
        if (a[i] <= a[j]) {
            if (countSmallerFlag) smaller[pos[i]] += j - mid;
            tmp[out] = a[i];
            tmpPos[out] = pos[i];
            out++;
            i++;
        } else {
            *inversions += mid - i;
            tmp[out] = a[j];
            tmpPos[out] = pos[j];
            out++;
            j++;
        }
    }
    while (i < mid) {
        if (countSmallerFlag) smaller[pos[i]] += j - mid;
        tmp[out] = a[i];
        tmpPos[out] = pos[i];
        out++;
        i++;
    }
    while (j < hi) {
        tmp[out] = a[j];
        tmpPos[out] = pos[j];
        out++;
        j++;
    }
    for (int k = lo; k < hi; k++) {
        a[k] = tmp[k];
        pos[k] = tmpPos[k];
    }
}

void sortAndCount(long long a[], int pos[], long long tmp[], int tmpPos[],
                  int lo, int hi, long long* inversions,
                  long long* reversePairs) {
    if (hi - lo < 2) return;
    int mid = lo + (hi - lo) / 2;
    sortAndCount(a, pos, tmp, tmpPos, lo, mid, inversions, reversePairs);
    sortAndCount(a, pos, tmp, tmpPos, mid, hi, inversions, reversePairs);
    mergeHalves(a, pos, tmp, tmpPos, lo, mid, hi, inversions, reversePairs);
}

/* n <= 1000 keeps the O(n^2) definitions cheap, so both algorithms run
 * and the program reports whether they agree - a real self-check. */
long long bruteInversions(const long long a[], int n) {
    long long total = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j]) total++;
    return total;
}

long long bruteReversePairs(const long long a[], int n) {
    long long total = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j] * 2) total++;
    return total;
}

void bruteSmallerAfterSelf(const long long a[], int n, int out[]) {
    for (int i = 0; i < n; i++) {
        out[i] = 0;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[i]) out[i]++;
        }
    }
}

int main(void) {
    int n;
    long long values[MAX_N];
    long long working[MAX_N];
    long long tmp[MAX_N];
    int pos[MAX_N];
    int tmpPos[MAX_N];

    printf("Divide & Conquer Hard - Inversions, Reverse Pairs, Smaller "
           "After Self\n\n");
    printf("How many numbers (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (|value| <= 10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 ||
            values[i] < -1000000000LL || values[i] > 1000000000LL) {
            printf("Invalid value\n");
            return 1;
        }
    }

    /* Pass 1: inversions + reverse pairs. */
    for (int i = 0; i < n; i++) {
        working[i] = values[i];
        pos[i] = i;
    }
    long long inversions = 0;
    long long reversePairs = 0;
    countSmallerFlag = 0;
    sortAndCount(working, pos, tmp, tmpPos, 0, n, &inversions, &reversePairs);

    /* Pass 2: smaller-after-self (same merge, one extra counter). */
    for (int i = 0; i < n; i++) {
        working[i] = values[i];
        pos[i] = i;
        smaller[i] = 0;
    }
    long long ignoredInversions = 0;
    long long ignoredReverse = 0;
    countSmallerFlag = 1;
    sortAndCount(working, pos, tmp, tmpPos, 0, n, &ignoredInversions,
                 &ignoredReverse);

    long long bruteInv = bruteInversions(values, n);
    long long brutePairs = bruteReversePairs(values, n);
    printf("\nInversions (LeetCode 912 / HackerRank): %lld\n", inversions);
    printf("Brute-force O(n^2) cross-check          : %lld -> %s\n", bruteInv,
           inversions == bruteInv ? "agree" : "MISMATCH");
    printf("Reverse pairs (LeetCode 493)            : %lld\n", reversePairs);
    printf("Brute-force O(n^2) cross-check          : %lld -> %s\n",
           brutePairs, reversePairs == brutePairs ? "agree" : "MISMATCH");

    printf("\nSmaller numbers after self (LeetCode 315):");
    for (int i = 0; i < n; i++) printf(" %d", smaller[i]);
    printf("\n");

    int bruteSmaller[MAX_N];
    bruteSmallerAfterSelf(values, n, bruteSmaller);
    int smallerOk = 1;
    for (int i = 0; i < n; i++) {
        if (smaller[i] != bruteSmaller[i]) smallerOk = 0;
    }
    printf("Brute-force O(n^2) cross-check          :");
    for (int i = 0; i < n; i++) printf(" %d", bruteSmaller[i]);
    printf(" -> %s\n", smallerOk ? "agree" : "MISMATCH");
    return 0;
}
