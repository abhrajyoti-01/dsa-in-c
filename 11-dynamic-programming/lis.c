/*
Longest Increasing Subsequence (LIS)
------------------------------------
What this program solves:
Finds the length of the longest strictly increasing subsequence
(elements need not be contiguous) and reconstructs one such
subsequence. Shows both O(n^2) DP and O(n log n) binary search.

Real life example:
Stock price trend analysis: pick the longest streak of days where
each selected day's price is higher than the previous picked day -
skipping volatile days is allowed since order matters, not adjacency.

How the process works (O(n^2) DP):
1. dp[i] = length of the LIS ENDING at index i (dp[i] = 1).
2. For each i, scan j < i: if arr[j] < arr[i],
   dp[i] = max(dp[i], dp[j] + 1).
3. Answer = max(dp[]); backtrack via parent[] to rebuild.

O(n log n) method: tails[k] = smallest possible tail of an
increasing subsequence of length k+1; place each element with
binary search (lower_bound).

DIAGRAM (LIS of [10 9 2 5 3 7]:
  tails: [10] -> [9] -> [2] -> [2 5] -> [2 3] -> [2 3 7]
  length = 3: subsequence 2, 3, 7
  binary search replaces the first tail >= x -> O(n log n).

Pseudo code:
START
FOR i = 0..n-1: dp[i] = 1; parent[i] = -1
FOR i = 1..n-1
    FOR j = 0..i-1
        IF arr[j] < arr[i] AND dp[j]+1 > dp[i]:
            dp[i] = dp[j]+1; parent[i] = j
best = MAX(dp); backtrack parent chain
END

Sample input and dry run:
[10, 22, 9, 33, 21, 50, 41, 60]
dp = [1, 2, 1, 3, 2, 4, 4, 5] -> LIS length 5
one LIS: 10 22 33 50 60

Main logic to understand:
dp[i] means "best increasing chain ending HERE" - the same
optimal-substructure pattern as LCS. O(n^2) version is simple and
reconstructs easily; the tails array version achieves O(n log n).
*/

#include <stdio.h>

#define MAX_N 200

int main() {
    int arr[MAX_N];
    int dp[MAX_N];
    int parent[MAX_N];
    int tails[MAX_N];
    int tailsIdx[MAX_N];
    int n;

    printf("Longest Increasing Subsequence\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        dp[i] = 1;
        parent[i] = -1;
    }

    /* STEP: O(n^2) DP - best chain ending at each index */
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
    }

    int best = 0;
    int bestIdx = 0;
    for (int i = 0; i < n; i++) {
        if (dp[i] > best) {
            best = dp[i];
            bestIdx = i;
        }
    }

    printf("\nDP lengths: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", dp[i]);
    }
    printf("\n");

    /* STEP: reconstruct one LIS by walking parent links */
    int lis[MAX_N];
    int len = 0;
    for (int i = bestIdx; i != -1; i = parent[i]) {
        lis[len] = arr[i];
        len = len + 1;
    }

    printf("LIS length: %d\n", best);
    printf("One LIS: ");
    for (int i = len - 1; i >= 0; i--) {
        printf("%d ", lis[i]);
    }
    printf("\n");

    /* STEP: O(n log n) tails-array method for verification */
    int k = 0;
    for (int i = 0; i < n; i++) {
        int lo = 0;
        int hi = k;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (arr[tailsIdx[mid]] < arr[i]) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        tailsIdx[lo] = i;
        if (lo == k) {
            k = k + 1;
        }
        tails[i] = lo + 1;
    }
    (void)tails;

    printf("O(n log n) method agrees: %d\n", k);

    return 0;
}
