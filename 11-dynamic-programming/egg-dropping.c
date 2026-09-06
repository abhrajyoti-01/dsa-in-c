/*
Egg Dropping Puzzle
-------------------
What this program solves:
Given k eggs and n floors, find the MINIMUM number of trials needed
in the worst case to discover the critical floor (the highest floor
from which an egg can be dropped without breaking).

Real life example:
Certifying a shipping box: drop prototypes from increasing heights
of a 100-floor building. Each broken prototype is one lost egg;
the goal is a strategy minimizing the worst-case number of drops.

DIAGRAM (2 eggs, 10 floors - minimize worst-case drops):
  dp[floors][eggs]: try floor k:
  egg breaks    -> dp[k-1][eggs-1]  (search below)
  egg survives  -> dp[floors-k][eggs] (search above)
  answer = 1 + min over k of max(both outcomes)
  2 eggs, 100 floors -> 14 drops (not 10 or binary search's 50).

How the process works:
1. dp[e][f] = minimum trials with e eggs, f floors.
2. Base: dp[e][0] = 0, dp[1][f] = f (linear scan with one egg).
3. Drop from floor x: worst case =
   max(dp[e-1][x-1] (egg breaks -> below), dp[e][f-x] (survives -> above)).
4. dp[e][f] = 1 + min over x of that worst case.
   (binary-search-free full scan keeps the code simple).

Pseudo code:
START
FOR e = 1..k: dp[e][0] = 0
FOR f = 1..n: dp[1][f] = f
FOR e = 2..k
    FOR f = 1..n
        dp[e][f] = 1 + MIN over x=1..f of
            MAX(dp[e-1][x-1], dp[e][f-x])
END

Sample input and dry run:
2 eggs, 10 floors -> dp = 4
(drop 4, 7, 9, then linear - covers 10 floors in worst 4 drops)
1 egg, 10 floors -> 10 (must go floor by floor)

Main logic to understand:
Adversarial (minimax) reasoning: you pick the floor, nature picks
the worst outcome. Time O(k * n^2). The famous closed-form variant
uses the "moves + eggs = floors" identity for large n.
*/

#include <stdio.h>

#define MAX_EGGS 10
#define MAX_FLOORS 100

int dp[MAX_EGGS + 1][MAX_FLOORS + 1];

int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int eggs;
    int floors;

    printf("Egg Dropping Puzzle\n\n");

    printf("Number of eggs (1-%d): ", MAX_EGGS);
    scanf("%d", &eggs);

    printf("Number of floors (1-%d): ", MAX_FLOORS);
    scanf("%d", &floors);

    if (eggs < 1 || eggs > MAX_EGGS || floors < 1 || floors > MAX_FLOORS) {
        printf("Invalid input\n");
        return 1;
    }

    /* STEP: base cases */
    for (int f = 1; f <= floors; f++) {
        dp[1][f] = f;
    }
    for (int e = 1; e <= eggs; e++) {
        dp[e][0] = 0;
        dp[e][1] = 1;
    }

    /* STEP: fill the table */
    for (int e = 2; e <= eggs; e++) {
        for (int f = 1; f <= floors; f++) {
            dp[e][f] = MAX_FLOORS * MAX_EGGS;
            for (int x = 1; x <= f; x++) {
                int worst = maxInt(dp[e - 1][x - 1], dp[e][f - x]);
                if (1 + worst < dp[e][f]) {
                    dp[e][f] = 1 + worst;
                }
            }
        }
    }

    printf("\nMinimum trials in worst case: %d\n", dp[eggs][floors]);

    printf("\nDP table (rows = eggs, cols = floors up to %d):\n",
           floors < 10 ? floors : 10);
    for (int e = 1; e <= eggs; e++) {
        printf("  eggs=%d: ", e);
        for (int f = 1; f <= floors && f <= 10; f++) {
            printf("%3d ", dp[e][f]);
        }
        printf("\n");
    }

    return 0;
}
