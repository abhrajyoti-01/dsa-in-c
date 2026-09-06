/*
Merge Sort
----------
What this program solves:
This program sorts an array by dividing it into smaller parts, sorting those
parts, and then combining them back in order.

Real life example:
Imagine a teacher has a huge stack of answer sheets with roll numbers in random
order. Instead of sorting the entire stack at once, the teacher divides it into
smaller bundles, sorts each bundle, and then merges the sorted bundles. That is
exactly the idea behind merge sort.

DIAGRAM (divide and merge, [8, 3, 6, 2]):
  [8   3   6   2]
       /        \
  [8  3]      [6  2]       (split)
   /   \       /   \
  [8] [3]   [6] [2]        (singletons - trivially sorted)
   \   /       \   /
  [3  8]     [2  6]        (merge: compare front elements)
       \        /
     [2  3  6  8]            (final merge)

Merge step detail: [3,8] + [2,6]:
take 2 (2<3), take 3, take 6, dump 8 -> [2,3,6,8]. One pass.

How the process works:
1. Divide the array into two halves.
2. Keep dividing until each part has only one element.
3. A single element is already sorted by itself.
4. Start merging nearby parts in sorted order.
5. Continue merging until the full array becomes sorted.

Pseudo code:
START
Read n and array elements
Call mergeSort(low, high)
In mergeSort
    If low < high
        Find mid
        Sort left half
        Sort right half
        Merge the two sorted halves
Print sorted array
END

Sample input and dry run:
Array = 8 3 6 2
First split into [8 3] and [6 2]
Then split into [8], [3], [6], [2]
Merge [8] and [3] into [3 8]
Merge [6] and [2] into [2 6]
Finally merge [3 8] and [2 6] into [2 3 6 8]

Main logic to understand:
The power of merge sort comes from the merge step. Since the left and right
halves are already sorted, we only compare the front elements and copy the
smaller one first.
*/
#include <stdio.h>

void merge(int a[], int low, int mid, int high) {
    int temp[100], i, j, k;

    i = low;
    j = mid + 1;
    k = low;

    while (i <= mid && j <= high) {
        if (a[i] <= a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = a[i++];
    }

    while (j <= high) {
        temp[k++] = a[j++];
    }

    for (i = low; i <= high; i++) {
        a[i] = temp[i];
    }
}

void mergeSort(int a[], int low, int high) {
    int mid;

    if (low < high) {
        mid = (low + high) / 2;
        mergeSort(a, low, mid);
        mergeSort(a, mid + 1, high);
        merge(a, low, mid, high);
    }
}

int main() {
    int a[100], n, i;

    printf("Merge Sort\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    mergeSort(a, 0, n - 1);

    printf("Sorted elements are:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}

