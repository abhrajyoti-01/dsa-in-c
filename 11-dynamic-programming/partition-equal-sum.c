/*
Partition Problem (Equal-Sum Subset)
------------------------------------
What this program solves:
Decides whether the multiset of given numbers can be split into
two subsets with EQUAL sums - equivalently, whether some subset
sums to total/2 - and prints one such subset.

Real life example:
Splitting an inheritance of indivisible items (each with a value)
between two heirs as fairly as possible, or dividing work tasks
into two batches with equal effort.

DIAGRAM (dp over reachable sums, [1 5 11 5] target 11):
  start {0}
  +1  -> {0,1}
  +5  -> {0,1,5,6}
  +11 -> {0,1,5,6,11,12,16,17}   11 reached -> YES
  dp[s] = true if some subset sums to s.

How the process works:
1. If total is odd -> impossible immediately.
2. subsetSum[v] = true if some subset sums to exactly v.
3. For each number x, sweep v from target DOWN to x:
   subsetSum[v] |= subsetSum[v - x]
   (downward sweep ensures each item is used at most once).
4. parent[v] records which item achieved v for reconstruction.

Pseudo code:
START
total = SUM(arr); IF odd: impossible
target = total / 2; subsetSum[0] = true
FOR each x in arr
    FOR v = target..x (downward)
        IF subsetSum[v-x] AND NOT subsetSum[v]:
            subsetSum[v] = true; used[v] = x
END

Sample input and dry run:
[3, 1, 1, 2, 2, 1] -> total 10, target 5
subset sums reachable include 5 -> YES
one half: {3, 2} sums to 5; other {1, 1, 2, 1} sums to 5

Main logic to understand:
Subset-sum DP: booleans over all achievable sums 0..target. The
downward loop is the 0/1 knapsack trick preventing item reuse.
O(n * target) time, O(target) space.
*/

#include <stdio.h>

#define MAX_ITEMS 100
#define MAX_SUM 5000

int main() {
    int arr[MAX_ITEMS];
    int n;
    int total = 0;

    printf("Partition Problem - Equal-Sum Subset\n\n");

    printf("How many numbers (1-%d)? ", MAX_ITEMS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d positive integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        total = total + arr[i];
    }

    printf("\nTotal sum: %d\n", total);

    if (total % 2 != 0) {
        printf("Odd total - equal partition is IMPOSSIBLE\n");
        return 0;
    }

    int target = total / 2;
    printf("Target subset sum: %d\n\n", target);

    static int subsetSum[MAX_SUM + 1];
    static int used[MAX_SUM + 1];

    for (int v = 0; v <= target; v++) {
        subsetSum[v] = 0;
        used[v] = -1;
    }
    subsetSum[0] = 1;

    /* STEP: fill subset sums, sweeping downward per item */
    for (int i = 0; i < n; i++) {
        int x = arr[i];
        for (int v = target; v >= x; v--) {
            if (subsetSum[v - x] && !subsetSum[v]) {
                subsetSum[v] = 1;
                used[v] = x;
            }
        }
    }

    if (!subsetSum[target]) {
        printf("No subset sums to %d - equal partition is IMPOSSIBLE\n", target);
        return 0;
    }

    /* STEP: reconstruct one half by walking the used[] chain */
    printf("Partition EXISTS\n");

    int taken[MAX_ITEMS] = {0};
    int half1[64];
    int h1 = 0;
    int v = target;

    while (v > 0) {
        int x = used[v];
        for (int i = 0; i < n; i++) {
            if (!taken[i] && arr[i] == x) {
                taken[i] = 1;
                break;
            }
        }
        half1[h1] = x;
        h1 = h1 + 1;
        v = v - x;
    }

    printf("Subset 1: ");
    int s1 = 0;
    for (int i = 0; i < h1; i++) {
        printf("%d ", half1[i]);
        s1 = s1 + half1[i];
    }
    printf("(sum %d)\n", s1);

    printf("Subset 2: ");
    for (int i = 0; i < n; i++) {
        if (!taken[i]) {
            printf("%d ", arr[i]);
        }
    }
    printf("(sum %d)\n", total - s1);

    return 0;
}
