/*
Linear Search
-------------
What this program solves:
Finds a target value by scanning every element from left to right.
Works on ANY array - sorted or not - and also reports all duplicate
positions.

Real life example:
Looking for your friend in an unorganized crowd: you check face by
face until you find them - there is no shortcut without order.

DIAGRAM (search 30 in [10, 50, 30, 70]):
  [10] 50 30 70   -> 10 != 30, move on
   10 [50] 30 70  -> 50 != 30, move on
   10  50 [30] 70 -> 30 == 30 -> FOUND at index 2
  worst case checks every box: O(n).

How the process works:
1. Compare arr[i] with the target for i = 0..n-1.
2. First match -> return index (or report all matches).
3. Loop ends -> element is absent.

Pseudo code:
START
FOR i = 0..n-1
    IF arr[i] == target: RETURN i
RETURN -1
END

Sample input and dry run:
arr = [10, 50, 30, 70, 80, 60], target = 30
compare 10, 50, 30 -> found at index 2 (3 comparisons)

Main logic to understand:
O(n) time always - the baseline all other searches try to beat;
O(1) space; works on unsorted data.
*/

#include <stdio.h>

int main() {
    int arr[100];
    int n;
    int target;

    printf("Linear Search\n\n");

    printf("How many elements (1-100)? ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter value to search: ");
    scanf("%d", &target);

    printf("\nArray: [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");

    /* STEP: scan every element, report first and all matches */
    int firstPos = -1;
    int comparisons = 0;
    int count = 0;

    printf("Scanning:\n");
    for (int i = 0; i < n; i++) {
        comparisons = comparisons + 1;
        if (arr[i] == target) {
            if (firstPos == -1) {
                firstPos = i;
                printf("  Found %d at index %d (after %d comparisons)\n",
                       target, i, comparisons);
            }
            printf("  Duplicate also at index %d\n", i);
            count = count + 1;
        }
    }

    if (firstPos == -1) {
        printf("  %d not found after %d comparisons\n", target, comparisons);
    } else {
        printf("Total occurrences: %d\n", count);
    }

    return 0;
}
