/*
Rod Cutting (Dynamic Programming)
---------------------------------
What this program solves:
Given a rod of length n and a price for each piece length 1..n,
cut the rod to maximize total revenue. Shows the recurrence, the
full DP table, and the chosen cut positions.

Real life example:
A steel merchant with an 8-meter rod and a market price list:
1m = Rs 100, 2m = Rs 210, ... Selling whole is rarely optimal -
cutting into the right mix of lengths earns the most.

DIAGRAM (rod length 4, prices length:1..4 = 1 5 8 9):
  best(4) = max( p1+best(3), p2+best(2), p4 )
          = max(1+5, 5+5, 9) = 10  -> two pieces of length 2

How the process works:
1. dp[i] = max revenue for a rod of length i.
2. dp[i] = max over cut lengths p of (price[p] + dp[i - p]).
   The "no cut" case is included as p = i (price[i] + dp[0]).
3. Rebuild the cuts by walking back through the recorded best cut.

Pseudo code:
START
dp[0] = 0
FOR i = 1..n
    dp[i] = -INF
    FOR p = 1..i
        IF price[p] + dp[i-p] > dp[i]:
            dp[i] = price[p] + dp[i-p]; cut[i] = p
REVENUE = dp[n]; walk cut[] for the pieces
END

Sample input and dry run:
lengths 1..8 prices: 1,5,8,9,10,17,17,20
dp[1]=1 dp[2]=5 dp[3]=8 dp[4]=10 dp[5]=13 dp[6]=17 dp[7]=18 dp[8]=22
best for 8: 2 + 6 -> 5 + 17 = 22 (cuts at 2, 6)

Main logic to understand:
Optimal substructure: the best way to cut length i includes one
piece of length p plus the OPTIMAL way to cut i-p. Same pattern as
coin change (max version). O(n^2) time, O(n) space.
*/

#include <stdio.h>

#define MAX_LEN 100

int main() {
    int price[MAX_LEN + 1];
    int dp[MAX_LEN + 1];
    int cut[MAX_LEN + 1];
    int n;

    printf("Rod Cutting - Maximum Revenue\n\n");

    printf("Rod length (1-%d)? ", MAX_LEN);
    scanf("%d", &n);

    if (n < 1 || n > MAX_LEN) {
        printf("Invalid length\n");
        return 1;
    }

    printf("Enter price for piece lengths 1..%d separated by space:\n", n);
    for (int p = 1; p <= n; p++) {
        scanf("%d", &price[p]);
    }

    /* STEP: bottom-up DP over rod lengths */
    dp[0] = 0;
    cut[0] = 0;
    for (int i = 1; i <= n; i++) {
        dp[i] = -1;
        for (int p = 1; p <= i; p++) {
            if (price[p] + dp[i - p] > dp[i]) {
                dp[i] = price[p] + dp[i - p];
                cut[i] = p;
            }
        }
    }

    printf("\nDP table (length: best revenue, first cut):\n");
    for (int i = 1; i <= n; i++) {
        printf("  length %2d: revenue %3d, cut %d\n", i, dp[i], cut[i]);
    }

    printf("\nMaximum revenue for rod %d: %d\n", n, dp[n]);

    /* STEP: reconstruct the cut list */
    printf("Cuts: ");
    int remaining = n;
    while (remaining > 0) {
        printf("%d ", cut[remaining]);
        remaining = remaining - cut[remaining];
    }
    printf("\n");

    return 0;
}
