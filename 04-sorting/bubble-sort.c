/*
Bubble Sort
-----------
What this program solves:
Sorts an array by repeatedly swapping adjacent elements that are in
the wrong order. With the swapped flag optimization, an already
sorted array exits after one pass.

Real life example:
Bubbles rising in water - the largest value "bubbles up" to the end
of the array on each pass, just like the largest bubble reaches the
surface first.

DIAGRAM (one full pass, [5, 2, 9, 1] - largest bubbles right):
  compare 5,2: swap  -> [2, 5, 9, 1]
  compare 5,9: ok    -> [2, 5, 9, 1]
  compare 9,1: swap  -> [2, 5, 1, 9]   (9 "bubbled" to the end)
  pass done: last position FINAL - shrink the window, repeat:
  [2, 5, 1] -> [2, 1, 5] -> [1, 2, 5] ... until no swaps happen.

Pseudo code:
START
FOR i = 0..n-2
    swapped = false
    FOR j = 0..n-2-i
        IF arr[j] > arr[j+1]: swap, swapped = true
    IF not swapped: break
END

Sample input and dry run:
[5, 2, 9, 1] -> pass 1: [2, 5, 1, 9] -> pass 2: [2, 1, 5, 9]
-> pass 3: [1, 2, 5, 9] sorted

Main logic to understand:
Adjacent comparisons push the maximum of the unsorted part to the
end each pass. Time O(n^2) worst/average, O(n) best (already
sorted); Space O(1); stable; in-place.
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
 * bubbleSort - Sorts ascending with early exit when no swap occurs.
 */
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

int main() {
    int arr[100];
    int n;

    printf("Bubble Sort\n\n");

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

    bubbleSort(arr, n);

    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}