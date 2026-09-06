/*
 PROBLEM DEFINITION:
 --------------------
 Given an amount and a set of coin denominations, find the MINIMUM number
 of coins that sum exactly to the amount. The GREEDY version always picks
 the LARGEST coin that still fits, repeatedly, until the amount is paid.

 IMPORTANT CAVEAT: Greedy coin change is optimal only for "canonical"
 coin systems (like Indian/US currency: 1, 2, 5, 10, 20, 50, 100...).
 For arbitrary denominations (e.g., {1, 3, 4}), greedy can FAIL, and
 Dynamic Programming is required (see coin_change_dp.c).

 REAL-LIFE EXAMPLE - Cashier Giving Change at a Shop:
 -----------------------------------------------------
 A cashier handing back Rs. 93 in change does not count out 93 one-rupee
 coins. Instinctively: one Rs.50 note, then Rs.20+20, then Rs.2+1 -
 largest denomination first. That is the greedy strategy. It works because
 real currency systems are designed so bigger denominations divide evenly
 enough into smaller combinations. Vending machines and cash registers
 use the same rule.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of paying a bill using the biggest notes first so the wallet
 empties as slowly as possible:

 1. SORT denominations in DESCENDING order (largest coin first).

 2. FOR each denomination d (from largest to smallest):
      While the remaining amount >= d:
          Take one coin of value d
          Subtract d from the remaining amount
          Record the coin

 3. IF remaining amount becomes 0 -> success, output the coins taken.
    ELSE (no coin fits but amount > 0) -> greedy fails for this system.

 WHY IT CAN FAIL (the classic trap):
    Denominations {1, 3, 4}, amount = 6:
      Greedy:  4 + 1 + 1  -> 3 coins
      Optimal: 3 + 3      -> 2 coins
    The greedy choice (biggest first) blocked the better combination.
    That is why DP is needed for arbitrary coin systems.

 PSEUDO CODE:
 ------------
 START
   Function GreedyCoinChange(denominations[], n, amount):
     Sort denominations in descending order
     remaining = amount
     coinsUsed = empty list

     For i = 0 to n-1:
         While remaining >= denominations[i]:
             remaining = remaining - denominations[i]
             Add denominations[i] to coinsUsed

     If remaining == 0:
         Return coinsUsed
     Else:
         Return FAILURE (exact change impossible with greedy)
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Denominations (Indian currency): {1, 2, 5, 10, 20, 50, 100}
 Amount: 93

 Step 1: remaining = 93. Coin 100? 93 < 100, skip.
 Step 2: Coin 50? 93 >= 50 -> take 50. remaining = 43.
 Step 3: Coin 20? 43 >= 20 -> take 20. remaining = 23.
 Step 4: Coin 20? 23 >= 20 -> take 20. remaining = 3.
 Step 5: Coin 20? 3 < 20, skip. Coin 10? skip. Coin 5? skip.
 Step 6: Coin 2? 3 >= 2 -> take 2. remaining = 1.
 Step 7: Coin 2? 1 < 2, skip. Coin 1? 1 >= 1 -> take 1. remaining = 0.

 Result: {50, 20, 20, 2, 1} -> 5 coins for Rs. 93. Optimal!

 Counter-example with {4, 3, 1}, amount = 6:
   Greedy: 4 + 1 + 1 = 3 coins (WRONG, suboptimal)
   DP would find: 3 + 3 = 2 coins

 TIME COMPLEXITY:
 - O(N) per query after sorting (N = number of denominations),
   since each denomination is considered once and amount decreases.

 SPACE COMPLEXITY: O(1) extra (excluding output list)

 COMMON USE CASES:
 - Cash registers and vending machines (canonical currency)
 - Task scheduling by "largest chunk first" heuristics
 - Teaching example contrasting Greedy vs Dynamic Programming
DIAGRAM (amount 63, coins 25 10 5 1 - greedy):
  63 -> 2 x 25 (13 left) -> 1 x 10 (3 left) -> 3 x 1
  5 coins total.  Greedy = take the biggest coin that still fits.

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_DENOMS 50

/*
 * compareDesc - Comparator for qsort: sorts integers in DESCENDING order.
 * Greedy needs the largest denomination available first.
 */
int compareDesc(const void* a, const void* b) {
    return (*(int*)b) - (*(int*)a);
}

/*
 * greedyCoinChange - Attempts to make change using largest-coin-first.
 *
 * coins[]     : denominations (sorted descending inside)
 * n           : number of denominations
 * amount      : target sum
 * used[]      : output - coins actually taken
 * usedCount   : output - how many coins were taken
 *
 * Returns remaining amount: 0 means SUCCESS, > 0 means greedy failed.
 *
 * Analogy: A cashier paying with the biggest notes first.
 */
int greedyCoinChange(int coins[], int n, int amount, int used[], int* usedCount) {
    qsort(coins, n, sizeof(int), compareDesc);

    int remaining = amount;
    *usedCount = 0;

    for (int i = 0; i < n; i++) {
        while (remaining >= coins[i]) {
            remaining -= coins[i];
            used[(*usedCount)++] = coins[i];
        }
    }

    return remaining;
}

/*
 * printResult - Displays the coins used and the count.
 */
void printResult(int used[], int usedCount) {
    printf("Coins used: ");
    for (int i = 0; i < usedCount; i++) {
        printf("%d ", used[i]);
    }
    printf("\nTotal coins: %d\n", usedCount);
}

int main() {
    int coins[MAX_DENOMS];
    int n;
    int amount;

    printf("Coin Change (Greedy) - Minimum Coins\n\n");

    printf("How many denominations (1-%d)? ", MAX_DENOMS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_DENOMS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d denominations separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &coins[i]);
    }

    printf("Enter the amount: ");
    scanf("%d", &amount);

    if (amount < 0) {
        printf("Invalid amount\n");
        return 1;
    }

    printf("\nDenominations: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", coins[i]);
    }
    printf("\nAmount = %d\n", amount);

    int used[MAX_DENOMS];
    int usedCount = 0;

    int remaining = greedyCoinChange(coins, n, amount, used, &usedCount);

    if (remaining == 0) {
        printResult(used, usedCount);
    } else {
        printf("Greedy FAILED, %d left unpaid\n", remaining);
    }

    return 0;
}
