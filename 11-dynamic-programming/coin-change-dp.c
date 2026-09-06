/*
 PROBLEM DEFINITION:
 --------------------
 Given an amount and coin denominations, find the MINIMUM number of coins
 that sum exactly to the amount - for ARBITRARY denominations, guaranteed
 correct. Dynamic Programming solves this by building the answer bottom-up:

    minCoins[x] = 1 + min over all coins c of ( minCoins[x - c] )

 i.e., the best way to make amount x is: take some coin c (the last one
 used) plus the best solution for the leftover x - c.

 This program ALSO demonstrates the closely related "count ways" variant:
 how many distinct orderings of coins sum to the amount (like combinations
 of coins in a jar adding to a total).

 REAL-LIFE EXAMPLE - Parking Meter & Vending Machine Change Maker:
 -------------------------------------------------------------------
 A machine's firmware must return exact change using the coins it ACTUALLY
 has - denominations may be unusual (e.g., {1, 3, 4} tokens). A naive
 biggest-first strategy gives 4+1+1 (3 coins) for 6 tokens when 3+3 (2
 coins) exists. The DP table method considers every possible "last coin"
 for every sub-amount, so it never falls into that trap. Currency exchangers
 and arcade token systems face exactly this problem.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of filling a ladder where each rung stores the best answer for
 that amount, and each rung is built from smaller rungs:

 1. CREATE THE DP TABLE:
    - dp[0] = 0        (zero coins make amount 0)
    - dp[x] = INFINITY for all x > 0  (not solved yet)

 2. FILL BOTTOM-UP (for every amount from 1 to target):
    - For each coin c:
        If c <= x and dp[x - c] + 1 < dp[x]:
            dp[x] = dp[x - c] + 1        (taking coin c is better)
    - Equivalent recurrence: dp[x] = 1 + min(dp[x - c]) over valid c

 3. RESULT:
    - dp[amount] is the minimum coin count.
    - If dp[amount] is still INFINITY, exact change is impossible.

 4. (BONUS) COUNT WAYS VARIANT:
    - ways[0] = 1
    - For each coin c: for x from c to amount:
        ways[x] += ways[x - c]
    - This counts combinations (order-independent).

 PSEUDO CODE:
 ------------
 START
   Function MinCoins(coins[], n, amount):
     dp[0] = 0
     For x = 1 to amount:
         dp[x] = INFINITY
     For x = 1 to amount:
         For each coin c in coins:
             If c <= x AND dp[x - c] + 1 < dp[x]:
                 dp[x] = dp[x - c] + 1
                 parent[x] = c
     If dp[amount] == INFINITY: Return IMPOSSIBLE
     Return dp[amount]

   Function CountWays(coins[], n, amount):
     ways[0] = 1, ways[x] = 0 for x > 0
     For each coin c in coins:
         For x = c to amount:
             ways[x] = ways[x] + ways[x - c]
     Return ways[amount]
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Denominations: {1, 3, 4}, Amount = 6

 Filling dp[] step by step (INF = infinity):

 x=1: coin 1 -> dp[0]+1=1. dp[1]=1        (used coin 1)
 x=2: coin 1 -> dp[1]+1=2. dp[2]=2        (used coin 1)
 x=3: coin 1 -> dp[2]+1=3; coin 3 -> dp[0]+1=1. dp[3]=1  (used coin 3)
 x=4: coin 1 -> dp[3]+1=2; coin 3 -> dp[1]+1=2;
      coin 4 -> dp[0]+1=1. dp[4]=1        (used coin 4)
 x=5: coin 1 -> dp[4]+1=2. dp[5]=2        (used coin 1)
 x=6: coin 1 -> dp[5]+1=3;
      coin 3 -> dp[3]+1=2;                (best!)
      coin 4 -> dp[2]+1=3.
      dp[6]=2                             (used coin 3)

 dp table: [0, 1, 2, 1, 1, 2, 2]

 Answer: dp[6] = 2 coins.
 Reconstruct via parent[]: 6 used coin 3 -> leftover 3 used coin 3 -> done.
 Coins: 3 + 3. (Greedy would have wrongly said 3 coins!)

 Count ways for amount 6 with {1, 3, 4}:
   1+1+1+1+1+1, 1+1+1+3, 3+3, 1+1+4  -> 4 ways

 TIME COMPLEXITY:
 - Min Coins:  O(amount x n)   (each cell checks all n coins)
 - Count Ways: O(amount x n)

 SPACE COMPLEXITY: O(amount) for the 1-D dp arrays

 COMMON USE CASES:
 - Vending machines and parking meters (arbitrary coin inventory)
 - Making up any target from arbitrary "pieces" (tokens, stamps, sticks)
 - Foundation for unbounded knapsack-style DP problems
DIAGRAM (min coins for 6 with coins {1,3,4}):
  dp[0]=0
  dp[1]=1 (1)   dp[2]=2   dp[3]=1 (3)
  dp[4]=1 (4)   dp[5]=2   dp[6]= min(dp[5], dp[3], dp[2]) + 1 = 2
  answer: 3 + 3 -> two coins (greedy would take 4+1+1 = 3 coins).

*/

