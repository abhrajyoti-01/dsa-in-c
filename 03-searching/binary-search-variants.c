/*
Binary Search Variants - First/Last Occurrence, Count, Lower/Upper Bound
------------------------------------------------------------------------
What this program solves:
The four binary search variants every interview uses: first
occurrence, last occurrence, count of a value in a sorted array,
and lower/upper bound - all via the "search boundary" mindset.

Real life example:
Type-ahead dictionaries highlight EVERY match of a prefix: first
and last occurrence bound the range - exactly these variants.

DIAGRAM (first/last occurrence of 2 in [1,2,2,2,3]):
  plain:      hit index 2 -> stop (only one answer)
  first occ:  hit 2@2 -> record, KEEP LEFT: hi = 1 -> hit 2@1 -> index 1
  last occ:   hit 2@2 -> record, KEEP RIGHT: lo = 3 -> hit 2@3 -> index 3
  count = last - first + 1 = 3

BEGINNER EXPLANATION (the simplest way to think):
Plain binary search answers "does it exist?" But sorted arrays
have DUPLICATES - "where does it START and END?" matters more.
Think of it as walking toward the answer but NEVER STOPPING at a
match: record it, then keep pushing LEFT (for first) or RIGHT
(for last). The boundary never lies.

SUPER SIMPLE EXAMPLE:
arr = [1, 2, 2, 2, 3], target = 2
plain binary search: middle index 2 is 2 -> returns 2. Correct but
incomplete!
first occurrence: keep searching left half after a hit -> index 1
last occurrence:  keep searching right half after a hit -> index 3
count = last - first + 1 = 3

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Target absent: both variants return -1; count = 0. Lower bound
  returns where it WOULD be inserted (first index with value >=
  target); upper bound = first index with value > target.
- Target smaller than everything: lower bound = 0 (insert at front).
- Target larger than everything: lower bound = n (insert at end).
- All elements equal to target: first = 0, last = n-1, count = n.
- The "count via bounds" identity: count = upper_bound - lower_bound
  ALWAYS, even when absent (both give the same insertion point).

DRY RUN (first occurrence, arr = [1,2,2,2,3], target = 2):
lo=0 hi=4 mid=2: arr[2]=2 -> record 2, push left: hi=1
lo=0 hi=1 mid=0: arr[0]=1 < 2 -> lo=1
lo=1 hi=1 mid=1: arr[1]=2 -> record 1, push left: hi=0
lo>hi stop. First occurrence = 1 (found in 3 steps)

TIPS:
- while (lo < hi) + hi = mid style gives "leftmost" search;
  while (lo <= hi) + record-and-continue gives explicit variants -
  pick ONE style per function and stay consistent.
- mid = lo + (hi - lo) / 2 avoids overflow (lo + hi could overflow
  in other languages; C ints too for huge arrays).
- count in O(log n) beats linear scanning ALWAYS - the whole point
  of sorted data.
- These four functions are the building blocks of "range sum on
  sorted data", "smallest missing", "peak element" style problems.
*/

#include <stdio.h>

#define MAX_N 500

/*
 * binarySearchFirst - Leftmost index of target or -1.
 */
int binarySearchFirst(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            hi = mid - 1;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

/*
 * binarySearchLast - Rightmost index of target or -1.
 */
int binarySearchLast(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            lo = mid + 1;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

/*
 * lowerBound - First index with value >= target (insertion point).
 */
int lowerBound(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/*
 * upperBound - First index with value > target.
 */
int upperBound(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int main() {
    int arr[MAX_N];
    int n;
    int target;

    printf("Binary Search Variants\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers in ASCENDING order (duplicates allowed): ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter target: ");
    scanf("%d", &target);

    int first = binarySearchFirst(arr, n, target);
    int last = binarySearchLast(arr, n, target);
    int lb = lowerBound(arr, n, target);
    int ub = upperBound(arr, n, target);

    printf("\nArray: [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
    printf("Target: %d\n\n", target);

    if (first != -1) {
        printf("First occurrence:  index %d\n", first);
        printf("Last occurrence:   index %d\n", last);
        printf("Count:             %d\n", last - first + 1);
    } else {
        printf("%d is not present\n", target);
    }

    printf("Lower bound:       index %d (first >= %d)\n", lb, target);
    printf("Upper bound:       index %d (first >  %d)\n", ub, target);
    printf("Count via bounds:  %d (upper - lower)\n", ub - lb);

    if (lb >= 0 && lb <= n) {
        printf("If inserted, %d would go at index %d\n", target, lb);
    }

    return 0;
}
