/*
Mo's Algorithm - Offline Range Queries in Square-Root Order (Advanced)
---------------------------------------------------------------------
PROBLEM DEFINITION:
Answer many queries "how many DISTINCT values are in a[l..r]?" on a
fixed array. Distinct counts cannot be combined like sums, so prefix
sums and ordinary segment trees do not apply. Mo's algorithm reads all
queries first (offline), sorts them so that a sliding window
[curL..curR] moves only O((n + q) * sqrt(n)) steps in total, and keeps
a frequency table while the window grows and shrinks one element at a
time.

REAL-LIFE EXAMPLE:
A shop analyses a year of sales: "how many different products sold
between day l and day r?" for thousands of (l, r) pairs prepared by
the analytics team overnight. Nobody needs the answers in real time,
so the queries may be reordered to reuse work.

DIAGRAM (n = 8, block size B = 3):
  index:  0 1 2 | 3 4 5 | 6 7
  value:  1 2 1 | 3 2 2 | 4 1
          block 0 block 1 block 2
  Sort queries by (block of l, then r). Inside one block the right
  pointer only moves forward, and the left pointer moves at most B
  steps per query - that is the whole square-root trick.

PSEUDO CODE:
  B = ceil(sqrt(n)); compress values to 0..d-1
  Sort queries by (l / B, r)
  curL = 0, curR = -1, distinct = 0, freq[] = 0
  For each query (l, r) in sorted order:
      while curR < r: Add(a[++curR])
      while curL > l: Add(a[--curL])
      while curR > r: Remove(a[curR--])
      while curL < l: Remove(a[curL++])
      answer[query.id] = distinct
  Add(v):    if freq[v]++ == 0: distinct++
  Remove(v): if --freq[v] == 0: distinct--
  Print the answers in the ORIGINAL query order.

SAMPLE INPUT & DRY RUN:
  8
  1 2 1 3 2 2 4 1
  4
  0 3
  2 6
  5 7
  1 1
  B = 3; processing order: [1,1] [0,3] [2,6]  (block 0, r = 1, 3, 6)
  then [5,7] (block 1).
  [1,1]: window {2}                         -> 1 distinct
  [0,3]: grow to 1 2 1 3                    -> 3
  [2,6]: grow right to 6, drop 0 and 1: 1 3 2 2 4 -> 4
  [5,7]: grow right to 7, drop 2..4: 2 4 1  -> 3
  Answers in input order: 3 4 3 1
  Pointer moves 3 + 3 + 5 + 4 = 15, versus 13 for re-scanning: on a
  tiny input Mo's is no faster. On 2000 random queries over 2000
  elements it makes about 115 thousand moves instead of 1.35 million.

SPECIAL CASES:
  - Single-element ranges and the whole array work.
  - Values are coordinate-compressed, so any int (even negative or
    10^9) is allowed without a huge frequency array.
  - Queries may repeat or nest; answers always come back in input order.
  - l > r or out-of-range indices are rejected.
  - n, q <= 100000; indices zero-based and inclusive.

COMPLEXITY:
  O((n + q) * sqrt(n)) pointer moves + O(q log q) sort + O(n log n)
  compression; O(n + q) space. Naive re-scanning costs O(n * q).

TIPS:
  Mo's works whenever Add/Remove of one element is O(1): distinct
  counts, frequency-of-frequency, pair counts (sum of C(freq, 2)).
  Sorting r descending in odd blocks ("zig-zag") roughly halves the
  moves. Needs updates? Look at Mo's with time dimension, or switch to
  a segment-tree / fenwick-tree formulation.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000
#define MAX_Q 100000

typedef struct {
    int l;
    int r;
    int id;
} Query;

int value[MAX_N];
int sortedValues[MAX_N];
int frequency[MAX_N];
Query queries[MAX_Q];
int answer[MAX_Q];
int blockSize;
int distinct;

int compareInt(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int compareQuery(const void *a, const void *b) {
    const Query *x = a;
    const Query *y = b;
    int blockX = x->l / blockSize;
    int blockY = y->l / blockSize;
    if (blockX != blockY) return blockX - blockY;
    if (x->r != y->r) return x->r - y->r;
    return x->id - y->id;
}

/* Index of v in the sorted, de-duplicated array of size count. */
int compressedIndex(int v, int count) {
    int low = 0;
    int high = count - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (sortedValues[mid] < v) low = mid + 1;
        else high = mid;
    }
    return low;
}

void add(int index) {
    if (frequency[value[index]]++ == 0) distinct++;
}

void removeIndex(int index) {
    if (--frequency[value[index]] == 0) distinct--;
}

int main(void) {
    int n;
    int q;

    printf("Mo's Algorithm - Distinct Values in Ranges\n\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &value[i]) != 1) {
            printf("Invalid value\n");
            return 1;
        }
        sortedValues[i] = value[i];
    }
    printf("How many queries (1-%d)? ", MAX_Q);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_Q) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        printf("Query %d: l r (zero-based, inclusive): ", i + 1);
        if (scanf("%d %d", &queries[i].l, &queries[i].r) != 2 ||
            queries[i].l < 0 || queries[i].r >= n ||
            queries[i].l > queries[i].r) {
            printf("Invalid range\n");
            return 1;
        }
        queries[i].id = i;
    }

    qsort(sortedValues, n, sizeof(int), compareInt);
    int unique = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || sortedValues[i] != sortedValues[i - 1]) {
            sortedValues[unique++] = sortedValues[i];
        }
    }
    for (int i = 0; i < n; i++) value[i] = compressedIndex(value[i], unique);

    blockSize = 1;
    while (blockSize * blockSize < n) blockSize++;
    qsort(queries, q, sizeof(Query), compareQuery);

    int curL = 0;
    int curR = -1;
    long long moves = 0;
    long long naiveWork = 0;
    printf("\nBlock size %d; processing order:\n", blockSize);
    for (int i = 0; i < q; i++) {
        int l = queries[i].l;
        int r = queries[i].r;
        long long before = moves;
        while (curR < r) { add(++curR); moves++; }
        while (curL > l) { add(--curL); moves++; }
        while (curR > r) { removeIndex(curR--); moves++; }
        while (curL < l) { removeIndex(curL++); moves++; }
        answer[queries[i].id] = distinct;
        naiveWork += r - l + 1;
        if (q <= 20) {
            printf("  [%d,%d] block %d: %d distinct (%lld moves)\n", l, r,
                   l / blockSize, distinct, moves - before);
        }
    }

    printf("\nAnswers in input order:");
    for (int i = 0; i < q; i++) printf(" %d", answer[i]);
    printf("\nPointer moves: %lld (re-scanning every range: %lld)\n", moves,
           naiveWork);
    return 0;
}
