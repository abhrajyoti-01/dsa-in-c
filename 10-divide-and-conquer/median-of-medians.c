/*
Median of Medians - Worst-Case Linear-Time Selection (Advanced)
--------------------------------------------------------------
PROBLEM DEFINITION:
Find the k-th smallest element of an unsorted array in O(n) time in
the WORST case. Quickselect is O(n) on average but O(n^2) with bad
pivots. The Blum-Floyd-Pratt-Rivest-Tarjan (BFPRT) algorithm chooses
a pivot that is guaranteed to be near the middle: the median of the
medians of groups of five.

REAL-LIFE EXAMPLE:
A real-time trading or control system must report the median latency
of the last batch within a hard deadline. "Usually fast" is not good
enough when an adversary (or bad luck) can feed the worst input; a
guaranteed O(n) selection never blows the deadline.

DIAGRAM (why the pivot is good):
  groups of 5, each sorted, medians in the middle row (M = pivot):
      .  .  .  .  .
      .  .  .  .  .
      m  m  M  m  m      medians sorted left to right
      .  .  .  .  .
      .  .  .  .  .
  The top-left block (half the groups x 3 rows) is <= M, and the
  bottom-right block is >= M: at least ~3n/10 elements on each side,
  so every recursive call discards at least 30% of the array.

PSEUDO CODE:
  Select(A, k):                         (k is 0-based)
      if |A| <= 5: sort A, return A[k]
      split A into groups of 5; sort each group; take each median
      pivot = Select(medians, (|medians| - 1) / 2)
      three-way partition A into  < pivot | = pivot | > pivot
      if k < |less|: return Select(less, k)
      if k < |less| + |equal|: return pivot
      return Select(greater, k - |less| - |equal|)

SAMPLE INPUT & DRY RUN:
  15
  12 3 5 7 4 19 26 23 1 8 15 10 6 2 9
  3
  8
  1
  15
  Groups sorted: [3 4 5 7 12] [1 8 19 23 26] [2 6 9 10 15]
  Medians: 5 19 9 -> median of medians = 9
  Partition around 9: 8 smaller, 1 equal, 6 larger.
  k = 8: rank 8 < 9, so recurse into the 8 smaller values -> 8
  k = 1 -> 1, k = 15 -> 26 (sorted: 1 2 3 4 5 6 7 8 9 10 12 15 19 23 26)

SPECIAL CASES:
  - Duplicates: the three-way partition keeps every pivot copy in the
    middle, so arrays like 7 7 7 7 do not cause endless recursion.
  - n <= 5 is solved by insertion sort directly.
  - k = 1 and k = n give the minimum and maximum.
  - 1 <= n <= 100000, any int values, up to 100 queries, k in 1..n.

COMPLEXITY:
  T(n) <= T(n/5) + T(7n/10) + O(n) -> O(n) worst case, since
  1/5 + 7/10 < 1. Extra space O(n) for the medians across the recursion.
  In practice randomized quickselect is faster (smaller constant).

TIPS:
  Groups of 3 do NOT work (1/3 + 2/3 = 1 gives O(n log n)); 5 is the
  smallest odd size that does. Introselect (C++ nth_element) starts as
  quickselect and falls back to median of medians when it goes badly.
  Compare structure-sort-quickselect and the master-theorem lesson.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100000
#define MAX_QUERIES 100
#define TRACE_LIMIT 30

int traceTopLevel;

void insertionSort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int selectKth(int *a, int n, int k, int depth) {
    if (n <= 5) {
        insertionSort(a, n);
        return a[k];
    }
    int groups = (n + 4) / 5;
    int *medians = malloc(groups * sizeof(int));
    if (medians == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    for (int g = 0; g < groups; g++) {
        int start = 5 * g;
        int length = n - start < 5 ? n - start : 5;
        insertionSort(a + start, length);
        medians[g] = a[start + (length - 1) / 2];
    }
    if (traceTopLevel && depth == 0) {
        printf("  groups sorted:");
        for (int g = 0; g < groups; g++) {
            int start = 5 * g;
            int length = n - start < 5 ? n - start : 5;
            printf(" [");
            for (int i = 0; i < length; i++) {
                printf(i ? " %d" : "%d", a[start + i]);
            }
            printf("]");
        }
        printf("\n  medians:");
        for (int g = 0; g < groups; g++) printf(" %d", medians[g]);
    }
    int pivot = selectKth(medians, groups, (groups - 1) / 2, depth + 1);
    free(medians);

    int lt = 0;
    int i = 0;
    int gt = n - 1;
    while (i <= gt) {
        if (a[i] < pivot) swap(&a[lt++], &a[i++]);
        else if (a[i] > pivot) swap(&a[i], &a[gt--]);
        else i++;
    }
    if (traceTopLevel && depth == 0) {
        printf(" -> pivot %d\n  partition: %d smaller, %d equal, %d larger\n",
               pivot, lt, gt - lt + 1, n - 1 - gt);
    }
    if (k < lt) return selectKth(a, lt, k, depth + 1);
    if (k <= gt) return pivot;
    return selectKth(a + gt + 1, n - gt - 1, k - gt - 1, depth + 1);
}

int compareInt(const void *x, const void *y) {
    int a = *(const int *)x;
    int b = *(const int *)y;
    return (a > b) - (a < b);
}

int main(void) {
    static int original[MAX_N];
    static int work[MAX_N];
    static int sorted[MAX_N];
    int n;
    int q;

    printf("Median of Medians - Linear-Time Selection\n\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &original[i]) != 1) {
            printf("Invalid value\n");
            return 1;
        }
    }
    memcpy(sorted, original, n * sizeof(int));
    qsort(sorted, n, sizeof(int), compareInt);

    printf("How many queries (1-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        int k;
        printf("Query %d: k (1..%d): ", i + 1, n);
        if (scanf("%d", &k) != 1 || k < 1 || k > n) {
            printf("Invalid k\n");
            return 1;
        }
        memcpy(work, original, n * sizeof(int));
        traceTopLevel = i == 0 && n <= TRACE_LIMIT;
        if (traceTopLevel) printf("\n");
        int answer = selectKth(work, n, k - 1, 0);
        printf("k = %d: %d  (sorting check: %s)\n", k, answer,
               answer == sorted[k - 1] ? "same" : "MISMATCH");
    }
    return 0;
}
