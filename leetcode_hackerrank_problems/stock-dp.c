/*
Stock DP - Buy/Sell, Cooldown, Fee, Two Transactions, k Transactions
(Intermediate -> Advanced)
-------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 121 - Best Time to Buy and Sell Stock
  LeetCode 122 - Stock II (unlimited transactions)
  LeetCode 123 - Stock III (at most two transactions)
  LeetCode 188 - Stock IV (at most k transactions)
  LeetCode 309 - Stock with Cooldown
  LeetCode 714 - Stock with Transaction Fee
  HackerRank "Stock Maximize" (unlimited transactions, same recurrence)

PROBLEM DEFINITION:
Six variants of one state machine. Track the best cash while HOLDING a
share and while FREE, and each variant adds one twist: unlimited
transactions, a cooldown day, a per-trade fee, or a cap of k trades.

REAL-LIFE EXAMPLE:
An algorithmic trading desk back-tests strategies on a price series:
the profit-maximising schedule under different broker rules (fees,
settlement cooldown, regulatory trade limits) is exactly this DP.

DIAGRAM (prices [1,3,2,8,4,9], unlimited trades, fee 0):
  hold = best cash while holding, free = best cash while flat
  i=0 p=1: free 0,        hold -1
  i=1 p=3: free 2,        hold max(-1, 0-3) = -1
  i=2 p=2: free 2,        hold max(-1, 2-2) = 0
  i=3 p=8: free 8,        hold max(0, 2-8) = 0
  i=4 p=4: free 8,        hold max(0, 8-4) = 4
  i=5 p=9: free 13,       hold max(4, 8-9) = 4
  Buy 1 sell 3 (+2), buy 2 sell 8 (+6), buy 4 sell 9 (+5) -> 13.

PSEUDO CODE:
  One transaction: track the minimum price seen; profit = max(profit,
      price - minPrice)
  Unlimited: hold = max(hold, free - price); free = max(free, hold + price)
  Cooldown: keep free, hold and freeYesterday;
      newHold = max(hold, free - price)
      newFree = max(free, hold + price)
      freeYesterday = free (the day's achieved free state is unusable
      tomorrow, which is exactly the cooldown)
  Fee: the same two-state loop with (hold + price - fee)
  At most k: hold[j] = max(hold[j], free[j] - price),
             free[j] = max(free[j], hold[j-1] + price) for j = k..1

SAMPLE INPUT & DRY RUN:
  6
  1 3 2 8 4 9
  2
  2
  Prices 1 3 2 8 4 9: one-transaction best 1 -> 9 = 8; unlimited best
  13 (diagram); with k = 2 transactions the best is 1->8 (+7) and
  4->9 (+5) = 12 (plus the 2->3 wait is not used by the optimum).

SPECIAL CASES:
  - Fewer than two prices: profit 0 - there is nothing to trade.
  - Strictly decreasing prices: 0 for the one-transaction variant
    (never buy), and 0 for the unlimited variant too.
  - Cooldown of one day means the free state cannot be used the very
    next day: that is the reason a THIRD variable is needed.
  - k >= number of price rises behaves exactly like the unlimited case.
  - As before, holding cash and shares can exceed 32-bit: long long.
  - days <= 1000, prices 1..10^9, k <= 100.

COMPLEXITY:
  O(days) for every variant except k-transactions, which is
  O(days * k) time; O(k) or O(1) extra space.

TIPS:
  Model the problem as STATES, not trades: "what is the best cash if I
  end the day holding/holding nothing?" Almost every stock variant
  falls out of adding one state or one transition. Write the two
  update lines on paper before coding.
*/

#include <stdio.h>

#define MAX_DAYS 1000
#define MAX_K 100
#define PRICE_LIMIT 1000000000LL

long long price[MAX_DAYS];

long long bestOneTransaction(int n) {
    long long minPrice = price[0];
    long long best = 0;
    for (int i = 1; i < n; i++) {
        long long profit = price[i] - minPrice;
        if (profit > best) best = profit;
        if (price[i] < minPrice) minPrice = price[i];
    }
    return best;
}

long long bestUnlimited(int n, long long fee) {
    long long hold = -price[0];
    long long free = 0;
    for (int i = 1; i < n; i++) {
        long long newHold = hold > free - price[i] ? hold : free - price[i];
        long long newFree = free > hold + price[i] - fee
                                ? free
                                : hold + price[i] - fee;
        hold = newHold;
        free = newFree;
    }
    return free;
}

long long bestWithCooldown(int n) {
    long long hold = -price[0];
    long long free = 0;
    const long long veryNegative = -4000000000000000000LL;
    long long sold = veryNegative;
    for (int i = 1; i < n; i++) {
        long long newHold = hold > free - price[i] ? hold : free - price[i];
        long long newSold = hold + price[i];
        long long newFree = free > sold ? free : sold;
        hold = newHold;
        free = newFree;
        sold = newSold;
    }
    return free > sold ? free : sold;
}

long long bestWithLimit(int n, int k) {
    if (k <= 0) return 0;
    long long hold[MAX_K + 1];
    long long freeState[MAX_K + 1];
    const long long veryNegative = -4000000000000000000LL;
    for (int j = 0; j <= k; j++) {
        hold[j] = veryNegative;
        freeState[j] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = k; j >= 1; j--) {
            if (hold[j] < freeState[j - 1] - price[i]) {
                hold[j] = freeState[j - 1] - price[i];
            }
            if (hold[j] != veryNegative &&
                freeState[j] < hold[j] + price[i]) {
                freeState[j] = hold[j] + price[i];
            }
        }
    }
    return freeState[k];
}

int main(void) {
    int n;

    printf("Stock DP - Buy/Sell Variants\n\n");
    printf("How many days (1-%d)? ", MAX_DAYS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_DAYS) {
        printf("Invalid day count\n");
        return 1;
    }
    printf("Enter %d prices (1..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &price[i]) != 1 || price[i] < 1 ||
            price[i] > PRICE_LIMIT) {
            printf("Invalid price\n");
            return 1;
        }
    }

    printf("\nBest single transaction profit  = %lld\n",
           bestOneTransaction(n));
    printf("Best unlimited transactions     = %lld\n", bestUnlimited(n, 0));

    long long fee;
    printf("Transaction fee (0..10^9): ");
    if (scanf("%lld", &fee) != 1 || fee < 0 || fee > PRICE_LIMIT ||
        fee * 2 > 2000000000LL) {
        printf("Invalid fee\n");
        return 1;
    }
    printf("Best unlimited with fee %lld     = %lld\n", fee,
           bestUnlimited(n, fee));
    printf("Best with a one-day cooldown    = %lld\n",
           bestWithCooldown(n));
    printf("Best with at most 2 transactions = %lld\n", bestWithLimit(n, 2));

    int k;
    printf("\nAt most how many transactions k (1-%d)? ", MAX_K);
    if (scanf("%d", &k) != 1 || k < 1 || k > MAX_K) {
        printf("Invalid k\n");
        return 1;
    }
    printf("Best with at most %d transactions = %lld\n", k,
           bestWithLimit(n, k));
    return 0;
}
