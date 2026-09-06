/*
Minimum Number of Platforms (Sweep Line Classic)
-------------------------------------------------
What this program solves:
Given train arrival and departure times, find the MINIMUM number
of platforms so no train ever waits - THE classic greedy/sweep
interview problem (same math for conference rooms, call center
agents, hospital beds).

Real life example:
Railway station platform planning and call-center staffing: sweep
through the day counting how many resources are busy at once.

DIAGRAM (sweep line over sorted arrivals/departures):
  arr:  9:00 9:10 9:30      dep: 9:20 10:00 10:20
  9:00 +1 -> 1     9:10 +1 -> 2     9:20 -1 -> 1
  9:30 +1 -> 2    10:00 -1 -> 1    10:20 -1 -> 0
  peak count = 2 -> 2 platforms needed.

BEGINNER EXPLANATION:
Imagine a counter that goes +1 when a train ARRIVES and -1 when
one DEPARTS. Walk through time in sorted order. The counter's
highest value = how many trains were simultaneously parked =
platforms needed. No pairing logic, no nested loops - just a
sorted timeline.

SUPER SIMPLE EXAMPLE:
Arrivals:   1:00, 1:10, 1:20
Departures: 1:30, 1:25, 1:40
At 1:20 all three trains are still there (none departed) -> max
concurrent = 3 -> need 3 platforms.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Departure at the same instant as another arrival: the departing
  train frees the platform BEFORE the new one uses it -> process
  DEPARTURES before arrivals at the same timestamp. Getting the
  tie-break wrong inflates the answer by 1 - the classic bug.
- Zero-duration trains (arrive == depart): the train still needs
  a platform for that instant; with the dep-first tie rule it
  contributes correctly.
- Overlap of ALL trains: answer = n.
- Trains crossing midnight: this file treats all times on one
  axis (0..2359 style); wrap-around scheduling needs splitting
  intervals - mention, don't solve.

DRY RUN (arr = [10:00, 10:05, 10:10], dep = [10:20, 10:15, 10:30]):
Events: (600,+1) (605,+1) (610,+1) (615,-1) (620,-1) (630,-1)
count: 1, 2, 3 (max!), 2, 1, 0.
Answer: 3 platforms - all three overlap between 10:10 and 10:15.

TIPS:
- Sort arrivals and departures SEPARATELY and merge-walk them
  (two pointers) OR merge into one event list - both O(n log n);
  the tie rule (departures first) lives in the comparator.
- The running-max pattern = sweep line; the same code counts
  simultaneous meetings (rooms), live users (servers), or depth
  of nesting.
- Compare with the two-pointer alternative: i over arrivals, j
  over departures - identical result, slightly fewer objects.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_TRAINS 50

int compareInt(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return x - y;
}

int main() {
    int n;
    int arrival[MAX_TRAINS];
    int departure[MAX_TRAINS];

    printf("Minimum Number of Platforms (sweep line)\n\n");

    printf("How many trains (1-%d)? ", MAX_TRAINS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_TRAINS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d arrival times (e.g. 1000 for 10:00): ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arrival[i]);
    }
    printf("Enter %d departure times: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &departure[i]);
    }

    qsort(arrival, (size_t)n, sizeof(int), compareInt);
    qsort(departure, (size_t)n, sizeof(int), compareInt);

    printf("\nSorted arrivals:   ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arrival[i]);
    }
    printf("\nSorted departures: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", departure[i]);
    }
    printf("\n\n");

    /* STEP: merge-walk both sorted lists */
    int i = 0;
    int j = 0;
    int current = 0;
    int maxNeeded = 0;
    int eventTime = -1;

    while (i < n && j < n) {
        if (arrival[i] < departure[j]) {
            current = current + 1;
            eventTime = arrival[i];
            printf("t=%d: arrival  -> trains on platform: %d\n",
                   eventTime, current);
            i = i + 1;
        } else {
            current = current - 1;
            eventTime = departure[j];
            printf("t=%d: departure-> trains on platform: %d\n",
                   eventTime, current);
            j = j + 1;
        }
        if (current > maxNeeded) {
            maxNeeded = current;
        }
    }

    printf("\nMinimum platforms required: %d\n", maxNeeded);

    return 0;
}
