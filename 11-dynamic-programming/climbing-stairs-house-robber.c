/*
DP Ladder - Climbing Stairs & House Robber (Basic -> Hard)
----------------------------------------------------------
Real life example:
Robber = pick jobs/ads with no two ADJACENT ones (non-overlapping
constraints); stairs = counting ways - both teach "answer from a
small fixed window of previous states".

DIAGRAM (house robber [2 7 9 3 1]):
  rob: 2,  skip 7? dp[i] = max(dp[i-1], dp[i-2] + value)
  dp: 2 7 11 11 12   -> answer 12 (2 + 9 + 1)

LADDER (easy -> hard inside this file):
  1. Climbing stairs (1 or 2 steps)   (THE easiest DP)
  2. Climbing stairs with 3-step moves (generalization)
  3. House robber (non-adjacent maximum)
  4. House robber in a CIRCLE (first and last are neighbors)

BEGINNER EXPLANATION:
DP = "answer for big problem from answers to smaller ones."
- Stairs: to reach step n you came from n-1 (one step) or n-2
  (double step): ways(n) = ways(n-1) + ways(n-2) - Fibonacci!
- Robber: at house i, either SKIP it (best = best up to i-1) or
  ROB it (best up to i-2 + money[i]): take the max. Two choices,
  one recurrence.

SUPER SIMPLE EXAMPLE (robber, [2, 7, 9, 3, 1]):
rob at 2: 2. At 7: max(2, 7) = 7. At 9: max(7, 2+9=11) = 11.
At 3: max(11, 7+3=10) = 11. At 1: max(11, 11+1=12) = 12.
Answer 12 = houses 2 + 9 + 1 (indices 0, 2, 4) - never adjacent.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Stairs with n = 0: 1 way (stand still) - the base case that
  makes the recurrence work; forgetting it gives wrong answers.
- Robber single house: rob it. Two houses: max of the two.
- Circular street: if you rob house 0 you cannot rob house n-1 -
  so answer = max(caseA: drop last, caseB: drop first). Running
  the linear solver twice is the trick.
- All-negative "money" (or 0 houses): take nothing = 0 - the
  recurrence naturally handles it if the base is set to 0.

DRY RUN (stairs, n = 4):
ways(0)=1, ways(1)=1, ways(2)=2, ways(3)=3, ways(4)=5.
The 4-step climbs: 1111, 112, 121, 211, 22 = 5. Matches!

TIPS:
- Stairs->Fibonacci->Tribonacci is the same recurrence with more
  terms - recognizing "which previous states do I need?" is the
  whole DP skill.
- House robber recurrence (skip/take max) returns in knapsack,
  job scheduling, and every "take or skip" DP - this is where
  that intuition is born.
- Space optimization: only the last 2 values matter -> O(1) space.
  Always ask "how much history do I actually need?"
*/

#include <stdio.h>

#define MAX_N 90

long long memo[MAX_N + 1];

long long stairsWays(int n) {
    if (n <= 1) {
        return 1;
    }
    if (memo[n] != 0) {
        return memo[n];
    }
    memo[n] = stairsWays(n - 1) + stairsWays(n - 2);
    return memo[n];
}

long long stairs3(int n) {
    if (n < 0) {
        return 0;
    }
    if (n == 0) {
        return 1;
    }
    return stairs3(n - 1) + stairs3(n - 2) + stairs3(n - 3);
}

long long robLinear(const int money[], int n) {
    if (n == 0) {
        return 0;
    }
    long long prev2 = 0;
    long long prev1 = money[0];
    for (int i = 1; i < n; i++) {
        long long skip = prev1;
        long long robIt = prev2 + money[i];
        prev2 = prev1;
        prev1 = (skip > robIt) ? skip : robIt;
    }
    return prev1;
}

long long robCircular(const int money[], int n) {
    if (n == 1) {
        return money[0];
    }
    if (n == 2) {
        long long m = money[0] > money[1] ? money[0] : money[1];
        return m;
    }
    long long a = robLinear(money, n - 1);
    long long b = robLinear(money + 1, n - 1);
    return (a > b) ? a : b;
}

int main() {
    int n;
    int choice;

    printf("DP Ladder: Stairs & Robber (easy -> hard)\n\n");

    while (1) {
        printf("\n1. Climbing stairs (1/2 steps)\n");
        printf("2. Climbing stairs (1/2/3 steps)\n");
        printf("3. House robber (linear street)\n");
        printf("4. House robber (circular street)\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("n (0-%d): ", MAX_N);
            scanf("%d", &n);
            if (n < 0 || n > MAX_N) {
                printf("Invalid n\n");
                continue;
            }
            for (int i = 0; i <= n; i++) {
                memo[i] = 0;
            }
            printf("Ways to climb %d steps: %lld\n", n, stairsWays(n));
        } else if (choice == 2) {
            printf("n (0-30): ");
            scanf("%d", &n);
            if (n < 0 || n > 30) {
                printf("Keep n <= 30 (naive recursion)\n");
                continue;
            }
            printf("Ways with 1/2/3-step moves: %lld\n", stairs3(n));
        } else if (choice == 3 || choice == 4) {
            int money[100];
            printf("How many houses (1-100)? ");
            scanf("%d", &n);
            printf("Enter %d money values: ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &money[i]);
            }
            if (choice == 3) {
                printf("Max loot (linear): %lld\n", robLinear(money, n));
            } else {
                printf("Max loot (circular): %lld\n", robCircular(money, n));
            }
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
