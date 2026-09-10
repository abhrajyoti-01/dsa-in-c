/*
Hard DP - Burst Balloons, Maximal Square, Maximal Rectangle,
Maximum Product Subarray (Advanced -> Master)
--------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 312 - Burst Balloons (interval DP: LAST balloon, not first)
  LeetCode 221 - Maximal Square (largest all-ones square)
  LeetCode 85  - Maximal Rectangle (the 2D generalisation, built on 84)
  LeetCode 152 - Maximum Product Subarray (negatives flip the sign)
  related: HackerRank "The Maximum Subarray" family - the same Kadane
           scan with only a maximum, no product twist; solved in
           dp-interview-problems.c and 11-dynamic-programming/kadane.c.

PROBLEM DEFINITION:
Four "one twist away from the classic" DP problems. Burst Balloons
reverses the usual interval order (choose the balloon that bursts LAST),
Maximal Square and Maximal Rectangle grow a shape out of per-row
histograms, and Maximum Product Subarray must track the minimum as well
as the maximum because a negative times a negative becomes a maximum.

REAL-LIFE EXAMPLE:
Chip-fabrication layouts and wafer dicing ask "how large a rectangular
usable region fits inside this defect map?" (Maximal Rectangle/Square).
Signal mixing and portfolio back-tests ask "what contiguous stretch
maximises the product of growth factors?" (Maximum Product Subarray),
where an even number of negative factors is a good thing.

DIAGRAM (Burst Balloons, nums = [3,1,5,8], padded to [1,3,1,5,8,1]):
  dp[L][R] = best coins from bursting everything strictly between the
  sentinels L and R. Choose the LAST balloon k to burst:
      dp[L][R] = max over k of dp[L][k] + dp[k][R]
                            + nums[L]*nums[k]*nums[R]
  width 1: dp[0][2]=1*3*1=3, dp[1][3]=3*1*5=15,
           dp[2][4]=1*5*8=40, dp[3][5]=5*8*1=40
  width 2: dp[1][4] over k=2,3 -> 1*1*8+dp[2][4]=48, or
           15+1*8*1=23                      -> 48
  width 4: 3+15+24+40 = ... final dp[0][5] = 167
  (167 is the known answer for [3,1,5,8].)

PSEUDO CODE:
  BurstBalloons(a, n):
      pad: v[0] = v[n+1] = 1, v[1..n] = a
      for width = 1..n:                      (interval length)
          for L = 0..n-width: R = L + width + 1
              dp[L][R] = max over k in (L, R) of
                  dp[L][k] + dp[k][R] + v[L]*v[k]*v[R]
      answer dp[0][n+1]
  MaximalSquare(m):                          (per-cell DP)
      dp[i][j] = 0 if m[i][j] == 0
              else 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])
      answer = (max dp value) ^ 2
  MaximalRectangle(m):
      keep a per-column height; for each row the heights form a
      histogram -> Largest Rectangle in Histogram (LeetCode 84)
      -> see 05-stacks-queues-deques/largest-rectangle-histogram.c
  MaxProduct(a, n):
      best = a[0]; high = low = a[0]
      for i in 1..n-1:
          candidates = {a[i], a[i]*high, a[i]*low}
          high = max(candidates); low = min(candidates)
          best = max(best, high)

SAMPLE INPUT & DRY RUN:
  4
  3 1 5 8
  4 5
  1 0 1 0 0
  1 0 1 1 1
  1 1 1 1 1
  1 0 0 1 0
  5
  2 3 -2 4 -1
  Burst balloons: 167 coins (diagram above).
  Maximal square: side 2 -> area 4 (rows 1-2, columns 2-3 of the grid).
  Maximal rectangle: 6 (the 2x3 block in rows 2-3, columns 0-2).
  Maximum product subarray: 2*3*(-2)*4*(-1) = 48, the whole array -
  beating the [2,3] product of 6, and only found because the running
  minimum (-6 after the -2) is carried alongside the maximum.

SPECIAL CASES:
  - Burst Balloons needs the two sentinel 1s: without them the boundary
    multiples vanish and [3,1,5,8] gives 40 instead of 167.
  - Empty input has no answer; the program requires at least one value.
  - k = 0 bursts nothing: the answer is 0 coins, not 1.
  - Maximal Square with no 1s reports area 0 (not "no square found"
    with a random number).
  - Maximum Product Subarray MUST carry the minimum: for [2,3,-2,4]
    the answer is 6 (2*3), and for [-2,-3] it is 6, which is only seen
    if the negative product is kept alive.
  - Maximum Product Subarray overflows long long on paper, so this
    program caps that part at m <= 12 values of |a[i]| <= 10 and prints
    the cap in the prompt instead of failing silently.
  - Rows are 0/1 digits separated by spaces; non-digit input is
    rejected instead of silently becoming 0.
  - n <= 100 balloons with 0 <= value <= 1000, rows/cols <= 50 for the
    grids, and the product array m <= 12 with |value| <= 10. The tight
    product bound is deliberate: a full window of twelve 10s is 10^12,
    which still fits a 64-bit integer, while 1000^1000 does not.
    Choosing a bound that provably cannot overflow is part of the
    specification, not a detail.

COMPLEXITY:
  Burst Balloons: O(n^3) time, O(n^2) space (the price of the reversed
  interval reasoning). Maximal Square: O(rows*cols) time, one row of
  extra space. Maximal Rectangle: O(rows*cols) time with a monotonic
  stack. Maximum Product: O(n) time, O(1) space.

TIPS:
  "Which item is burst FIRST?" has no independent subproblems; "which
  item is burst LAST?" splits the array cleanly. Whenever an interval
  DP looks unsolvable, flip the order of the choice.
  For products, remember the two-state rule: track best AND worst.
*/

