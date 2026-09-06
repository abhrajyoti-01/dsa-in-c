/*
Selection Sort
--------------
What this program solves:
Sorts an array by repeatedly SELECTING the minimum element from the
unsorted part and moving it to the front. Exactly n-1 swaps - the
fewest of all simple sorts.

Real life example:
Sorting exam papers by marks: scan all unsorted papers, pick the
lowest, place it first; repeat with the remaining pile. You always
"select" the next smallest.

DIAGRAM (each pass: find min of the unsorted region, swap to front):
  [64 | 25 12 22 11]   min of unsorted = 11 -> swap with 64
  [11 25 | 12 22 64]   min = 12 -> swap with 25
  [11 12 22 | 25 64]   min = 22 (already in place)
  [11 12 22 25 | 64]   sorted region grows from the left
  |----sorted--|        |---unsorted---|

How the process works:
1. Find the index of the minimum element in arr[i..n-1].
2. Swap it with arr[i].
3. The sorted region grows by one from the left each pass.

Pseudo code:
START
FOR i = 0..n-2
    minIdx = i
    FOR j = i+1..n-1
        IF arr[j] < arr[minIdx]: minIdx = j
    SWAP arr[i], arr[minIdx]
END

Sample input and dry run:
[64, 25, 12, 22, 11]
pass 1: min 11 -> [11, 25, 12, 22, 64]
pass 2: min 12 -> [11, 12, 25, 22, 64]
pass 3: min 22 -> [11, 12, 22, 25, 64]
pass 4: min 25 -> already placed -> sorted

Main logic to understand:
Selection of the minimum for each position - swaps are O(n) total
but comparisons remain O(n^2) in ALL cases. Not stable (in this
simple form); in-place.
*/

#include <stdio.h>

void printArray(const int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

/*
 * selectionSort - Picks the minimum of the unsorted region each pass.
 */
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            int temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
        }
    }
}

int main() {
    int arr[100];
    int n;

    printf("Selection Sort\n\n");

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

    printf("\nOriginal: ");
    printArray(arr, n);

    selectionSort(arr, n);

    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}