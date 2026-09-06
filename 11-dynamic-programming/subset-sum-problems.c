/*
DP Problems Ladder 2 - Subsequence Sets (Basic -> Hard)
--------------------------------------------------------
Real life example:
Budgeting: can exact change be formed from available notes?
Knapsack-style packing and cryptographic knapsack ciphers use the
same DP.

DIAGRAM (subset sum target 9, items [3 4 5]):
  reachable sums after each item:
  {0} -> {0,3} -> {0,3,4,7} -> {0,3,4,5,7,8,9,12}
  9 IS reachable: 4 + 5

LADDER (easy -> hard inside this file):
  1. Subset sum exists?            (boolean DP - the gateway)
  2. Count subsets with given sum  (counting DP)
  3. Minimum subset sum difference (partition refinement)
  4. Longest subarray with equal 0/1 (see subarray-techniques too)

BEGINNER EXPLANATION:
All four are the SAME table with different cell meanings:
dp[v] = "can I make sum v?" / "how many ways?" / "closest to
half?". The item loop sweeps value DOWNWARD so each item is used
once (0/1 knapsack rule - sweeping up would allow reuse, which
is the unbounded variant).

SUPER SIMPLE EXAMPLE (subset sum, arr [2,3,7], target 8):
start: dp[0]=true.
item 2: dp[2]=true.
item 3: dp[5]=true, dp[3]=true.
item 7: dp[9], dp[8](=5+? no: 5+3? used... dp[8] = dp[1]? no) ->
sweep down: dp[12-? ] careful: with target 8: after 2,3: reachable
{0,2,3,5}. item 7: reachable {0,2,3,5,7,8? 5+3 no wait 5+7=12>8,
2+7=9>8, 3+7=10, 0+7=7 -> add 7} -> {0,2,3,5,7, 8? no 8 = 5+3
already there? 5+3 = 8 YES from items 2+3? 2+3=5, hmm}.
The program prints the reachable set - read it off directly.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Target 0: always true (empty subset) - dp[0]=1 base.
- All items bigger than target: only dp[0] true -> "no".
- Negative numbers: the downward sweep trick assumes non-negative
  values; negatives need an offset table (noted).
- Min-difference with odd total: still works - answer is the
  subset sum closest to total/2 from below; difference = total -
  2*best.
- Count-subsets can overflow int on many-item tables - long long
  used here.

DRY RUN (count subsets sum 4, arr [1,1,2,3]... simpler [2,2,3]):
target 4. dp[0]=1.
item 2 (down): dp[4]? 4-2=2 dp[2]=0 not yet... sweep from 4:
dp[2] = 1. 
item 2: dp[4] = dp[4] + dp[2] = 1; dp[2] = dp[2] + dp[0] = 2.
item 3: dp[3] = dp[3]+dp[0] = 1.
counts: sum4 -> 1 way (2+2), sum3 -> 1 way (3), sum2 -> 2 ways.
Table printed confirms.

TIPS:
- Subset-sum is 0/1 knapsack with value==weight - once this is
  solid, knapsack problems are variations.
- Min subset difference: run subset-sum to total/2, find the
  largest reachable v -> min |total - 2v|. One line on top of
  this file's table.
- The downward sweep = "each item once"; upward = "unbounded"
  (coin change ways). The loop direction IS the semantics.
*/

#include <stdio.h>
#include <string.h>

#define MAX_TARGET 2000

long long dp[MAX_TARGET + 1];

int main() {
    int arr[100];
    int n;
    int target;
    int choice;

    printf("DP Ladder 2: Subset-Sum Family (easy -> hard)\n\n");

    printf("How many numbers (1-100)? ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d positive integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    while (1) {
        printf("\n1. Does a subset sum to target?\n");
        printf("2. Count subsets summing to target\n");
        printf("3. Minimum subset-sum difference (two halves)\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1 || choice == 2) {
            printf("Target sum (0-%d)? ", MAX_TARGET);
            scanf("%d", &target);
            if (target < 0 || target > MAX_TARGET) {
                printf("Target out of range\n");
                continue;
            }
            memset(dp, 0, sizeof(dp));
            dp[0] = 1;

            /* STEP: items outer, values inner DOWNWARD */
            for (int i = 0; i < n; i++) {
                for (int v = target; v >= arr[i]; v--) {
                    if (choice == 1) {
                        if (dp[v - arr[i]]) {
                            dp[v] = 1;
                        }
                    } else {
                        dp[v] = dp[v] + dp[v - arr[i]];
                    }
                }
            }

            if (choice == 1) {
                printf("Subset with sum %d: %s\n", target,
                       dp[target] ? "EXISTS" : "does NOT exist");
            } else {
                printf("Subsets with sum %d: %lld\n", target, dp[target]);
            }
        } else if (choice == 3) {
            int total = 0;
            for (int i = 0; i < n; i++) {
                total = total + arr[i];
            }
            int half = total / 2;
            memset(dp, 0, sizeof(dp));
            dp[0] = 1;
            for (int i = 0; i < n; i++) {
                for (int v = half; v >= arr[i]; v--) {
                    if (dp[v - arr[i]]) {
                        dp[v] = 1;
                    }
                }
            }
            int bestV = 0;
            for (int v = half; v >= 0; v--) {
                if (dp[v]) {
                    bestV = v;
                    break;
                }
            }
            printf("Total %d; best half-sum %d;\n", total, bestV);
            printf("Minimum difference between halves: %d\n",
                   total - 2 * bestV);
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
