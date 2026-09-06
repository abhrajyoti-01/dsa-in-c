/*
Min-Max Problem (Divide and Conquer)
------------------------------------
What this program solves:
Finds both the MINIMUM and MAXIMUM of an array using ~1.5n
comparisons instead of the naive 2n - by pairing elements and
recurring on splits.

Real life example:
Exam processing: the exam board needs both the topper (max) and
the lowest score (min) in one pass. Fewer comparisons = cheaper
hardware verification in cryptographic/sensor pipelines too.

DIAGRAM (pairwise min/max beats scanning twice):
  [20 5 7 40 1]
  pairs: (20,5)->min5 max20   (7,40)->min7 max40   leftover 1
  min of mins = 1, max of maxes = 40
  3 comparisons per 2 elements -> ~1.5n instead of 2n.

How the process works:
1. Naive baseline: n-1 comparisons for min, then n-1 for max
   = 2n - 2.
2. D&C version: pair elements, compare the pair once (smaller ->
   candidate min list, larger -> candidate max list), recurse.
   Recurrence: T(n) = 2T(n/2) + 2 -> ~3n/2 comparisons.
3. Leaf case (n == 2): one comparison decides both.

Pseudo code:
START
MINMAX(lo, hi):
    IF lo == hi: min = max = arr[lo]
    ELSE IF hi == lo+1: one comparison splits min/max
    ELSE:
        mid = (lo+hi)/2
        (Lmin, Lmax) = MINMAX(lo, mid)
        (Rmin, Rmax) = MINMAX(mid+1, hi)
        min = MIN(Lmin, Rmin)   (1 comparison)
        max = MAX(Lmax, Rmax)   (1 comparison)
END

Sample input and dry run:
[22, 13, -5, -8, 15, 60, 17, 31]
min -8, max 60 found with 1.5*8 = ~10-11 comparisons
(naive approach needs 14)

Main logic to understand:
T(n) = 2T(n/2) + 2 comparisons -> C(n) = 3n/2 - 2, the provable
minimum for simultaneous min & max. Pairing trick does the magic:
each pair "pays" one comparison that the naive method pays twice.
*/

#include <stdio.h>

#define MAX_N 500

int arr[MAX_N];
long long comparisons = 0;

/*
 * minmax - Fills min/max over arr[lo..hi]; counts comparisons.
 */
void minmax(int lo, int hi, int* mn, int* mx) {
    if (lo == hi) {
        *mn = arr[lo];
        *mx = arr[lo];
        return;
    }
    if (hi == lo + 1) {
        comparisons = comparisons + 1;
        if (arr[lo] < arr[hi]) {
            *mn = arr[lo];
            *mx = arr[hi];
        } else {
            *mn = arr[hi];
            *mx = arr[lo];
        }
        return;
    }

    int mid = (lo + hi) / 2;
    int lmin;
    int lmax;
    int rmin;
    int rmax;

    minmax(lo, mid, &lmin, &lmax);
    minmax(mid + 1, hi, &rmin, &rmax);

    comparisons = comparisons + 2;
    *mn = (lmin < rmin) ? lmin : rmin;
    *mx = (lmax > rmax) ? lmax : rmax;
}

int main() {
    int n;

    printf("Min-Max (Divide and Conquer, 1.5n comparisons)\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int mn;
    int mx;
    minmax(0, n - 1, &mn, &mx);

    printf("\nMinimum: %d\n", mn);
    printf("Maximum: %d\n", mx);
    printf("Comparisons used: %lld (theoretical best ~1.5n = %d)\n",
           comparisons, 3 * n / 2);
    printf("Naive scan would use: %d\n", (n >= 2) ? 2 * n - 2 : 0);

    return 0;
}
