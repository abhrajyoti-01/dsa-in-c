/*
Heap Sort
---------
What this program solves:
This program sorts an array by first arranging it like a max heap, where the
largest element always stays at the root, and then repeatedly moving that
largest element to the correct final position.

Real life example:
Imagine a school is selecting the tallest student repeatedly from a group to
stand at the end of a line. If the tallest student can always be identified
quickly, then after placing that student at the end, the next tallest is found
from the remaining group. Heap sort works in a similar way.

DIAGRAM (max-heap then repeatedly extract the root):
  build: [4 10 3 5 1] -> heap [10 5 3 4 1]
         10
        /  \
       5    3
      / \
     4   1
  swap root with last -> 10 fixed at the end; sift down; repeat.
  sorted region grows from the RIGHT.

How the process works:
1. Convert the given array into a max heap.
2. The largest element now appears at the root.
3. Swap the root with the last unsorted element.
4. Reduce the heap size by one.
5. Restore heap property again.
6. Repeat until all elements are placed.

Pseudo code:
START
Read n and array elements
Build a max heap from the array
For end = n - 1 down to 1
    Swap root with last unsorted element
    Reduce heap size by 1
    Heapify the root again
Print the sorted array
END

Sample input and dry run:
Array = 4 10 3 5 1
After building a max heap, 10 comes to the root.
Swap 10 with the last element, so 10 goes to its final sorted position.
Now heapify the remaining part to bring the next largest element to the root.
Repeating this process produces the sorted order 1 3 4 5 10.

Main logic to understand:
The heap structure helps us quickly bring the maximum element to the front in
every pass.
*/
#include <stdio.h>

void heapify(int a[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int temp;

    if (left < n && a[left] > a[largest]) {
        largest = left;
    }

    if (right < n && a[right] > a[largest]) {
        largest = right;
    }

    if (largest != i) {
        temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        heapify(a, n, largest);
    }
}

void heapSort(int a[], int n) {
    int i, temp;

    for (i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }

    for (i = n - 1; i > 0; i--) {
        temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        heapify(a, i, 0);
    }
}

int main() {
    int a[100], n, i;

    printf("Heap Sort\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    heapSort(a, n);

    printf("Sorted elements are:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}

