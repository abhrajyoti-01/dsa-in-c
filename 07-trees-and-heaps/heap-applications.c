/*
Heap Applications Ladder (Basic -> Hard)
----------------------------------------
Real life example:
Hospital triage and OS schedulers always serve the most urgent
item (priority queue); merging ropes by least cost is file-merge
planning; running median is live analytics dashboards.

DIAGRAM (running median with two heaps):
  max-heap (lower half)      min-heap (upper half)
        12                        15
       /  \                      /  \
      8    10                  18    20
  sizes equal -> median = (top(lower) + top(upper)) / 2

LADDER (easy -> hard inside this file):
  1. K largest elements        (min-heap of size k)
  2. Connect ropes / minimum cost of merging  (min-heap greedy)
  3. Running median of a stream (two-heap technique - the master one)

BEGINNER EXPLANATION:
A MIN-heap keeps the SMALLEST at the top. Applications:
- K largest: keep a min-heap of size k; a new element bigger than
  the top replaces it. Top = k-th largest at the end. O(n log k).
- Connect ropes: joining the two SHORTEST ropes first minimizes
  total cost (Huffman-flavored greedy). A min-heap gives the two
  smallest instantly; push the merged rope back.
- Running median: max-heap for the LOWER half, min-heap for the
  UPPER half. Sizes balanced so tops straddle the median. Median
  after every element in O(log n) instead of re-sorting O(n log n).

SUPER SIMPLE EXAMPLE (connect ropes):
Ropes 4, 3, 2, 6.
merge 2+3 = 5 (cost 5). ropes: 4, 5, 6
merge 4+5 = 9 (cost 9). ropes: 9, 6
merge 9+6 = 15 (cost 15). Total = 5+9+15 = 29.
Any other order costs more (verify: 2+4=6, then 3+6=9, 9+6=15 ->
30). Greedy smallest-first wins.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- k >= n: all elements are "k largest" - the heap just holds all.
- Running median with an EVEN count: median = average of the two
  middle values; with ODD count it is the middle. The two-heap
  sizes differ by at most 1 - handle both parities.
- Single rope: no merging needed, cost 0.
- Stream ends: the lower heap's top (or the average of tops) is
  the final median.

DRY RUN (running median of 12, 4, 7):
12 -> lower(maxH) [12]. median 12.
4 -> smaller than 12 -> maxH [4,12]? maxH keeps LOWER half:
     actually 4 < 12 goes to maxH; rebalance sizes 2 vs 0 ->
     move max to minH. maxH [4], minH [12]. median (4+12)/2 = 8.
7 -> 7 >= 4 -> minH [7,12]. sizes 1 vs 2 -> move min to maxH:
     maxH [7,4], minH [12]. median 7. (odd count -> maxH top)
Sequence of medians: 12, 8, 7.

TIPS:
- The two-heap median trick appears in databases (percentiles on
  streams) and scheduling; it is THE heap-pair interview problem.
- Connect ropes = Huffman's cost structure - compare with
  huffman_coding.c in folder 09 (same greedy, different data).
- "K largest" via min-heap (not max-heap!) is the counterintuitive
  point interviewers probe - the heap holds the CANDIDATES.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP 200

/* ---------- Generic min-heap ---------- */
int minHeap[MAX_HEAP];
int minSize = 0;

void minPush(int v) {
    if (minSize >= MAX_HEAP) {
        printf("Heap full\n");
        return;
    }
    int i = minSize;
    minHeap[i] = v;
    minSize = minSize + 1;
    while (i > 0 && minHeap[(i - 1) / 2] > minHeap[i]) {
        int t = minHeap[(i - 1) / 2];
        minHeap[(i - 1) / 2] = minHeap[i];
        minHeap[i] = t;
        i = (i - 1) / 2;
    }
}

