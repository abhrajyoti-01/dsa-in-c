/*
Activity Selection Problem
--------------------------
What this program solves:
This program selects the maximum number of non-overlapping activities from a
given set of start times and finish times.

Real life example:
Imagine a college seminar hall that can host only one event at a time. Many
student groups request the hall for different time intervals. The goal is to
schedule the maximum number of events without clashes.

DIAGRAM (sort by finish time, take non-overlapping):
  jobs:  A(1-4) B(3-5) C(0-6) D(5-7) E(3-8)
  sorted: A C D E ...
  take A [1-4] -> C overlaps? C(0-6) yes -> skip
  take D [5-7] -> E overlaps -> skip
  chosen: A, D

How the process works:
1. Read start and finish times of all activities.
2. Sort the activities by finishing time.
3. Select the activity that finishes first.
4. Then keep selecting the next activity whose start time is greater than or
   equal to the finish time of the last selected activity.

Pseudo code:
START
Read n and all activities
Sort activities by finish time
Select the first activity
Set lastFinish = finish time of first activity
For each remaining activity
    If activity start time >= lastFinish
        Select the activity
        Update lastFinish
Print selected activities
END

Dry run idea:
If activities are:
(1,2), (3,4), (0,6), (5,7), (8,9)
First choose (1,2), then (3,4), then (5,7), then (8,9).
These do not overlap, so the answer contains 4 activities.
*/
#include <stdio.h>

#define MAX 50

struct Activity {
    int id;
    int start;
    int finish;
};

int main() {
    struct Activity activities[MAX], temp;
    int n, i, j, lastFinish;

    printf("Activity Selection Problem\n");
    printf("Enter the number of activities: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        activities[i].id = i + 1;
        printf("Enter start time for activity %d: ", i + 1);
        scanf("%d", &activities[i].start);
        printf("Enter finish time for activity %d: ", i + 1);
        scanf("%d", &activities[i].finish);
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (activities[j].finish > activities[j + 1].finish) {
                temp = activities[j];
                activities[j] = activities[j + 1];
                activities[j + 1] = temp;
            }
        }
    }

    printf("\nSelected activities:\n");
    printf("Activity %d (%d, %d)\n", activities[0].id, activities[0].start, activities[0].finish);
    lastFinish = activities[0].finish;

    for (i = 1; i < n; i++) {
        if (activities[i].start >= lastFinish) {
            printf("Activity %d (%d, %d)\n", activities[i].id, activities[i].start, activities[i].finish);
            lastFinish = activities[i].finish;
        }
    }

    return 0;
}

