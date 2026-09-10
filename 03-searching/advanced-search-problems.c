/*
Advanced Search Problems Ladder (Basic -> Hard)
-----------------------------------------------
Real life example:
Rotated-array search is a daily agenda or a sorted log that wrapped
around midnight; peak finding is market/high-point detection.

DIAGRAM (rotated array [4,5,6,7,0,1,2], target 0):
  lo=0 hi=6 mid=3: arr[3]=7 >= arr[0]=4 -> LEFT half sorted
  0 not in [4..7] -> go right
  lo=4 hi=6 mid=5: arr[5]=1, arr[4]=0 -> left half [0] sorted? check
  range: 0 is in [0..1] -> go left -> found at index 6? trace prints it
  Exponential search: 1,2,4,8... double the window, then binary search.

LADDER (easy -> hard inside this file):
  1. Exponential search         (unbounded/doubled windows)
  2. Floor and Ceil             (binary search returning neighbors)
  3. Peak element               (binary search on a SHAPE, not a value)
  4. Search in rotated array    (which half is sorted?)
  5. Missing number             (sum method, folder-1 problem revisited with binary search)
  6. Find duplicate (Floyd)     (cycle detection without modification)

BEGINNER EXPLANATION:
Binary search assumes ONE sorted run. These problems bend it:
- Exponential: array may be "effectively infinite" - double the
  window until the target is bracketed, then binary search.
- Floor/Ceil: the search that FAILS still tells you WHERE the
  target would sit - the neighbors are the answer.
- Peak: the array "goes up then down" - compare mid with mid+1:
  rising means peak lies right.
- Rotated: a rotated sorted array has ONE sorted half; check
  which half contains the target by RANGE, not by value.

SUPER SIMPLE EXAMPLE (rotated, [4,5,6,7,0,1,2], target 0):
lo=0 hi=6 mid=3: arr[3]=7 >= arr[0]=4 -> left half sorted.
Is 0 in [4..7]? No -> go right: lo=4.
lo=4 hi=6 mid=5: arr[5]=1 >= arr[4]=0 -> left half [0..1] sorted.
0 in [0..1]? Yes -> hi=5. mid=4: arr[4]=0 == target. Found!

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Peak on a strictly increasing array: the LAST element is the
  peak (boundary) - the algorithm converges there naturally.
- Rotated array rotated by 0 (not rotated at all): every check
  still works - left half is always sorted.
- Duplicate peak values ([2,2,2]): pure binary search can fail;
  need the linear-worst fallback or randomized - noted in tips.
- Floor/ceil when target smaller than all: floor = -1 (none);
  larger than all: ceil = -1. Both are VALID answers, not errors.
- Floyd's duplicate: needs array of n+1 values in 1..n - the
  constraint IS the algorithm (pigeonhole creates the cycle).

DRY RUN (peak, [1,3,8,7,5]):
lo=0 hi=4 mid=2: arr[2]=8 > arr[3]=7 -> peak at mid or left: hi=2
lo=0 hi=2 mid=1: arr[1]=3 < arr[2]=8 -> peak right: lo=2
lo==hi==2 -> peak = index 2 (value 8). Correct.

TIPS:
- "Binary search on the ANSWER" (peak, rotated) is the pattern
  that unlocks hard interview problems - when a predicate is
  monotone, binary search applies even to non-sorted data.
- Exponential search = O(log i) where i is the target's position
  - better than plain O(log n) when the target is near the front.
- Floyd's tortoise-hare appears again for linked-list cycles in
  folder 06 - same math, different container.
*/

#include <stdio.h>

#define MAX_N 500

/*
 * exponentialSearch - Doubling window then binary search.
 */
int exponentialSearch(const int arr[], int n, int target) {
    if (arr[0] == target) {
        return 0;
    }
    int bound = 1;
    int steps = 1;
    while (bound < n && arr[bound] <= target) {
        printf("  bound -> %d (arr=%d)\n", bound, arr[bound]);
        bound = bound * 2;
        steps = steps + 1;
    }
    int lo = bound / 2;
    int hi = (bound < n) ? bound : n - 1;
    printf("  binary search in [%d..%d]\n", lo, hi);
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            printf("  found after ~%d steps total\n", steps);
            return mid;
        }
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

