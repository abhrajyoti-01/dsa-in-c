/*
Median of Two Sorted Arrays - Partition Binary Search (Advanced)
----------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 4   - Median of Two Sorted Arrays (classic O(log(min)) solve)
  LeetCode 21  - Merge Two Sorted Lists (the O(n+m) baseline)
  HackerRank "Find the Median" on sorted data

PROBLEM DEFINITION:
Return the median of two sorted arrays in O(log(min(n, m))) time
without merging: binary search the number of elements the left half
takes from the shorter array, then read the boundary values.

REAL-LIFE EXAMPLE:
Two sorted sensor feeds arrive at different rates; the combined
median latency must be reported without buffering and sorting all of
both streams. The partition method answers instantly.

DIAGRAM (A = [1, 3], B = [2]):
  want 2 elements of the 3 in the left half.
  i = 2 from A, j = 0 from B:
      A[0..1] = 1 3 | B[0..0] = (empty)
      left  = max(3) = 3, right = min(B[0]=2) = 2  -> 3 > 2 INVALID
  i = 1, j = 1:
      left  = max(A[0]=1, B[0]=2) = 2
      right = min(A[1]=3) = 3      -> 2 <= 3 VALID
  odd total -> median = left = 2.

PSEUDO CODE:
  Ensure A is the shorter array (swap roles if not)
  lo = 0, hi = n            // i = elements taken from A
  while lo <= hi:
      i = (lo + hi) / 2; j = half - i
      leftA  = i == 0    ? -inf : A[i-1]
      rightA = i == n    ? +inf : A[i]
      leftB  = j == 0    ? -inf : B[j-1]
      rightB = j == m    ? +inf : B[j]
      if leftA <= rightB and leftB <= rightA: found
      else if leftA > rightB: hi = i - 1
      else:                   lo = i + 1
  even total -> (max(lefts) + min(rights)) / 2
  odd  total -> max(lefts)

SAMPLE INPUT & DRY RUN:
  2
  1 3
  1
  2
  Median = 2 (detailed trace in the diagram above).
  Arrays [1,2] and [3,4] give (2 + 3)/2 = 2.5; the program prints the
  exact double and the two boundary values so the arithmetic is clear.

SPECIAL CASES:
  - Empty array: the median is the median of the other one. Both empty
    is rejected.
  - All of A before B (or vice versa): infinities bound the partition.
  - Negative and duplicated values are fine; comparisons only.
  - Even totals use the average of the two middle values.
  - Use -inf/+inf sentinels instead of indexing out of bounds - the
    classic bug is testing i == 0 AFTER reading A[i-1].
  - n, m <= 1000, values within +/-10^9.

COMPLEXITY:
  O(log(min(n, m))) time, O(1) extra space; the O(n + m) merge is only
  shown by the printed merged prefix for comparison.

TIPS:
  The invariant is "left half has exactly (n+m+1)/2 elements and every
  left value <= every right value". Binary search on i (the cut in the
  shorter array) keeps the range tiny and the code branchless enough
  to reason about. Drawing the cut on paper is the fastest sanity check.
*/

#include <stdio.h>

#define MAX_N 1000
#define VALUE_LIMIT 1000000000LL

long long first[MAX_N];
long long second[MAX_N];
long long merged[2 * MAX_N];

double medianOfTwo(const long long a[], int n, const long long b[], int m,
                   long long* leftValue, long long* rightValue) {
    if (n > m) {
        return medianOfTwo(b, m, a, n, leftValue, rightValue);
    }
    int low = 0;
    int high = n;
    int half = (n + m + 1) / 2;
    const long long infinity = 4000000000000000000LL;
    while (low <= high) {
        int i = low + (high - low) / 2;
        int j = half - i;
        long long leftA = i == 0 ? -infinity : a[i - 1];
        long long rightA = i == n ? infinity : a[i];
        long long leftB = j == 0 ? -infinity : b[j - 1];
        long long rightB = j == m ? infinity : b[j];
        if (leftA <= rightB && leftB <= rightA) {
            long long leftMax = leftA > leftB ? leftA : leftB;
            long long rightMin = rightA < rightB ? rightA : rightB;
            *leftValue = leftMax;
            *rightValue = rightMin;
            if ((n + m) % 2 == 1) {
                return (double)leftMax;
            }
            return ((double)leftMax + (double)rightMin) / 2.0;
        }
        if (leftA > rightB) {
            high = i - 1;
        } else {
            low = i + 1;
        }
    }
    *leftValue = 0;
    *rightValue = 0;
    return 0.0;
}

int main(void) {
    int n;
    int m;
    long long leftValue = 0;
    long long rightValue = 0;

    printf("Median of Two Sorted Arrays - Partition Search\n\n");
    printf("How many elements in the first SORTED array (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers in non-decreasing order:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &first[i]) != 1 ||
            first[i] < -VALUE_LIMIT || first[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        if (i > 0 && first[i] < first[i - 1]) {
            printf("Invalid input: array must be sorted\n");
            return 1;
        }
    }
    printf("How many elements in the second SORTED array (0-%d)? ", MAX_N);
    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < m; i++) {
        printf("Element %d: ", i + 1);
        if (scanf("%lld", &second[i]) != 1 ||
            second[i] < -VALUE_LIMIT || second[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        if (i > 0 && second[i] < second[i - 1]) {
            printf("Invalid input: array must be sorted\n");
            return 1;
        }
    }

    double median = medianOfTwo(first, n, second, m, &leftValue, &rightValue);
    printf("\nBoundary values: left = %lld, right = %lld\n", leftValue,
           rightValue);
    if ((n + m) % 2 == 1) {
        printf("Total elements = %d (odd) -> median = %lld\n", n + m,
               leftValue);
    } else {
        printf("Total elements = %d (even) -> median = (%lld + %lld)/2 = ",
               n + m, leftValue, rightValue);
        printf("%.1f\n", median);
    }
    printf("Merged median (double) = %.10g\n", median);

    int i = 0;
    int j = 0;
    int k = 0;
    while (i < n && j < m) merged[k++] = first[i] <= second[j] ? first[i++]
                                                              : second[j++];
    while (i < n) merged[k++] = first[i++];
    while (j < m) merged[k++] = second[j++];
    printf("Merged (O(n+m) reference method):");
    for (int t = 0; t < k; t++) printf(" %lld", merged[t]);
    printf("\n");
    return 0;
}