#include <stdio.h>

#define MAX_BALLOONS 100
#define MAX_GRID 50
#define MAX_PRODUCT 12

long long balloons[MAX_BALLOONS + 2];
int grid[MAX_GRID][MAX_GRID];
long long productValues[MAX_PRODUCT];
long long dp[MAX_BALLOONS + 2][MAX_BALLOONS + 2];

/* LeetCode 312: burst the balloons in the order that maximises coins.
 * Choosing the LAST balloon k to burst (rather than the first) makes the
 * left and right sub-ranges independent, because k is the only neighbour
 * still standing when each side is finished. */
long long burstBalloons(int n) {
    int width;
    for (width = 1; width <= n; width++) {
        for (int left = 0; left + width + 1 <= n + 1; left++) {
            int right = left + width + 1;
            long long best = 0;
            for (int k = left + 1; k < right; k++) {
                long long coins = dp[left][k] + dp[k][right] +
                                  balloons[left] * balloons[k] * balloons[right];
                if (coins > best) best = coins;
            }
            dp[left][right] = best;
        }
    }
    return dp[0][n + 1];
}

/* LeetCode 221: dp[i][j] is the side of the largest all-ones square whose
 * bottom-right corner is (i, j). A 1 extends the smallest of its three
 * neighbours; the answer is the maximum side, squared. */
int maximalSquare(int rows, int cols) {
    int best = 0;
    int previous[MAX_GRID];
    int current[MAX_GRID];
    for (int j = 0; j < cols; j++) previous[j] = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 0) {
                current[j] = 0;
                continue;
            }
            if (i == 0 || j == 0) {
                current[j] = 1;
            } else {
                int a = previous[j];
                int b = current[j - 1];
                int c = previous[j - 1];
                int smallest = a < b ? a : b;
                if (c < smallest) smallest = c;
                current[j] = 1 + smallest;
            }
            if (current[j] > best) best = current[j];
        }
        for (int j = 0; j < cols; j++) previous[j] = current[j];
    }
    return best;
}

/* LeetCode 85: for every row the accumulated column heights form a
 * histogram, and the Largest Rectangle in Histogram (LeetCode 84, see
 * 05-stacks-queues-deques/largest-rectangle-histogram.c) turns each row
 * into one monotonic-stack pass. */
