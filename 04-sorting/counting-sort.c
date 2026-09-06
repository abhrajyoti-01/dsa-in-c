/*
Counting Sort
-------------
What this program solves:
Sorts integers in a KNOWN SMALL RANGE by counting occurrences -
no comparisons at all, achieving true linear time.

Real life example:
Sorting exam scores that range from 0 to 100. Instead of comparing
papers, make 101 piles (one per possible score), drop each paper in
its pile, then read the piles in order.

DIAGRAM (count, accumulate, place - [2,1,1,0,2], k=3):
  count:   [1, 2, 2]        (how many 0s, 1s, 2s)
  prefix:  [1, 3, 5]        (how many <= i -> final positions)
  place 2 -> index 4, place 1 -> index 2, place 1 -> index 1, place 0
  result: [0, 1, 1, 2, 2]   O(n + k), no comparisons at all.

How the process works:
1. Find the maximum value k to size the count array.
2. count[v] = number of elements equal to v.
3. Prefix-sum count so count[v] = number of elements <= v
   (this gives each element its final position).
4. Place elements into the output array RIGHT-TO-LEFT so equal
   elements keep their original order (stability).

Pseudo code:
START
max = MAX(arr)
count[0..max] = 0
FOR each v in arr: count[v] = count[v] + 1
FOR v = 1..max: count[v] = count[v] + count[v-1]
FOR i = n-1..0: output[--count[arr[i]]] = arr[i]
END

Sample input and dry run:
[4, 2, 2, 8, 3, 3, 1]
count: 1->1, 2->2, 3->2, 4->1, 8->1
prefix: 1, 3, 5, 6, 7
output (right to left): [1, 2, 2, 3, 3, 4, 8]

Main logic to understand:
Trading comparisons for counting: O(n + k) time where k is the
range, O(n + k) space. Stable. Perfect when k is small; terrible
when values are sparse or huge (use radix sort on top instead).
*/

#include <stdio.h>
#include <stdlib.h>

void printArray(const int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

/*
 * countingSort - Non-comparison sort for non-negative integers.
 */
void countingSort(const int arr[], int n, int out[]) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    int* count = (int*)calloc((size_t)max + 1, sizeof(int));
    if (count == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        count[arr[i]] = count[arr[i]] + 1;
    }

    for (int v = 1; v <= max; v++) {
        count[v] = count[v] + count[v - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        count[arr[i]] = count[arr[i]] - 1;
        out[count[arr[i]]] = arr[i];
    }

    free(count);
}

int main() {
    int arr[200];
    int out[200];
    int n;

    printf("Counting Sort\n\n");

    printf("How many elements (1-200)? ");
    scanf("%d", &n);

    if (n < 1 || n > 200) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d NON-NEGATIVE integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        if (arr[i] < 0) {
            printf("Negative values not supported\n");
            return 1;
        }
    }

    printf("\nOriginal: ");
    printArray(arr, n);

    countingSort(arr, n, out);

    printf("Sorted:   ");
    printArray(out, n);

    return 0;
}