#include <stdio.h>
#include <stdlib.h>

#define INF 999999
#define MAX_AMOUNT 1000
#define MAX_COINS 50

/*
 * minCoins - Fills the dp table and returns minimum coin count.
 * Returns -1 if exact change is impossible.
 *
 * parent[x] records WHICH coin was last used at amount x, letting us
 * reconstruct the actual coin list afterwards.
 *
 * Analogy: Building a ladder rung by rung; each rung asks
 * "what is the cheapest final coin that gets me here?"
 */
int minCoins(int coins[], int n, int amount, int parent[]) {
    int dp[MAX_AMOUNT];

    dp[0] = 0;
    parent[0] = 0;

    for (int x = 1; x <= amount; x++) {
        dp[x] = INF;
        parent[x] = 0;
    }

    printf("  Building dp table for amount %d:\n\n", amount);
    printf("  dp[0] = 0 (base case: no coins needed)\n");

    for (int x = 1; x <= amount; x++) {
        for (int c = 0; c < n; c++) {
            if (coins[c] <= x && dp[x - coins[c]] + 1 < dp[x]) {
                dp[x] = dp[x - coins[c]] + 1;
                parent[x] = coins[c];
            }
        }
        if (dp[x] != INF) {
            printf("  dp[%d] = %d (last coin: %d)\n", x, dp[x], parent[x]);
        } else {
            printf("  dp[%d] = IMPOSSIBLE\n", x);
        }
    }

    if (dp[amount] == INF) {
        return -1;
    }
    return dp[amount];
}

/*
 * reconstructCoins - Walks the parent[] chain to print the coins used.
 */
void reconstructCoins(int parent[], int amount) {
    printf("\n  Coins used: ");
    int x = amount;
    while (x > 0) {
        printf("%d ", parent[x]);
        x -= parent[x];
    }
    printf("\n");
}

/*
 * countWays - Counts the number of ways to form the amount.
 * Iterates coins in the OUTER loop so combinations are counted
 * (1+3 and 3+1 count as ONE way), not permutations.
 */
int countWays(int coins[], int n, int amount) {
    int ways[MAX_AMOUNT];

    ways[0] = 1;
    for (int x = 1; x <= amount; x++) {
        ways[x] = 0;
    }

    for (int c = 0; c < n; c++) {
        for (int x = coins[c]; x <= amount; x++) {
            ways[x] += ways[x - coins[c]];
        }
    }

    return ways[amount];
}

int main() {
    int coins[MAX_COINS];
    int n;
    int amount;

    printf("Coin Change (Dynamic Programming) - Min Coins\n\n");

    printf("How many coin denominations (1-%d)? ", MAX_COINS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_COINS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d denominations separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
    }

    printf("Enter the amount: ");
    scanf("%d", &amount);

    if (amount < 0 || amount >= MAX_AMOUNT) {
        printf("Invalid amount\n");
        return 1;
    }

    printf("\nAmount = %d, coins: ", amount);
    for (int i = 0; i < n; i++) {
        printf("%d ", coins[i]);
    }
    printf("\n\n");

    int parent[MAX_AMOUNT];
    int result = minCoins(coins, n, amount, parent);

    if (result != -1) {
        printf("\nMinimum coins required: %d\n", result);
        reconstructCoins(parent, amount);
    } else {
        printf("\nExact change impossible with given coins\n");
    }

    /* VARIANT: count the number of distinct ways to make the amount */
    int ways = countWays(coins, n, amount);
    printf("\nWays to make %d with the given coins: %d\n", amount, ways);

    return 0;
}
