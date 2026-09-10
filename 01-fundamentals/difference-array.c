/*
Difference Array - Fast Batch Range Additions (Basic -> Intermediate)
-------------------------------------------------------------------
PROBLEM DEFINITION:
Add a value to every element in several inclusive ranges [l..r], then
print the final array. A difference array makes each update O(1).

REAL-LIFE EXAMPLE:
A booking system adds passenger counts to consecutive train segments.
Record where each booking starts and ends, then total all bookings once.

DIAGRAM:
  original:       2   4   1   3
  difference:     2   2  -3   2
  add 5 to [1..2]:    +5      -5  (stop the addition at r + 1)
  new difference: 2   7  -3  -3
  prefix sums:    2   9   6   3

PSEUDO CODE:
  diff[0] = a[0]; diff[i] = a[i] - a[i-1]
  For each update (l, r, delta):
      diff[l] += delta
      diff[r+1] -= delta
  running = 0
  For i = 0..n-1:
      running += diff[i]; print running

SAMPLE INPUT & DRY RUN:
  4
  2 4 1 3
  2
  1 2 5
  0 3 -1
  First update gives [2,9,6,3]. Second gives [1,8,5,2].
  Final difference entries are [1,7,-3,-3]; prefix sums recover it.

SPECIAL CASES:
  - Zero updates leaves the original array unchanged.
  - Negative additions and a one-element range work unchanged.
  - diff[n] is a spare sentinel for updates ending at n-1.
  - Indices are zero-based; reversed or out-of-bounds ranges are rejected.
  - n and update count <= 1000; values/deltas are within +/-10^9.
    These limits keep all intermediate sums safely inside long long.

COMPLEXITY:
  O(n + q) time, O(n) space for q updates; no O(n) scan per update.

TIPS:
  This is an OFFLINE technique: reconstruct after all updates.
  Use a Fenwick tree or lazy segment tree for interleaved queries.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_UPDATES 1000
#define VALUE_LIMIT 1000000000LL

int main(void) {
    long long diff[MAX_N + 1] = {0};
    long long previous = 0;
    int n;
    int q;

    printf("Difference Array - Batch Range Additions\n\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers (within +/-10^9): ", n);
    for (int i = 0; i < n; i++) {
        long long value;
        if (scanf("%lld", &value) != 1 ||
            value < -VALUE_LIMIT || value > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        diff[i] = value - previous;
        previous = value;
    }

    printf("How many updates (0-%d)? ", MAX_UPDATES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_UPDATES) {
        printf("Invalid update count\n");
        return 1;
    }
    printf("Enter updates as 'l r delta' (zero-based, inclusive):\n");
    for (int i = 0; i < q; i++) {
        int l;
        int r;
        long long delta;
        if (scanf("%d %d %lld", &l, &r, &delta) != 3 ||
            l < 0 || r >= n || l > r ||
            delta < -VALUE_LIMIT || delta > VALUE_LIMIT) {
            printf("Invalid update\n");
            return 1;
        }
        diff[l] += delta;
        diff[r + 1] -= delta;
    }

    long long running = 0;
    printf("Final array:");
    for (int i = 0; i < n; i++) {
        running += diff[i];
        printf(" %lld", running);
    }
    printf("\n");
    return 0;
}
