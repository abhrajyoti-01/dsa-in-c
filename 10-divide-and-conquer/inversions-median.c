/*
Divide & Conquer Problems Ladder (Basic -> Hard)
-------------------------------------------------
Real life example:
Inversions measure how "unsorted" a ranking is - used to compare
two recommendation lists; median-of-two-arrays powers streaming
statistics without merging everything.

DIAGRAM (inversions counted during the MERGE step):
  left [2 4] right [1 3]
  take 1: 1 is smaller than 2 AND 4 -> +2 inversions
  take 2, take 3: 3 < 4 -> +1
  total = 3 inversions (3-2, 3-4, 4-1... trace prints pairs)

LADDER (easy -> hard inside this file):
  1. Power of a number (halving - warm-up)
  2. Count inversions (merge-sort based, THE classic)
  3. Median of two sorted arrays (partition logic - the hard one)

BEGINNER EXPLANATION:
- Inversions: pairs (i,j) with i<j but arr[i]>arr[j] - "how
  unsorted is this array?" Merge sort counts them for free:
  every time the merge takes a RIGHT element before some remaining
  LEFT elements, those remaining all form inversions - add them
  in a lump.
- Median of two sorted arrays: binary search the PARTITION of the
  smaller array; the correct split has every left-side element <=
  every right-side element. That is pure divide & conquer thinking
  applied to the median position.

SUPER SIMPLE EXAMPLE (inversions, [2, 4, 1, 3]):
Pairs out of order: (4,1), (2,1), (4?no 4<3 is false -> (4,3)!)
Inversions: (2,1), (4,1), (4,3) = 3.
Merge sort: split [2,4] [1,3]; merging: take 1 -> it jumps over
2 and 4 -> +2 inversions. Take 2, 4. Cross merge done. Then
merging [1,2,4] with [3]: 3 jumps over 4 -> +1. Total 3.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Sorted ascending array: 0 inversions (merge never takes right
  before left).
- Reverse sorted: n*(n-1)/2 inversions - the maximum.
- Equal elements: count STRICT inversions (equal pairs don't
  count) - the merge must take left on ties (<=).
- Median with even total length: average of the two middle
  elements; with one array empty: median of the other.
- Partition edge cases in the median search: left half may take
  0 or all elements - the +/-1 bounds in the code encode exactly
  those boundaries; off-by-one here is THE difficulty of this
  problem.

DRY RUN (median, A=[1,3], B=[2]):
n=2 m=1 total=3 odd -> answer at position 1 (0-based).
Search partition in smaller A: i = 1 (half). Left A = [1],
right A = [3]. Left B size = total/2 - i = 2 - 1 = 1 -> [2].
Check: maxLeftA=1 <= minRightB=2 yes; maxLeftB=2 <= minRightA=3
yes -> left half = {1, 2}, median (odd) = max(1, 2) = 2. Correct!

TIPS:
- Inversions = bubble-sort swap count = O(n^2) brute force ->
  merge-sort O(n log n) is the upgrade; the counting happens at
  merge time (lump additions, not per-pair).
- Median-two-arrays has a famous O(log min(m,n)) solution - the
  partition + parity check; writing it bug-free IS the challenge.
- Max subarray D&C lives in kadane.c comparisons (folder 11);
  Strassen (folder 10's original file) is the matrix version of
  the same "split-solve-merge" theme.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200

long long inversionCount = 0;

/*
 * mergeCount - Merges two sorted halves, counting lump inversions.
 */
void mergeCount(int arr[], int temp[], int lo, int mid, int hi) {
    int i = lo;
    int j = mid + 1;
    int k = lo;

    while (i <= mid && j <= hi) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            k = k + 1;
            i = i + 1;
        } else {
            temp[k] = arr[j];
            k = k + 1;
            j = j + 1;
            /* arr[j] jumps ahead of ALL remaining left elements */
            inversionCount = inversionCount + (mid - i + 1);
        }
    }
    while (i <= mid) {
        temp[k] = arr[i];
        k = k + 1;
        i = i + 1;
    }
    while (j <= hi) {
        temp[k] = arr[j];
        k = k + 1;
        j = j + 1;
    }
    for (i = lo; i <= hi; i++) {
        arr[i] = temp[i];
    }
}

void mergeSortCount(int arr[], int temp[], int lo, int hi) {
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSortCount(arr, temp, lo, mid);
    mergeSortCount(arr, temp, mid + 1, hi);
    mergeCount(arr, temp, lo, mid, hi);
}

/*
 * medianTwoSorted - Median via O(log min) partition.
 */
double medianTwoSorted(const int a[], int m, const int b[], int n) {
    const int* small = a;
    const int* big = b;
    int ns = m;
    int nb = n;
    if (m > n) {
        small = b;
        big = a;
        ns = n;
        nb = m;
    }

    int total = m + n;
    int half = (total + 1) / 2;

    int lo = 0;
    int hi = ns;
    while (lo <= hi) {
        int i = lo + (hi - lo) / 2;
        int j = half - i;

        int maxLeftA = (i == 0) ? -2000000000 : small[i - 1];
        int minRightA = (i == ns) ? 2000000000 : small[i];
        int maxLeftB = (j == 0) ? -2000000000 : big[j - 1];
        int minRightB = (j == nb) ? 2000000000 : big[j];

        if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
            if (total % 2 == 1) {
                double med = (maxLeftA > maxLeftB) ? maxLeftA : maxLeftB;
                return med;
            }
            int l = (maxLeftA > maxLeftB) ? maxLeftA : maxLeftB;
            int r = (minRightA < minRightB) ? minRightA : minRightB;
            return (l + r) / 2.0;
        } else if (maxLeftA > minRightB) {
            hi = i - 1;
        } else {
            lo = i + 1;
        }
    }
    return 0.0;
}

int main() {
    int arr[MAX_N];
    int temp[MAX_N];
    int n;
    int choice;

    printf("Divide & Conquer Problems Ladder (easy -> hard)\n\n");

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

    while (1) {
        printf("\n1. Count inversions (merge-sort based)\n");
        printf("2. Median of two sorted arrays (enter second array)\n");
        printf("3. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int copy[MAX_N];
            for (int i = 0; i < n; i++) {
                copy[i] = arr[i];
            }
            inversionCount = 0;
            mergeSortCount(copy, temp, 0, n - 1);
            printf("Inversions: %lld\n", inversionCount);
            printf("Maximum possible for n=%d: %d (reverse sorted)\n",
                   n, n * (n - 1) / 2);
        } else if (choice == 2) {
            int m;
            printf("Size of second sorted array (1-%d)? ", MAX_N);
            scanf("%d", &m);
            int b[MAX_N];
            printf("Enter %d sorted integers: ", m);
            for (int i = 0; i < m; i++) {
                scanf("%d", &b[i]);
            }
            int a2[MAX_N];
            for (int i = 0; i < n; i++) {
                a2[i] = arr[i];
            }
            /* sort a2 with merge sort (reuse the counter silently) */
            inversionCount = 0;
            mergeSortCount(a2, temp, 0, n - 1);
            double med = medianTwoSorted(a2, n, b, m);
            printf("Median of the two arrays: %.2f\n", med);
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
