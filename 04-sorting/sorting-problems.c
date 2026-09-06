/*
Sorting Problems Ladder - Detection & Applications (Basic -> Hard)
-------------------------------------------------------------------
Real life example:
The Dutch flag is load balancing by category (small/medium/large
jobs in ONE pass); minimum swaps to sort measures how broken a
ranking is - used to score recommendation lists.

DIAGRAM (Dutch national flag on [2,0,1,2,0]):
  low=0 mid=0 high=4
  a[0]=2 -> swap mid/high -> [0,0,1,2,2], high--
  a[0]=0 -> swap low/mid  -> low++, mid++
  a[1]=0 -> swap low/mid  -> low++, mid++
  a[2]=1 -> mid++         -> [0,0,1,2,2] sorted in one pass

LADDER (easy -> hard inside this file):
  1. Is the array sorted? (ascending/descending detection)
  2. Sort 0s, 1s, 2s (Dutch national flag - single pass!)
  3. Merge sorted arrays in place (gap method concept)
  4. Minimum swaps to sort (cycle decomposition)

BEGINNER EXPLANATION:
- Dutch national flag: three pointers - low (next 0 slot), mid
  (current), high (next 2 slot). One pass, no counting, no extra
  array: 0s go left, 2s go right, 1s stay in the middle.
- Minimum swaps to sort: think of the array as a GRAPH where each
  element points to where it should go. Each cycle of length L
  needs L-1 swaps. Total = sum over cycles. Sorting insight
  disguised as graph insight!

SUPER SIMPLE EXAMPLE (0/1/2 sort, [2,0,1,2,0]):
low=0 mid=0 high=4.
a[0]=2 -> swap with high (2), high-- -> [0,0,1,2,2]? careful:
a[0] now 0 -> swap with low, low++, mid++ -> [0,...]
Trace the program output - the state prints live.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- All same elements: sorted (both directions "true" for a single
  element or all-equal - the program reports both).
- Dutch flag with only two values: it degenerates to the classic
  0/1 partition (quick sort's step).
- Minimum swaps with duplicates: cycle method assumes DISTINCT
  elements (or index-aware pairing); duplicates need a different
  pairing strategy (noted, not implemented).
- Descending sorted input to "is ascending": false - direction
  detection must check BOTH and report which.

DRY RUN (min swaps, [4, 3, 2, 1]):
Position-sorted pairing: 4 should be at index 3, 3 at index 2...
cycles: (0 3) and (1 2) -> each length 2 -> 1 swap each = 2 swaps.
Indeed: swap 4<->1, swap 3<->2 -> sorted in 2.

TIPS:
- Dutch national flag is quicksort's 3-way partition - it fixes
  quicksort's duplicate-heavy worst case (Dijkstra's design).
- Cycle decomposition returns in: minimum swaps to group balls,
  permutation restoration, and even rubik-style puzzles.
- These are the "sorting WITHOUT sorting" family - the answer
  falls out of structure, not comparisons.
*/

#include <stdio.h>

#define MAX_N 100

void printArray(const int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int arr[MAX_N];
    int n;
    int choice;

    printf("Sorting Problems Ladder (easy -> hard)\n\n");

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

    printf("Array: ");
    printArray(arr, n);

    while (1) {
        printf("\n1. Is sorted? (ascending / descending)\n");
        printf("2. Sort 0s, 1s, 2s (Dutch national flag)\n");
        printf("3. Minimum swaps to sort (cycle method)\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int ascending = 1;
            int descending = 1;
            for (int i = 1; i < n; i++) {
                if (arr[i] < arr[i - 1]) {
                    ascending = 0;
                }
                if (arr[i] > arr[i - 1]) {
                    descending = 0;
                }
            }
            if (ascending && !descending) {
                printf("Sorted ASCENDING\n");
            } else if (descending && !ascending) {
                printf("Sorted DESCENDING\n");
            } else if (ascending && descending) {
                printf("All elements equal (both directions true)\n");
            } else {
                printf("Not sorted\n");
            }
        } else if (choice == 2) {
            printf("This menu expects only 0s, 1s and 2s.\n");
            printf("Enter %d values (0/1/2): ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            int low = 0;
            int mid = 0;
            int high = n - 1;
            while (mid <= high) {
                if (arr[mid] == 0) {
                    int t = arr[low];
                    arr[low] = arr[mid];
                    arr[mid] = t;
                    low = low + 1;
                    mid = mid + 1;
                } else if (arr[mid] == 2) {
                    int t = arr[mid];
                    arr[mid] = arr[high];
                    arr[high] = t;
                    high = high - 1;
                } else {
                    mid = mid + 1;
                }
            }
            printf("Result: ");
            printArray(arr, n);
        } else if (choice == 3) {
            int copy[MAX_N];
            int sorted[MAX_N];
            int pos[MAX_N];
            for (int i = 0; i < n; i++) {
                copy[i] = arr[i];
                sorted[i] = arr[i];
            }
            /* sort a copy (insertion - n is small) */
            for (int i = 1; i < n; i++) {
                int key = sorted[i];
                int j = i - 1;
                while (j >= 0 && sorted[j] > key) {
                    sorted[j + 1] = sorted[j];
                    j = j - 1;
                }
                sorted[j + 1] = key;
            }
            /* map value -> target index (first unused match) */
            int used[MAX_N] = {0};
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (!used[j] && sorted[j] == copy[i]) {
                        pos[i] = j;
                        used[j] = 1;
                        break;
                    }
                }
            }
            /* cycle decomposition on the permutation */
            int visited[MAX_N] = {0};
            int swaps = 0;
            for (int i = 0; i < n; i++) {
                if (visited[i] || pos[i] == i) {
                    continue;
                }
                int cycleLen = 0;
                int j = i;
                while (!visited[j]) {
                    visited[j] = 1;
                    j = pos[j];
                    cycleLen = cycleLen + 1;
                }
                if (cycleLen > 1) {
                    swaps = swaps + cycleLen - 1;
                }
            }
            printf("Minimum swaps to sort: %d\n", swaps);
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
