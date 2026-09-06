/*
Bucket Sort
-----------
What this program solves:
Distributes elements into a number of buckets, sorts each bucket
individually (insertion sort here), then concatenates the buckets.
Works best when input is uniformly distributed.

Real life example:
Sorting mail by first letter: drop each letter into one of 26
pigeonholes, then sort each small pigeonhole separately - much
faster than sorting the entire pile at once.

DIAGRAM (values 0.12 0.45 0.33 0.78 0.51 into 5 buckets):
  bucket i = floor(value * 5):
  [0.12] [0.33] [0.45] [0.51] [0.78]
     0      1      2      3      4
  sort each small bucket (insertion), then concatenate.

How the process works:
1. Create n buckets; map each value v to bucket (v*n)/max.
2. Sort each bucket with insertion sort (small buckets are fast).
3. Concatenate buckets 0..n-1 into the result.

Pseudo code:
START
max = MAX(arr)
FOR each v: bucket[(v * n) / (max + 1)].add(v)
FOR each bucket: insertionSort(bucket)
CONCAT all buckets in order
END

Sample input and dry run:
[42, 10, 25, 63, 18, 90, 5] with 7 buckets
bucket sizes are proportional to value/91
small values land in early buckets, big in late ones;
sorting inside buckets then concatenating gives full order.

Main logic to understand:
Expected time O(n + n^2/k + k) - with k = n buckets and uniform
data it is O(n) on average, O(n^2) worst case when everything
lands in one bucket. Stable if the inner sort is stable.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUCKETS 100

void printArray(const int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

/*
 * insertionSortSmall - Sorts a small bucket in place.
 */
void insertionSortSmall(int arr[], int n) {
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

/*
 * bucketSort - Distributes into buckets, sorts each, concatenates.
 */
void bucketSort(int arr[], int n, int out[]) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    int buckets[MAX_BUCKETS][200];
    int counts[MAX_BUCKETS] = {0};
    int numBuckets = (n < MAX_BUCKETS) ? n : MAX_BUCKETS;

    for (int i = 0; i < n; i++) {
        int b = (int)((long long)arr[i] * numBuckets / ((long long)max + 1));
        if (b >= numBuckets) {
            b = numBuckets - 1;
        }
        buckets[b][counts[b]] = arr[i];
        counts[b] = counts[b] + 1;
    }

    int idx = 0;
    for (int b = 0; b < numBuckets; b++) {
        if (counts[b] > 1) {
            insertionSortSmall(buckets[b], counts[b]);
        }
        for (int j = 0; j < counts[b]; j++) {
            out[idx] = buckets[b][j];
            idx = idx + 1;
        }
    }
}

int main() {
    int arr[200];
    int out[200];
    int n;

    printf("Bucket Sort\n\n");

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

    bucketSort(arr, n, out);

    printf("Sorted:   ");
    printArray(out, n);

    return 0;
}
