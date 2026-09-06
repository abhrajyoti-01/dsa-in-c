/*
Interval Problems Ladder (Basic -> Hard)
-----------------------------------------
Real life example:
Calendar apps merge overlapping bookings into single busy blocks;
"minimum arrows to burst balloons" is the same overlap logic.

DIAGRAM (merge overlapping intervals):
  [1,3] [2,6] [8,10]
  sort by start: [1,3] [2,6] [8,10]
  [1,3] + [2,6] overlap -> [1,6]
  [1,6] + [8,10] disjoint -> keep
  result: [1,6] [8,10]

LADDER (easy -> hard inside this file):
  1. Meeting rooms: can ONE person attend all meetings? (overlap check)
  2. Merge overlapping intervals
  3. Minimum platforms (sweep line with arrivals/departures)
  4. Minimum arrows to burst balloons (points vs intervals)

BEGINNER EXPLANATION:
All interval problems start with one move: SORT BY START TIME.
Then either:
- walk adjacent pairs (overlap check / merge),
- or convert to EVENTS: train arrives = +1 person, departs = -1;
  walk the timeline in order, track the running total - its MAX
  is the peak requirement (platforms, conference rooms).

SUPER SIMPLE EXAMPLE (merge):
[1,3] [2,6] [8,10] [15,18]
Sorted already. 1-3 then 2-6: 2 <= 3 -> overlap -> merge to [1,6].
[8,10]: 8 > 6 -> no overlap, start new. [15,18]: new.
Merged: [1,6], [8,10], [15,18].

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Touching intervals [1,2] [2,3]: overlap or not? Conventions
  differ - closed intervals share the instant 2 (merge), half-open
  do not. This file uses CLOSED (merge on touch) - document it.
- Minimum platforms with arrival == departure at the same time:
  platform freed THEN reused -> process departures BEFORE arrivals
  at equal times (sort departs first in the tie).
- Arrows: a balloon burst by an arrow at x covers intervals
  CONTAINING x - a point on the boundary counts (closed). Sort by
  end; shoot at the current interval's end - the greedy proof.
- Unsorted input: sorting first is what makes adjacent-pair logic
  valid - never skip the sort.

DRY RUN (min platforms, arr=[1,2,3], dep=[2,3,4]):
Events sorted by time (dep before arr on ties):
t=1 arr +1 -> count 1 (max 1)
t=2 dep -1 -> count 0; then arr +1 -> count 1 (max 1)
t=3 dep -1 -> 0; arr +1 -> 1 (max 1)
t=4 dep -1 -> 0.
Max = 1: one train at a time - 1 platform suffices. With
arr=[1,2,3], dep=[3,3,4]: t=3 dep -1, arr +1, ... max 2.

TIPS:
- Sort-by-start then sweep is THE interval template; overlapping
  detection, calendar merging, and resource allocation all reuse it.
- "Events with +1/-1" is the sweep line technique - it returns in
  computational geometry (folder 17) for segment counting.
- Arrows/balloons is the classic greedy-with-sorting proof
  exercise: shoot at the smallest end among currently active.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_INT 50

typedef struct {
    int start;
    int end;
} Interval;

typedef struct {
    int time;
    int delta;
} Event;

int compareEvent(const void* a, const void* b) {
    const Event* x = (const Event*)a;
    const Event* y = (const Event*)b;
    if (x->delta != y->delta && x->time == y->time) {
        return x->delta - y->delta;
    }
    return x->time - y->time;
}

int compareStart(const void* a, const void* b) {
    const int* x = (const int*)a;
    const int* y = (const int*)b;
    return x[0] - y[0];
}

int main() {
    int n;
    int choice;

    printf("Interval Problems Ladder (easy -> hard)\n\n");

    printf("How many intervals (1-%d)? ", MAX_INT);
    scanf("%d", &n);

    if (n < 1 || n > MAX_INT) {
        printf("Invalid count\n");
        return 1;
    }

    int start[MAX_INT];
    int end[MAX_INT];
    printf("Enter start end for each interval:\n");
    for (int i = 0; i < n; i++) {
        printf("  Interval %d: ", i + 1);
        scanf("%d %d", &start[i], &end[i]);
    }

    while (1) {
        printf("\n1. Can one person attend all? (overlap check)\n");
        printf("2. Merge intervals\n");
        printf("3. Minimum platforms (sweep line)\n");
        printf("4. Minimum arrows for balloons\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int idx[MAX_INT];
            for (int i = 0; i < n; i++) {
                idx[i] = i;
            }
            /* sort by start using the pairs */
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - 1 - i; j++) {
                    if (start[idx[j]] > start[idx[j + 1]]) {
                        int t = idx[j];
                        idx[j] = idx[j + 1];
                        idx[j + 1] = t;
                    }
                }
            }
            int conflict = 0;
            for (int i = 0; i < n - 1; i++) {
                int a = idx[i];
                int b = idx[i + 1];
                if (start[b] < end[a]) {
                    printf("Overlap: [%d,%d] and [%d,%d]\n",
                           start[a], end[a], start[b], end[b]);
                    conflict = 1;
                    break;
                }
            }
            if (!conflict) {
                printf("All meetings fit in ONE room\n");
            }
        } else if (choice == 2) {
            /* merge intervals */
            int s2[MAX_INT];
            int e2[MAX_INT];
            for (int i = 0; i < n; i++) {
                s2[i] = start[i];
                e2[i] = end[i];
            }
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - 1 - i; j++) {
                    if (s2[j] > s2[j + 1]) {
                        int t = s2[j];
                        s2[j] = s2[j + 1];
                        s2[j + 1] = t;
                        t = e2[j];
                        e2[j] = e2[j + 1];
                        e2[j + 1] = t;
                    }
                }
            }
            printf("Merged intervals:\n");
            int cs = s2[0];
            int ce = e2[0];
            for (int i = 1; i < n; i++) {
                if (s2[i] <= ce) {
                    if (e2[i] > ce) {
                        ce = e2[i];
                    }
                } else {
                    printf("  [%d, %d]\n", cs, ce);
                    cs = s2[i];
                    ce = e2[i];
                }
            }
            printf("  [%d, %d]\n", cs, ce);
        } else if (choice == 3) {
            Event events[2 * MAX_INT];
            for (int i = 0; i < n; i++) {
                events[2 * i].time = start[i];
                events[2 * i].delta = 1;
                events[2 * i + 1].time = end[i];
                events[2 * i + 1].delta = -1;
            }
            qsort(events, (size_t)(2 * n), sizeof(Event), compareEvent);

            int current = 0;
            int maxCount = 0;
            for (int i = 0; i < 2 * n; i++) {
                current = current + events[i].delta;
                if (current > maxCount) {
                    maxCount = current;
                }
            }
            printf("Minimum platforms/rooms required: %d\n", maxCount);
        } else if (choice == 4) {
            /* arrows: sort intervals by end; greedy shoot at ends */
            int s2[MAX_INT];
            int e2[MAX_INT];
            for (int i = 0; i < n; i++) {
                s2[i] = start[i];
                e2[i] = end[i];
            }
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - 1 - i; j++) {
                    if (e2[j] > e2[j + 1]) {
                        int t = e2[j];
                        e2[j] = e2[j + 1];
                        e2[j + 1] = t;
                        t = s2[j];
                        s2[j] = s2[j + 1];
                        s2[j + 1] = t;
                    }
                }
            }
            int arrows = 1;
            int shotAtVal = e2[0];
            for (int i = 1; i < n; i++) {
                if (s2[i] > shotAtVal) {
                    arrows = arrows + 1;
                    shotAtVal = e2[i];
                }
            }
            printf("Minimum arrows: %d\n", arrows);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
