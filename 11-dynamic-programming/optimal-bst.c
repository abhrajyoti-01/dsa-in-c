/*
Optimal Binary Search Tree
--------------------------
What this program solves:
Given sorted keys and how frequently each key is searched, this program builds
the binary search tree with minimum average search cost.

Real life example:
Imagine the search box of an online dictionary or library system. Some words are
searched far more often than others. It makes sense to keep those frequently searched
items near the top of the tree, so the average search becomes faster. This program
finds the best arrangement.

DIAGRAM (keys a(3) b(1) c(2): put frequent keys shallow):
        b?  no - weight-aware split:
        a(3)             cost = depth * frequency summed
       /   \
     (b)   (c)
  dp[i][j] = min root r: dp[i][r-1] + dp[r+1][j] + sum(freq i..j)

How the process works:
1. Consider every possible range of keys.
2. For a range from i to j, try every key k as the root.
3. Keys before k form the left subtree and keys after k form the right subtree.
4. Total cost = cost of left subtree + cost of right subtree + frequency sum of the range.
5. Keep the minimum cost among all root choices.

Pseudo code:
START
Read number of keys and their frequencies
Initialize cost of single key tree
For each range length from 2 to n
    For each key range i to j
        Compute sum of frequencies in the range
        Try each key k as root
            Cost = left subtree + right subtree + sum of frequencies
            Keep the minimum cost
Print minimum search cost
END

Sample input and dry run:
Suppose keys are 10, 20, 30 with frequencies 3, 10, 2
Key 20 is searched the most, so it is a strong candidate for the root.
If 20 is chosen as root, key 10 goes left and key 30 goes right.
That keeps the frequently used key near the top and reduces average search cost.
The DP table checks all such root choices and stores the minimum one.

Main logic to understand:
Frequently searched keys should appear at smaller depth, and dynamic programming
helps evaluate all meaningful tree structures efficiently.
*/
#include <stdio.h>

#define MAX 20
#define INF 999999

int main() {
    int freq[MAX], cost[MAX][MAX], sum[MAX][MAX];
    int n, i, j, k, length, minCost;

    printf("Optimal Binary Search Tree\n");
    printf("Enter the number of keys: ");
    scanf("%d", &n);

    printf("Enter the frequency of each key:\n");
    for (i = 1; i <= n; i++) {
        scanf("%d", &freq[i]);
    }

    for (i = 1; i <= n; i++) {
        cost[i][i] = freq[i];
        sum[i][i] = freq[i];
    }

    for (length = 2; length <= n; length++) {
        for (i = 1; i <= n - length + 1; i++) {
            j = i + length - 1;
            sum[i][j] = sum[i][j - 1] + freq[j];
            cost[i][j] = INF;

            for (k = i; k <= j; k++) {
                if (k == i) {
                    minCost = cost[k + 1][j];
                } else if (k == j) {
                    minCost = cost[i][k - 1];
                } else {
                    minCost = cost[i][k - 1] + cost[k + 1][j];
                }

                if (minCost + sum[i][j] < cost[i][j]) {
                    cost[i][j] = minCost + sum[i][j];
                }
            }
        }
    }

    printf("Minimum search cost of optimal BST: %d\n", cost[1][n]);
    return 0;
}

