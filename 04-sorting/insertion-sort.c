/*
Insertion Sort
--------------
What this program solves:
Sorts by INSERTING each element into its correct place within the
already-sorted left portion - like sorting playing cards in hand.

Real life example:
Sorting playing cards while dealing: each new card is slid into its
rightful position among the cards already held in your hand.

DIAGRAM (inserting 5 into sorted hand [11, 12, 13]):
  hand: 11 12 13   key = 5
  13 > 5 -> shift right: _ 11 12 13
  12 > 5 -> shift right: _ _ 11 12 13
  11 > 5 -> shift right: _ _ _ 11 12 13
  nothing smaller -> drop 5 in the gap: [5, 11, 12, 13]
Two runners: j shifts, key waits for its slot.

How the process works:
1. Take key = arr[i] (the next unsorted element).
2. Shift all larger elements of the sorted region one step right.
3. Drop the key into the gap.

Pseudo code:
START
FOR i = 1..n-1
    key = arr[i]
    j = i - 1
    WHILE j >= 0 AND arr[j] > key
        arr[j+1] = arr[j]
        j = j - 1
    arr[j+1] = key
END

Sample input and dry run:
[12, 11, 13, 5, 6]
i=1: [11, 12, 13, 5, 6]
i=2: [11, 12, 13, 5, 6]
i=3: [5, 11, 12, 13, 6]
i=4: [5, 6, 11, 12, 13]

Main logic to understand:
Shifting instead of swapping makes it fast on nearly-sorted data:
O(n) best case, O(n^2) worst/average. Stable; in-place; the go-to
sort for tiny arrays inside hybrid sorts (like Introsort/Timsort).
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
 * insertionSort - Slides each element left into the sorted region.
 */
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int arr[100];
    int n;

    printf("Insertion Sort\n\n");

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

    insertionSort(arr, n);

    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}