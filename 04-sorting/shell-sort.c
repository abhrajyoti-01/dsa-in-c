/*
Shell Sort
----------
What this program solves:
Generalizes insertion sort by allowing exchanges of elements that
are far apart, shrinking the gap until it becomes 1 - at which
point it is plain insertion sort but on nearly-sorted data.

Real life example:
Organizing books on a long shelf: first move books roughly into the
correct HALF of the shelf (big jumps), then quarter, then fine-tune
with adjacent swaps. The early big jumps make the final pass cheap.

DIAGRAM (gapped insertion, n=8, gap 4 -> 2 -> 1):
  gap 4: compare arr[i] with arr[i-4] for i=4..7 (far-apart fixes)
  gap 2: subsequences [a0 a2 a4 a6] and [a1 a3 a5 a7] sorted
  gap 1: plain insertion sort - but now almost sorted, so cheap
  big jumps early move elements most of the way home.

How the process works:
1. Start with gap = n/2.
2. Perform gapped insertion sort: every element at index
   i, i+gap, i+2*gap ... forms a virtual subsequence sorted by
   insertion.
3. Halve the gap; repeat until gap = 1 (plain insertion sort).

Pseudo code:
START
FOR gap = n/2; gap > 0; gap = gap/2
    FOR i = gap..n-1
        temp = arr[i]
        j = i
        WHILE j >= gap AND arr[j-gap] > temp
            arr[j] = arr[j-gap]
            j = j - gap
        arr[j] = temp
END

Sample input and dry run:
[12, 34, 54, 2, 3] gap=2 -> [12, 3, 54, 2, 34]
gap=1 -> plain insertion -> [2, 3, 12, 34, 54]

Main logic to understand:
Each pass leaves the array "gap-sorted"; later smaller-gap passes
rearrange little because earlier passes already moved elements
close to home. Time ~O(n^1.5) with this gap sequence; O(1) space;
unstable; in-place.
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
 * shellSort - Gapped insertion sort with halving gaps.
 */
void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap = gap / 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j = j - gap;
            }
            arr[j] = temp;
        }
    }
}

int main() {
    int arr[100];
    int n;

    printf("Shell Sort\n\n");

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

    shellSort(arr, n);

    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}
