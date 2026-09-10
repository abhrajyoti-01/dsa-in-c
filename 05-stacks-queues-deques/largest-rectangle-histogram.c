/*
Largest Rectangle in a Histogram - Monotonic Stack (Intermediate)
----------------------------------------------------------------
PROBLEM DEFINITION:
Given non-negative heights of unit-width bars, find the largest
rectangle contained under consecutive bars and report its interval.

REAL-LIFE EXAMPLE:
Find the largest rectangular area in a skyline or available storage
profile. Repeating this on rows also solves maximal rectangle in a
binary matrix.

DIAGRAM (heights [2,1,5,6,2,3]):
  6 |       #
  5 |     # #       bars 2..3, height 5, width 2 -> area 10
  4 |     # #
  3 |     # #   #
  2 | #   # # # #
  1 | # # # # # #
      0 1 2 3 4 5

PSEUDO CODE:
  Keep a stack of indices with strictly increasing heights
  For i = 0..n (use a final virtual height 0):
      While stack not empty and height[top] >= current height:
          Pop bar; right = i-1
          left = 0 if stack empty, otherwise new top + 1
          area = height[bar] * (right-left+1); update best
      Push i if i < n

SAMPLE INPUT & DRY RUN:
  6
  2 1 5 6 2 3
  At i=4, pop height 6: width 1 -> area 6.
  Then pop height 5: left=2, right=3 -> area 10 (the maximum).
  The final virtual bar flushes the remaining candidates.

SPECIAL CASES:
  - Equal heights are popped; the later equal bar can extend farther.
  - Increasing heights need the final flush; decreasing heights pop early.
  - All zeros: maximum area 0, no positive-area rectangle.
  - n is 1..1000, heights 0..10^9; long long prevents area overflow.
  - If several rectangles tie, retain the first one evaluated.

COMPLEXITY:
  O(n) time (each index pushed/popped once), O(n) stack space.

TIPS:
  Store INDICES, not just heights: widths require positions.
  Compare the nearest-smaller idea with next-greater-stock-span.
*/

#include <stdio.h>

#define MAX_N 1000
#define HEIGHT_LIMIT 1000000000LL

int main(void) {
    long long height[MAX_N];
    int stack[MAX_N];
    int top = -1;
    int n;
    long long bestArea = 0;
    long long bestHeight = 0;
    int bestLeft = -1;
    int bestRight = -1;

    printf("Largest Rectangle in a Histogram\n\n");
    printf("How many bars (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d heights (0..10^9): ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &height[i]) != 1 ||
            height[i] < 0 || height[i] > HEIGHT_LIMIT) {
            printf("Invalid height\n");
            return 1;
        }
    }

    printf("\nPopped-bar trace (zero-based ranges):\n");
    for (int i = 0; i <= n; i++) {
        long long current = i == n ? 0 : height[i];
        while (top >= 0 && height[stack[top]] >= current) {
            int bar = stack[top--];
            int left = top < 0 ? 0 : stack[top] + 1;
            int right = i - 1;
            long long area = height[bar] * (right - left + 1);
            printf("  height %lld, range [%d..%d], area %lld\n",
                   height[bar], left, right, area);
            if (area > bestArea) {
                bestArea = area;
                bestHeight = height[bar];
                bestLeft = left;
                bestRight = right;
            }
        }
        if (i < n) {
            stack[++top] = i;
        }
    }

    printf("Maximum area: %lld\n", bestArea);
    if (bestLeft >= 0) {
        printf("Rectangle: [%d..%d], height %lld\n",
               bestLeft, bestRight, bestHeight);
    } else {
        printf("No positive-area rectangle\n");
    }
    return 0;
}
