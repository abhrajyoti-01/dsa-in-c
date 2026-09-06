/*
Heap (Priority Queue)
---------------------
What this program solves:
This program implements a Max Heap supporting insert, extract maximum,
peek and heap sort.

Real life example:
A hospital emergency room treats patients by severity, not arrival time.
The most critical patient is always at the front. A max heap does exactly
this: the largest element is always at the root, so serving the highest
priority item takes only O(log n) work to reorganize.

DIAGRAM (tree shape and its array storage, [20, 10, 15]):
  tree:          array:
        20       index:  0    1    2
       /  \               20 | 10 | 15
     10    15
    (root is the max; children of index i live at 2i+1 and 2i+2)

Insert 25: place at next free slot, bubble UP:
        20                25
       /  \              /  \
     10    15    ->    10    15
    ...                 \   (25 > parent 20 -> swap; 25 > 10 -> swap)
                         25
Extract max 25: move last (10) to root, bubble DOWN against
larger child.

How the process works:
1. The heap is stored in an array. For index i:
   parent = (i-1)/2, left child = 2i+1, right child = 2i+2.
2. Insert: place the new element at the end, then heapify up by
   swapping with the parent while it is larger.
3. Extract max: remove the root, move the last element to the root,
   then heapify down by swapping with the larger child.
4. Heap sort: build a heap, then repeatedly extract the max.

Pseudo code:
START
Insert(value)
    Place value at end of array
    While value bigger than parent, swap with parent
ExtractMax()
    Save root as maximum
    Move last element to root
    While a child is bigger, swap with the larger child
END

Sample input and dry run:
Insert 10, 20, 15
10 becomes root. 20 is bigger than 10 so they swap: [20, 10].
15 goes under 20; 15 < 20 so no swap: [20, 10, 15].
Extract max returns 20; last element 15 moves to root: [15, 10].

Main logic to understand:
The complete binary tree shape lets us store it in a plain array with
no pointers, and the heap property (parent >= children) keeps the
maximum instantly available at index 0.
*/
#include <stdio.h>
#include <stdlib.h>

int heap[100];
int heapSize = 0;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(int index) {
    while (index > 0 && heap[(index - 1) / 2] < heap[index]) {
        swap(&heap[(index - 1) / 2], &heap[index]);
        index = (index - 1) / 2;
    }
}

void heapifyDown(int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapSize && heap[left] > heap[largest]) {
        largest = left;
    }
    if (right < heapSize && heap[right] > heap[largest]) {
        largest = right;
    }

    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapifyDown(largest);
    }
}

void insert(int value) {
    heap[heapSize] = value;
    heapifyUp(heapSize);
    heapSize++;
}

int extractMax() {
    int max = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
    return max;
}

void display() {
    for (int i = 0; i < heapSize; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
}

void heapSort(int a[], int n) {
    heapSize = 0;
    for (int i = 0; i < n; i++) {
        insert(a[i]);
    }
    for (int i = n - 1; i >= 0; i--) {
        a[i] = extractMax();
    }
}

int main() {
    int choice, value, n;

    while (1) {
        printf("\nMax Heap (Priority Queue)\n");
        printf("1. Insert\n");
        printf("2. Extract maximum\n");
        printf("3. Peek maximum\n");
        printf("4. Display heap\n");
        printf("5. Heap sort (enter new array)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter value: ");
            scanf("%d", &value);
            insert(value);
            printf("Inserted %d\n", value);
        } else if (choice == 2) {
            if (heapSize == 0) {
                printf("Heap is empty\n");
            } else {
                printf("Extracted maximum: %d\n", extractMax());
            }
        } else if (choice == 3) {
            if (heapSize == 0) {
                printf("Heap is empty\n");
            } else {
                printf("Maximum element: %d\n", heap[0]);
            }
        } else if (choice == 4) {
            printf("Heap array: ");
            display();
        } else if (choice == 5) {
            printf("Enter number of elements: ");
            scanf("%d", &n);
            int arr[100];
            printf("Enter the elements:\n");
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            heapSort(arr, n);
            printf("Sorted array: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
