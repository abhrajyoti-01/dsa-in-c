/*
Sparse Table - Constant-Time Static Range Minimum (Advanced)
------------------------------------------------------------
PROBLEM DEFINITION:
Preprocess an unchanging array to answer minimum queries on inclusive
ranges [l..r] in O(1), without traversing a tree for each query.

REAL-LIFE EXAMPLE:
Analyze archived daily temperatures: many queries ask for the coldest
day in different intervals, but the historical measurements never change.

DIAGRAM (array [4,2,7,1,3]):
  length 1:  4  2  7  1  3
  length 2:  2  2  1  1
  length 4:  1  1
  query [0..4]: min(block [0..3], block [1..4]) = min(1,1) = 1
  Overlap is safe: min(x,x) = x (idempotence).

PSEUDO CODE:
  table[0][i] = a[i]
  table[k][i] = min(table[k-1][i], table[k-1][i + 2^(k-1)])
  Precompute floor(log2(length)) for lengths 1..n
  Query(l,r):
      k = floor(log2(r-l+1))
      Return min(table[k][l], table[k][r - 2^k + 1])

SAMPLE INPUT & DRY RUN:
  5
  4 2 7 1 3
  3
  0 4
  0 2
  2 2
  [0..4] uses two length-4 blocks -> 1.
  [0..2] uses [0..1] and [1..2] -> min(2,2) = 2.
  [2..2] uses the single value 7.

SPECIAL CASES:
  - Negative numbers, duplicates, and single-element queries work.
  - Zero queries is allowed; empty arrays and invalid ranges are rejected.
  - No updates: changing a value requires rebuilding the table.
  - The overlapping-block trick is NOT valid for sum (double counting).
  - n <= 1000, queries <= 1000; indices are zero-based.

COMPLEXITY:
  Build O(n log n) time/space; each minimum query O(1).

TIPS:
  Sparse tables suit static min/max/GCD. Compare segment-tree and
  fenwick-tree when the data changes between queries.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_LOG 10
#define MAX_QUERIES 1000

int table[MAX_LOG][MAX_N];
int logValue[MAX_N + 1];

int minimum(int a, int b) {
    return a < b ? a : b;
}

void buildTable(int n) {
    for (int i = 2; i <= n; i++) {
        logValue[i] = logValue[i / 2] + 1;
    }
    for (int k = 1; (1 << k) <= n; k++) {
        int half = 1 << (k - 1);
        for (int i = 0; i + (1 << k) <= n; i++) {
            table[k][i] = minimum(table[k - 1][i], table[k - 1][i + half]);
        }
    }
}

int queryMin(int l, int r) {
    int k = logValue[r - l + 1];
    return minimum(table[k][l], table[k][r - (1 << k) + 1]);
}

int main(void) {
    int n;
    int q;
    printf("Sparse Table - Static Range Minimum\n\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &table[0][i]) != 1) {
            printf("Invalid value\n");
            return 1;
        }
    }
    buildTable(n);
    printf("How many queries (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        int l;
        int r;
        printf("Query %d: l r (zero-based, inclusive): ", i + 1);
        if (scanf("%d %d", &l, &r) != 2 || l < 0 || r >= n || l > r) {
            printf("Invalid range\n");
            return 1;
        }
        printf("min(%d, %d) = %d\n", l, r, queryMin(l, r));
    }
    return 0;
}
