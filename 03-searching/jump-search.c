/*
Jump Search
-----------
What this program solves:
Searches a SORTED array by jumping ahead in fixed blocks of size
sqrt(n), then doing a linear scan inside the block that contains
the target. Fewer comparisons than linear search, no recursion.

Real life example:
Finding a word in a dictionary by flipping pages in fixed chunks:
jump 10 pages at a time until you overshoot, then page back one
chunk and scan line by line.

DIAGRAM (n = 16, block = sqrt(16) = 4, target 23, [1..16]):
  jump: 4 -> 8 -> 12 -> 16 (arr[16] >= 23? 16 < 23 -> out of range)
  block [12..16] contains 23 -> linear scan 13 14 ... found at 23
  jumps: sqrt(n), scan: sqrt(n) -> O(sqrt n)

How the process works:
1. step = sqrt(n); jump step, 2*step, ... while arr[step] < target.
2. When arr[min(step, n-1)] >= target, the target lies in the
   previous block.
3. Linear scan that block.

Pseudo code:
START
step = sqrt(n); prev = 0
WHILE arr[min(step,n)-1] < target
    prev = step; step = step + sqrt(n)
    IF prev >= n: RETURN -1
WHILE arr[prev] < target: prev = prev + 1
IF arr[prev] == target: RETURN prev ELSE -1
END

Sample input and dry run:
arr = [0,1,2,3,4,5,6,7,8,9,10], n = 11, target = 7
step = 3: check arr[2]=2 <7 -> jump; arr[5]=5 <7 -> jump;
arr[8]=8 >=7 -> scan from index 3.. -> found 7 at index 7

Main logic to understand:
Optimal jump = sqrt(n) balances jumping cost (sqrt(n) jumps) and
scanning cost (sqrt(n) scans) -> O(sqrt(n)) total. Requires sorted
data; O(1) space. Sits between linear O(n) and binary O(log n).
*/

#include <stdio.h>
#include <math.h>

int main() {
    int arr[200];
    int n;
    int target;

    printf("Jump Search\n\n");

    printf("How many elements (1-200)? ");
    scanf("%d", &n);

    if (n < 1 || n > 200) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers in ASCENDING order separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter value to search: ");
    scanf("%d", &target);

    int step = (int)sqrt((double)n);
    if (step < 1) {
        step = 1;
    }
    int prev = 0;
    int jumps = 0;
    int comparisons = 0;

    /* STEP: jump ahead in blocks of sqrt(n) */
    while (arr[(step < n ? step : n) - 1] < target) {
        jumps = jumps + 1;
        prev = step;
        step = step + (int)sqrt((double)n);
        if (prev >= n) {
            break;
        }
    }

    /* STEP: linear scan inside the candidate block */
    int found = -1;
    while (prev < n && prev < step) {
        comparisons = comparisons + 1;
        if (arr[prev] == target) {
            found = prev;
            break;
        }
        prev = prev + 1;
    }

    printf("\nBlock size (sqrt n): %d, jumps made: %d, scans: %d\n",
           (int)sqrt((double)n), jumps, comparisons);

    if (found != -1) {
        printf("Found %d at index %d\n", target, found);
    } else {
        printf("%d not found\n", target);
    }

    return 0;
}