int minPop(void) {
    if (minSize == 0) {
        return -1000000000;
    }
    int top = minHeap[0];
    minSize = minSize - 1;
    minHeap[0] = minHeap[minSize];
    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < minSize && minHeap[l] < minHeap[smallest]) {
            smallest = l;
        }
        if (r < minSize && minHeap[r] < minHeap[smallest]) {
            smallest = r;
        }
        if (smallest == i) {
            break;
        }
        int t = minHeap[i];
        minHeap[i] = minHeap[smallest];
        minHeap[smallest] = t;
        i = smallest;
    }
    return top;
}

/* ---------- Max-heap for median ---------- */
int maxHeap[MAX_HEAP];
int maxSize = 0;

void maxPush(int v) {
    int i = maxSize;
    maxHeap[i] = v;
    maxSize = maxSize + 1;
    while (i > 0 && maxHeap[(i - 1) / 2] < maxHeap[i]) {
        int t = maxHeap[(i - 1) / 2];
        maxHeap[(i - 1) / 2] = maxHeap[i];
        maxHeap[i] = t;
        i = (i - 1) / 2;
    }
}

int maxPop(void) {
    int top = maxHeap[0];
    maxSize = maxSize - 1;
    maxHeap[0] = maxHeap[maxSize];
    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int largest = i;
        if (l < maxSize && maxHeap[l] > maxHeap[largest]) {
            largest = l;
        }
        if (r < maxSize && maxHeap[r] > maxHeap[largest]) {
            largest = r;
        }
        if (largest == i) {
            break;
        }
        int t = maxHeap[i];
        maxHeap[i] = maxHeap[largest];
        maxHeap[largest] = t;
        i = largest;
    }
    return top;
}

/*
 * balanceHeaps - Keep maxSize == minSize or maxSize == minSize+1.
 */
void balanceHeaps(void) {
    if (maxSize > minSize + 1) {
        minPush(maxPop());
    } else if (minSize > maxSize) {
        maxPush(minPop());
    }
}

void printMedian(int count) {
    double median;
    if (count % 2 == 1) {
        median = (double)maxHeap[0];
    } else {
        median = ((double)maxHeap[0] + (double)minHeap[0]) / 2.0;
    }
    printf("After %d elements: median = %.1f\n", count, median);
}

int main() {
    int choice;
    int n;

    printf("Heap Applications Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. K largest elements\n");
        printf("2. Connect ropes minimum cost\n");
        printf("3. Running median of a stream\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int arr[100];
            int k;
            printf("How many numbers (1-100)? ");
            scanf("%d", &n);
            printf("Enter %d numbers: ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            printf("k: ");
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("k out of range\n");
                continue;
            }
            minSize = 0;
            for (int i = 0; i < k; i++) {
                minPush(arr[i]);
            }
            for (int i = k; i < n; i++) {
                if (arr[i] > minHeap[0]) {
                    minPop();
                    minPush(arr[i]);
                }
            }
            printf("%d largest:", k);
            while (minSize > 0) {
                printf(" %d", minPop());
            }
            printf("\n");
        } else if (choice == 2) {
            minSize = 0;
            printf("How many ropes (1-100)? ");
            scanf("%d", &n);
            printf("Enter %d rope lengths: ", n);
            for (int i = 0; i < n; i++) {
                int len;
                scanf("%d", &len);
                minPush(len);
            }
            long long totalCost = 0;
            if (n == 1) {
                printf("Single rope - cost 0\n");
            } else {
                while (minSize > 1) {
                    int a = minPop();
                    int b = minPop();
                    int merged = a + b;
                    totalCost = totalCost + merged;
                    minPush(merged);
                    printf("  connect %d + %d -> %d (cost so far %lld)\n",
                           a, b, merged, totalCost);
                }
                printf("Minimum total cost: %lld\n", totalCost);
            }
        } else if (choice == 3) {
            minSize = 0;
            maxSize = 0;
            printf("How many stream values (1-100)? ");
            scanf("%d", &n);
            printf("Enter %d values one by one:\n", n);
            for (int i = 0; i < n; i++) {
                int v;
                scanf("%d", &v);
                if (maxSize == 0 || v <= maxHeap[0]) {
                    maxPush(v);
                } else {
                    minPush(v);
                }
                balanceHeaps();
                printMedian(i + 1);
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
