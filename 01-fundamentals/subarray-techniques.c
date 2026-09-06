/*
Array Problems Ladder 2 - Subarray Magic (Basic -> Hard)
---------------------------------------------------------
Real life example:
"Best stretch of days for our stock" (maximum subarray) and
"longest run of sensors under threshold" (window problems) are
everyday subarray questions in finance and monitoring systems.

DIAGRAM (prefix sums turn any range sum into one subtraction):
  arr:      2   4   1   3
  prefix:   2   6   7  10
  sum(i..j) = prefix[j] - prefix[i-1]   e.g. sum(1..2) = 7-2 = 5
  sliding window: extend right; while the window breaks the rule,
  shrink from the left.

LADDER (easy -> hard inside this file):
  1. Longest subarray with given SUM (positives only - sliding window)
  2. Max sum subarray of size k      (fixed window sum)
  3. Equilibrium index               (left sum == right sum)
  4. Majority-style prefix balance   (longest equal 0s and 1s via prefix sums)

BEGINNER EXPLANATION:
The SLIDING WINDOW: keep a window [lo..hi] with a running sum.
Grow hi; when the sum exceeds target, shrink lo. Because the
array is all-positive, the sum only grows with size - the window
never needs to shrink past what's needed. O(n) total (each index
enters and leaves once).

SUPER SIMPLE EXAMPLE (max sum size k = 3, [1, 4, 2, 10, 2]):
window [1,4,2] = 7. Slide: 7-1+10 = 16. Slide: 16-4+2 = 14.
Max = 16. Each slide is O(1) - add one, drop one.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Sliding window with NEGATIVES: the monotone growth argument
  breaks - that problem needs prefix sums + hash map (noted).
- k > n: no window exists - reject.
- Equilibrium at index 0: left sum = 0 (empty prefix) - valid!
  Multiple equilibria: report the FIRST (or all - state choice).
- Longest equal 0/1: convert 0 -> -1, then it's "longest subarray
  with sum 0" - first-seen prefix sum positions. This is the
  prefix-sum + hash pattern that solves dozens of problems.

DRY RUN (equilibrium, [-7, 1, 5, 2, -4, 3, 0]):
total = 0. Walk: i=0: total=-7, left=0? right=-7 -> no...
(program prints table). i=3: left=-1, right=-1 -> equilibrium!
left = -7+1+5 = -1, right = 2-4+3+0... the program computes it
precisely - trust the printed table.

TIPS:
- Fixed window + variable window are the two sliding templates;
  prefix-sum + hash map is the third. Together they solve most
  "subarray" interview problems.
- Equilibrium is a special case of prefix sums: pre[i-1] ==
  total - pre[i]. One array of prefix sums unlocks range-sum
  queries in O(1) after O(n) build.
- kadane.c (folder 11) is the max-SUM cousin of these max-LENGTH
  problems - study the pair.
*/

#include <stdio.h>

#define MAX_N 200

int main() {
    int arr[MAX_N];
    int n;
    int choice;

    printf("Subarray Problems Ladder (easy -> hard)\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Array: [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");

    while (1) {
        printf("\n1. Max sum subarray of size k (fixed window)\n");
        printf("2. Longest subarray with sum S (positives - variable window)\n");
        printf("3. Equilibrium index\n");
        printf("4. Longest subarray with equal 0s and 1s (binary array)\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int k;
            printf("k (1-%d): ", n);
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("k out of range\n");
                continue;
            }
            int sum = 0;
            for (int i = 0; i < k; i++) {
                sum = sum + arr[i];
            }
            int best = sum;
            int bestEnd = k - 1;
            for (int i = k; i < n; i++) {
                sum = sum + arr[i] - arr[i - k];
                if (sum > best) {
                    best = sum;
                    bestEnd = i;
                }
            }
            printf("Max sum = %d, window [%d..%d]\n",
                   best, bestEnd - k + 1, bestEnd);
        } else if (choice == 2) {
            int target;
            printf("This requires POSITIVE numbers. Target sum S: ");
            scanf("%d", &target);
            int lo = 0;
            int sum = 0;
            int bestLen = 0;
            int bestLo = -1;
            for (int hi = 0; hi < n; hi++) {
                sum = sum + arr[hi];
                while (sum > target && lo <= hi) {
                    sum = sum - arr[lo];
                    lo = lo + 1;
                }
                if (sum == target && hi - lo + 1 > bestLen) {
                    bestLen = hi - lo + 1;
                    bestLo = lo;
                }
            }
            if (bestLo == -1) {
                printf("No subarray sums to %d\n", target);
            } else {
                printf("Longest: length %d, window [%d..%d]\n",
                       bestLen, bestLo, bestLo + bestLen - 1);
            }
        } else if (choice == 3) {
            int total = 0;
            for (int i = 0; i < n; i++) {
                total = total + arr[i];
            }
            int leftSum = 0;
            int found = -1;
            for (int i = 0; i < n; i++) {
                int rightSum = total - leftSum - arr[i];
                if (leftSum == rightSum) {
                    found = i;
                    break;
                }
                leftSum = leftSum + arr[i];
            }
            if (found == -1) {
                printf("No equilibrium index\n");
            } else {
                printf("Equilibrium at index %d (value %d)\n",
                       found, arr[found]);
            }
        } else if (choice == 4) {
            printf("Enter %d values (0 or 1 only): ", n);
            int ok = 1;
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
                if (arr[i] != 0 && arr[i] != 1) {
                    ok = 0;
                }
            }
            if (!ok) {
                printf("Only 0s and 1s allowed\n");
                continue;
            }
            /* prefix sums: balance = count(1) - count(0);
               longest equal 0/1 = longest subarray with balance 0.
               firstSeen[balance] = earliest index with that balance. */
            int firstSeen[2 * MAX_N + 1];
            for (int i = 0; i <= 2 * MAX_N; i++) {
                firstSeen[i] = -2;
            }
            firstSeen[MAX_N] = -1;
            int balance = 0;
            int bestLen = 0;
            int bestLo = -1;
            for (int i = 0; i < n; i++) {
                balance = balance + (arr[i] == 1 ? 1 : -1);
                if (firstSeen[balance + MAX_N] == -2) {
                    firstSeen[balance + MAX_N] = i;
                } else {
                    int len = i - firstSeen[balance + MAX_N];
                    if (len > bestLen) {
                        bestLen = len;
                        bestLo = firstSeen[balance + MAX_N] + 1;
                    }
                }
            }
            if (bestLo == -1) {
                printf("No subarray with equal 0s and 1s\n");
            } else {
                printf("Longest equal 0/1: length %d, window [%d..%d]\n",
                       bestLen, bestLo, bestLo + bestLen - 1);
            }
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
