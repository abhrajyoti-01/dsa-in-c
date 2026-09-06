/*
 PROBLEM DEFINITION:
 --------------------
 The Maximum Subarray problem finds the CONTIGUOUS run of elements within
 a one-dimensional array that has the largest sum. Kadane's Algorithm
 solves it in a single O(N) pass:

     currentMax = max(nums[i], currentMax + nums[i])
     globalMax  = max(globalMax, currentMax)

 Intuition: at each element, decide whether to EXTEND the previous
 subarray or START FRESH at this element. Extending is worth it only if
 the running sum before this element is positive (it adds value);
 otherwise a negative prefix would only drag the total down.

 REAL-LIFE EXAMPLE - Stock Trading & Profit Streaks:
 -----------------------------------------------------
 A trader studies daily profit/loss deltas: [+3, -2, +5, -1, +6, -4].
 The best contiguous streak is days 2-6: -2+5-1+6 = +8 - the maximum
 money made holding through one uninterrupted stretch. Selling and
 re-buying splits the streak, which the problem forbids (contiguity).
 The same analysis applies to: game score swings, CPU load spikes
 (maximum sustained load window), and signal processing (strongest
 continuous signal segment).

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Walk the array once, carrying a "running best ending here":

 1. INITIALIZATION:
     currentMax = globalMax = first element
     (start = end = tempStart = 0 for index tracking)

 2. SINGLE PASS (i = 1 .. n-1):
     Choice A: extend   -> currentMax + nums[i]
     Choice B: restart  -> nums[i] alone
     Pick the larger:
        If nums[i] > currentMax + nums[i]:
            currentMax = nums[i]        (fresh start at i)
            tempStart = i               (remember candidate start)
        Else:
            currentMax += nums[i]       (extend previous run)

     Update the global answer:
        If currentMax > globalMax:
            globalMax = currentMax
            start = tempStart, end = i  (lock in the winning window)

 3. RESULT:
     globalMax is the best contiguous sum; [start..end] is the window.

 KEY INSIGHT (why greedy works here):
     A negative prefix NEVER helps the future. The moment the running
     sum drops below the current element alone, drop the history.

 DIAGRAM (decision at each element, extend vs restart):
   index:   0    1    2    3    4    5    6    7    8
   value:  -2    1   -3    4   -1    2    1   -5    4
            |    |         \___________/      |
            |    |            extend keeps    |
            |    |            growing         |
   restart when history drags you below starting fresh:
   -2 loses to 1 -> restart at 1
   -2 (1-3) loses to 4 -> restart at 4
   then extend through -1, +2, +1 -> best window [4,-1,2,1] = 8

 PSEUDO CODE:
 ------------
 START
   Function Kadane(nums[], n):
     currentMax = nums[0]
     globalMax = nums[0]
     tempStart = 0, start = 0, end = 0

     For i = 1 to n-1:
         If nums[i] > currentMax + nums[i]:
             currentMax = nums[i]
             tempStart = i
         Else:
             currentMax = currentMax + nums[i]

         If currentMax > globalMax:
             globalMax = currentMax
             start = tempStart
             end = i

     Return globalMax, start, end
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Array: [-2, 1, -3, 4, -1, 2, 1, -5, 4]

 i=1 (val  1): extend?  -2+1=-1 < 1 -> restart. currentMax=1,  tempStart=1
                globalMax = 1  (start=1,end=1)
 i=2 (val -3): extend?   1-3=-2 > -3 -> extend. currentMax=-2
                globalMax stays 1
 i=3 (val  4): extend?  -2+4=2 < 4 -> restart. currentMax=4,  tempStart=3
                globalMax = 4  (start=3,end=3)
 i=4 (val -1): extend?   4-1=3 > -1 -> extend. currentMax=3
                globalMax stays 4
 i=5 (val  2): extend?   3+2=5 > 2 -> extend. currentMax=5
                globalMax = 5  (start=3,end=5)
 i=6 (val  1): extend?   5+1=6 > 1 -> extend. currentMax=6
                globalMax = 6  (start=3,end=6)
 i=7 (val -5): extend?   6-5=1 > -5 -> extend. currentMax=1
                globalMax stays 6
 i=8 (val  4): extend?   1+4=5 > 4 -> extend. currentMax=5
                globalMax stays 6

 Best subarray: indices 3..6 -> [4, -1, 2, 1], sum = 8

 TIME COMPLEXITY: O(N) - single pass, constant work per element
 SPACE COMPLEXITY: O(1) - just a few variables

 COMMON USE CASES:
 - Maximum profit streak analysis (stocks, sales)
 - Signal processing: strongest contiguous signal
 - Game development: best score-streak detection
 - Image analysis: brightest horizontal band
 - Foundation for 2-D variant (max-sum rectangle in a matrix)
*/

#include <stdio.h>
#include <stdlib.h>

/*
 * kadane - Core O(N) algorithm. Returns the maximum subarray sum and
 * reports the winning window via out-parameters start/end.
 *
 * Analogy: A trader asks at every day: "keep holding, or start fresh?"
 */
int kadane(int nums[], int n, int* start, int* end) {
    int currentMax = nums[0];
    int globalMax = nums[0];
    int tempStart = 0;

    *start = 0;
    *end = 0;

    for (int i = 1; i < n; i++) {
        if (nums[i] > currentMax + nums[i]) {
            currentMax = nums[i];
            tempStart = i;
        } else {
            currentMax = currentMax + nums[i];
        }

        if (currentMax > globalMax) {
            globalMax = currentMax;
            *start = tempStart;
            *end = i;
        }
    }

    return globalMax;
}

int main() {
    int arr[100];
    int n;

    printf("Kadane's Algorithm - Maximum Subarray\n\n");

    printf("How many elements (1-100)? ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("\nArray: [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");

    int start, end;
    int maxSum = kadane(arr, n, &start, &end);

    printf("Maximum subarray sum: %d\n", maxSum);
    printf("Window: indices %d..%d -> [", start, end);
    for (int i = start; i <= end; i++) {
        printf("%d", arr[i]);
        if (i < end) printf(", ");
    }
    printf("]\n");

    return 0;
}