int maximalRectangle(int rows, int cols) {
    int heights[MAX_GRID];
    int stack[MAX_GRID + 1];
    for (int j = 0; j < cols; j++) heights[j] = 0;
    int best = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            heights[j] = grid[i][j] == 1 ? heights[j] + 1 : 0;
        }
        int top = -1;
        for (int j = 0; j <= cols; j++) {
            int h = (j == cols) ? 0 : heights[j];
            while (top >= 0 && heights[stack[top]] >= h) {
                int height = heights[stack[top]];
                top--;
                int leftBoundary = (top < 0) ? -1 : stack[top];
                int width = j - leftBoundary - 1;
                int area = height * width;
                if (area > best) best = area;
            }
            if (j < cols) {
                top++;
                stack[top] = j;
            }
        }
    }
    return best;
}

/* LeetCode 152: a negative value swaps the roles of the best and worst
 * product, so BOTH must be carried through the scan. */
long long maxProductSubarray(int n) {
    long long best = productValues[0];
    long long high = productValues[0];
    long long low = productValues[0];
    for (int i = 1; i < n; i++) {
        long long v = productValues[i];
        long long optionHigh = v * high;
        long long optionLow = v * low;
        long long newHigh = v;
        if (optionHigh > newHigh) newHigh = optionHigh;
        if (optionLow > newHigh) newHigh = optionLow;
        long long newLow = v;
        if (optionHigh < newLow) newLow = optionHigh;
        if (optionLow < newLow) newLow = optionLow;
        high = newHigh;
        low = newLow;
        if (high > best) best = high;
    }
    return best;
}

int readGrid(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int v;
            if (scanf("%d", &v) != 1 || (v != 0 && v != 1)) {
                printf("Invalid grid (use 0 and 1 only)\n");
                return -1;
            }
            grid[i][j] = v;
        }
    }
    return 0;
}

int main(void) {
    printf("Hard DP - Burst Balloons, Maximal Square, Maximal Rectangle, "
           "Maximum Product\n\n");

    int n;
    printf("--- LeetCode 312: burst balloons ---\n");
    printf("How many balloons (1-%d)? ", MAX_BALLOONS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_BALLOONS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d values (0..1000); a neighbour of value 1 acts as the "
           "boundary:\n", n);
    for (int i = 1; i <= n; i++) {
        if (scanf("%lld", &balloons[i]) != 1 || balloons[i] < 0 ||
            balloons[i] > 1000) {
            printf("Invalid value\n");
            return 1;
        }
    }
    balloons[0] = 1;
    balloons[n + 1] = 1;
    printf("Maximum coins: %lld\n", burstBalloons(n));

    int rows;
    int cols;
    printf("\n--- LeetCode 221 / 85: grids of 0s and 1s ---\n");
    printf("Rows and columns (1-%d each): ", MAX_GRID);
    if (scanf("%d %d", &rows, &cols) != 2 || rows < 1 || cols < 1 ||
        rows > MAX_GRID || cols > MAX_GRID) {
        printf("Invalid grid size\n");
        return 1;
    }
    printf("Enter %d values row by row:\n", rows * cols);
    if (readGrid(rows, cols) != 0) return 1;
    int squareSide = maximalSquare(rows, cols);
    printf("Maximal square side: %d -> area %d\n", squareSide,
           squareSide * squareSide);
    printf("Maximal rectangle area: %d\n", maximalRectangle(rows, cols));

    int m;
    printf("\n--- LeetCode 152: maximum product subarray ---\n");
    printf("How many values (1-%d)? ", MAX_PRODUCT);
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_PRODUCT) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d values (-10..10):\n", m);
    for (int i = 0; i < m; i++) {
        if (scanf("%lld", &productValues[i]) != 1 ||
            productValues[i] < -10 || productValues[i] > 10) {
            printf("Invalid value\n");
            return 1;
        }
    }
    printf("Maximum product: %lld\n", maxProductSubarray(m));
    return 0;
}
