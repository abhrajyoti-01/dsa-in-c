/*
Monotonic Stack Master Class - Next Greater, Daily Temperatures,
Largest Rectangle, Trapping Rain Water (Intermediate -> Advanced)
------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 496 - Next Greater Element I
  LeetCode 739 - Daily Temperatures
  LeetCode 84  - Largest Rectangle in Histogram
  LeetCode 42  - Trapping Rain Water (stack method)
  LeetCode 901 - Online Stock Span (the same "wait for a bigger value")
  HackerRank "Largest Rectangle" (direct port of LeetCode 84)

PROBLEM DEFINITION:
One idea - keep a stack of indices whose values are monotone - answers
five famous problems. Each element is pushed once and popped once, so
the total work is linear even though the loops look nested.

REAL-LIFE EXAMPLE:
A trading dashboard: for every day, "how many days until a higher
price?" (daily temperatures) and "how high can a block of bars reach
before a shorter bar cuts it off?" (largest rectangle). Both are the
same stack discipline on a price/height series.

DIAGRAM (heights [2,1,5,6,2,3], next-greater to the right):
  index:  0  1  2  3  4  5
  value:  2  1  5  6  2  3
  next greater: 2 <- 5, 1 <- 5, 5 <- 6, 6 <- none, 2 <- 3, 3 <- none
  stack while scanning:
    0(2) push; 1(1) push; 2(5) pops 1 -> ans[1]=5, pops 0 -> ans[0]=5
  Each index enters and leaves the stack exactly once.

PSEUDO CODE:
  NextGreater(a): stack of indices, empty
      for i = 0..n-1:
          while stack not empty and a[top] < a[i]:
              ans[pop] = a[i]
          push i
      remaining indices have no greater element (report -1)
  DailyTemperatures(a): identical, answer stores (i - popped index)
  LargestRectangle(a): same stack, area = a[popped] * (i - newTop - 1)
  TrappingRainWater(a): monotone DECREASING stack of bars; when a taller
      bar arrives, water above the popped bar is
      min(a[left], a[right]) - a[popped] times the width between them

SAMPLE INPUT & DRY RUN:
  6
  2 1 5 6 2 3
  Next greater: 5 5 6 -1 3 -1      (as values)
  Days to wait: 2 1 1 0 1 0        (as distances)
  Largest rectangle: 10 over bars [2..3]   (height 5, width 2)
  Trapped rain water: 2
    leftMax  = 2 2 5 6 6 6
    rightMax = 6 6 6 6 3 3
    min(l,r) - h = 0 1 0 0 1 0  ->  1 unit above index 1 and 1 above
    index 4, so the total is 2 (NOT 1 - a very common miscount).

SPECIAL CASES:
  - Strictly decreasing input: nothing is ever popped early, so the
    final flush assigns -1 to everything - the classic missed case.
  - Equal values: the strict comparison `<` keeps equals on the stack
    (distance to a strictly greater value). Use `<=` for
    next-greater-or-equal; state which one you want.
  - n = 1: no greater element, area = height, and no trapping.
  - Trapping needs both walls: end bars can never hold water.
  - n <= 1000, heights 0..10^9; areas use long long.

COMPLEXITY:
  O(n) time and O(n) stack space for every part - the amortised
  argument is "each index is pushed once and popped at most once".

TIPS:
  Recognise the trigger phrase "next bigger/smaller element" or "how
  far until a taller wall". If a brute-force loop looks O(n^2) but
  every element is only processed once, you are looking at a monotone
  stack. Store INDICES (not values) whenever a distance or width is
  needed.
*/

#include <stdio.h>

#define MAX_N 1000
#define HEIGHT_LIMIT 1000000000LL

long long height[MAX_N];
long long nextGreater[MAX_N];
long long days[MAX_N];
int stack[MAX_N];

int main(void) {
    int n;

    printf("Monotonic Stack Master Class\n\n");
    printf("How many values (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d non-negative integers (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &height[i]) != 1 || height[i] < 0 ||
            height[i] > HEIGHT_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        nextGreater[i] = -1;
    }

    int top = -1;
    for (int i = 0; i < n; i++) {
        while (top >= 0 && height[stack[top]] < height[i]) {
            nextGreater[stack[top--]] = height[i];
        }
        stack[++top] = i;
    }
    printf("\nNext greater element to the right:");
    for (int i = 0; i < n; i++) printf(" %lld", nextGreater[i]);
    printf("\n");

    top = -1;
    for (int i = 0; i < n; i++) {
        days[i] = 0;
        while (top >= 0 && height[stack[top]] < height[i]) {
            int index = stack[top--];
            days[index] = i - index;
        }
        stack[++top] = i;
    }
    printf("Days until a warmer temperature:");
    for (int i = 0; i < n; i++) printf(" %lld", days[i]);
    printf("\n");

    top = -1;
    long long bestArea = 0;
    int bestLeft = -1;
    int bestRight = -1;
    for (int i = 0; i <= n; i++) {
        long long current = i == n ? 0 : height[i];
        while (top >= 0 && height[stack[top]] >= current) {
            int bar = stack[top--];
            int left = top < 0 ? 0 : stack[top] + 1;
            long long area = height[bar] * (i - left);
            if (area > bestArea) {
                bestArea = area;
                bestLeft = left;
                bestRight = i - 1;
            }
        }
        if (i < n) stack[++top] = i;
    }
    printf("Largest rectangle area = %lld", bestArea);
    if (bestLeft >= 0) {
        printf(" over bars [%d..%d]", bestLeft, bestRight);
    }
    printf("\n");

    top = -1;
    long long water = 0;
    for (int i = 0; i < n; i++) {
        while (top >= 0 && height[stack[top]] <= height[i]) {
            int bar = stack[top--];
            if (top < 0) break;
            int left = stack[top];
            long long wall = height[left] < height[i] ? height[left]
                                                      : height[i];
            water += (wall - height[bar]) * (i - left - 1);
        }
        stack[++top] = i;
    }
    printf("Trapped rain water = %lld\n", water);
    return 0;
}