/*
 * floorCeil - Neighbors of target in a sorted array.
 */
void floorCeil(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n - 1;
    int floorIdx = -1;
    int ceilIdx = -1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            floorIdx = mid;
            ceilIdx = mid;
            break;
        } else if (arr[mid] < target) {
            floorIdx = mid;
            lo = mid + 1;
        } else {
            ceilIdx = mid;
            hi = mid - 1;
        }
    }

    printf("Floor: ");
    if (floorIdx == -1) {
        printf("none (target below all)\n");
    } else {
        printf("%d (index %d)\n", arr[floorIdx], floorIdx);
    }
    printf("Ceil:  ");
    if (ceilIdx == -1) {
        printf("none (target above all)\n");
    } else {
        printf("%d (index %d)\n", arr[ceilIdx], ceilIdx);
    }
}

/*
 * findPeak - Peak in a bitonic (up-then-down) array.
 */
int findPeak(const int arr[], int n) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < arr[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/*
 * searchRotated - Binary search in a rotated sorted array.
 */
int searchRotated(const int arr[], int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[lo] <= arr[mid]) {
            if (target >= arr[lo] && target < arr[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            if (target > arr[mid] && target <= arr[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}

/*
 * findDuplicateFloyd - Cycle detection, O(1) space.
 *
 * The value range 1..n is guaranteed by the caller, so the array length
 * n is only needed as documentation of that contract; the walk itself
 * follows arr[] as a linked list and never indexes past the cycle.
 */
int findDuplicateFloyd(const int arr[], int n) {
    (void)n;   /* the contract, not a computation */
    int slow = arr[0];
    int fast = arr[0];
    do {
        slow = arr[slow];
        fast = arr[arr[fast]];
    } while (slow != fast);

    slow = arr[0];
    while (slow != fast) {
        slow = arr[slow];
        fast = arr[fast];
    }
    return slow;
}

int main() {
    int arr[MAX_N];
    int n;
    int target;
    int choice;

    printf("Advanced Search Ladder (easy -> hard)\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers in ASCENDING order: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    while (1) {
        printf("\n1. Exponential search\n");
        printf("2. Floor and ceil\n");
        printf("3. Peak element (bitonic input - re-enter array)\n");
        printf("4. Search in rotated array (re-enter array)\n");
        printf("5. Missing number via binary search\n");
        printf("6. Duplicate via Floyd (special input format)\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Target: ");
            scanf("%d", &target);
            int pos = exponentialSearch(arr, n, target);
            if (pos != -1) {
                printf("Found %d at index %d\n", target, pos);
            } else {
                printf("%d not found\n", target);
            }
        } else if (choice == 2) {
            printf("Target: ");
            scanf("%d", &target);
            floorCeil(arr, n, target);
        } else if (choice == 3) {
            printf("Enter %d integers (rise then fall): ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            int p = findPeak(arr, n);
            printf("Peak at index %d (value %d)\n", p, arr[p]);
        } else if (choice == 4) {
            printf("Enter %d rotated sorted integers: ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            printf("Target: ");
            scanf("%d", &target);
            int pos = searchRotated(arr, n, target);
            if (pos != -1) {
                printf("Found at index %d\n", pos);
            } else {
                printf("%d not found\n", target);
            }
        } else if (choice == 5) {
            printf("Array is sorted with numbers 1..%d, one missing.\n", n + 1);
            printf("Enter %d sorted integers (skip one): ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            int lo = 0;
            int hi = n - 1;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (arr[mid] == mid + 1) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }
            printf("Missing number: %d\n", lo + 1);
        } else if (choice == 6) {
            int m;
            printf("Array has n+1 values in 1..n (one duplicated).\n");
            printf("Enter count m (2-%d): ", MAX_N - 1);
            scanf("%d", &m);
            printf("Enter %d values: ", m);
            for (int i = 0; i < m; i++) {
                scanf("%d", &arr[i]);
            }
            int dup = findDuplicateFloyd(arr, m);
            printf("Duplicate: %d (no modification, O(1) space)\n", dup);
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
