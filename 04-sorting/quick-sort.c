/*
Quick Sort
----------
What this program solves:
This program sorts an array by selecting one element called pivot and arranging
all smaller elements on one side and larger elements on the other side.

Real life example:
Imagine a teacher asks students to stand in a line according to height. One
student is chosen as a reference. Everyone shorter stands to the left and everyone
taller stands to the right. After that, the same idea is repeated in both groups.
That is how quick sort works.

DIAGRAM (partition with pivot 5, array 5 3 8 4 2):
  pivot: 5
  [5 | 3 8 4 2]            (pivot separated)
  smaller: 3 4 2   larger: 8
  after: [4 3 2] 5 [8]
          |        |
       recurse  recurse
Result: 2 3 4 5 8

Recursion tree:
              [5 3 8 4 2]
             /            \
       [4 3 2] 5           [8]
       /      \
   [3 2] 4   [ ]? -> smaller parts
Each level does O(n) work; height O(log n) when balanced ->
O(n log n) total.

How the process works:
1. Choose a pivot element.
2. Rearrange the array so smaller elements move left of pivot.
3. Move larger elements to the right of pivot.
4. Put the pivot into its final correct position.
5. Apply the same process on the left and right parts.

Pseudo code:
START
Read n and array elements
Call quickSort(low, high)
In quickSort
    If low < high
        Partition array around pivot
        Recursively sort left part
        Recursively sort right part
Print sorted array
END

Sample input and dry run:
Array = 5 3 8 4 2
Choose 5 as pivot.
After partitioning, smaller elements move left and larger ones move right.
The array becomes something like 4 3 2 5 8.
Now quick sort runs again on [4 3 2] and on [8].
After the recursive steps, the final sorted order becomes 2 3 4 5 8.

Main logic to understand:
Once partitioning is done, the pivot reaches the exact position it should have
in the final sorted array.
*/
#include <stdio.h>

int partition(int a[], int low, int high) {
    int pivot = a[low];
    int i = low + 1;
    int j = high;
    int temp;

    while (i <= j) {
        while (i <= high && a[i] <= pivot) {
            i++;
        }

        while (a[j] > pivot) {
            j--;
        }

        if (i < j) {
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    temp = a[low];
    a[low] = a[j];
    a[j] = temp;

    return j;
}

void quickSort(int a[], int low, int high) {
    int pivotIndex;

    if (low < high) {
        pivotIndex = partition(a, low, high);
        quickSort(a, low, pivotIndex - 1);
        quickSort(a, pivotIndex + 1, high);
    }
}

int main() {
    int a[100], n, i;

    printf("Quick Sort\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    quickSort(a, 0, n - 1);

    printf("Sorted elements are:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}

