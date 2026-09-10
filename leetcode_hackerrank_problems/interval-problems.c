/*
Interval Problems - Meeting Rooms, Merge, Insert, Erase (Intermediate -> Advanced)
----------------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 56   - Merge Intervals
  LeetCode 57   - Insert Interval
  LeetCode 252  - Meeting Rooms (can one person attend all?)
  LeetCode 253  - Meeting Rooms II (minimum rooms needed)
  LeetCode 1288 - Remove Covered Intervals
  HackerRank "Interval" scheduling drills

PROBLEM DEFINITION:
Given a set of half-open intervals [start, end), answer the four
questions every calendar app implements: merge the overlaps, insert a
new interval, test whether one room suffices, and count the minimum
rooms needed. Sorting by start (and sweeping with a heap-free method)
answers all four.

REAL-LIFE EXAMPLE:
A conference-room booking system: merging back-to-back bookings into
one busy block, inserting a last-minute meeting, and reporting how
many rooms the day actually needs.

DIAGRAM (intervals [1,3) [2,6) [8,10) [9,12)):
  sorted by start: 1-3  2-6  8-10  9-12
  merge sweep:   [1,3) overlaps [2,6) -> merge to [1,6)
                 [8,10) overlaps [9,12) -> merge to [8,12)
  result: [1,6) [8,12)   -> 2 merged blocks
  minimum rooms: +1 at 1, +1 at 2 (now 2), -1 at 3 (1), -1 at 6 (0),
                 +1 at 8, +1 at 9 (2), -1 at 10, -1 at 12 -> peak 2 rooms

PSEUDO CODE:
  Sort by start, then by end
  Merge: keep a current block; if next.start < current.end extend
         current.end = max(current.end, next.end), else flush and start
  Insert: append the new interval, re-sort, then merge (simple and safe)
  OneRoom: one room suffices exactly when NOTHING overlaps. After the
           merge sweep that is simply mergedCount == n (no two blocks
           were joined). Checking mergedCount == 1 is WRONG: two
           disjoint bookings like [1,3) [5,7) merge into two blocks yet
           one room still attends both.
  MinRooms: collect starts and ends arrays, sort each independently,
            sweep: if next start < current end -> rooms++ and start++,
            else end++ and rooms stays; track the maximum

SAMPLE INPUT & DRY RUN:
  4
  1 3
  2 6
  8 10
  9 12
  5 7
  Merged: [1,6) [8,12). Inserting [5,7) gives [1,7) [8,12).
  One room cannot hold all (2 and 8 are busy blocks) -> answer no.
  Minimum rooms = 2 (trace in the diagram above).

SPECIAL CASES:
  - Half-open intervals make touching compatible: [1,3) and [3,5) do
    NOT overlap and CAN share a room. Choose a convention and state it.
  - Identical intervals merge into one.
  - Interval fully containing another is absorbed by merge and counts
    as "covered" for the LeetCode 1288 style count.
  - LeetCode 1288 sorts by start ASCENDING and end DESCENDING, so that
    two intervals sharing a start place the WIDER one first. With
    ascending ends, [1,5) and [1,3) would print "0 covered" although
    [1,3) is covered by [1,5) - the reason for the descending tie-break.
  - One room is enough for the sample? No: [1,6) and [8,12) are two
    separate busy blocks, so the answer is "no" even though the block
    count is 2 - the count alone never decides the room question.
  - start must be < end (zero-length meetings are rejected).
  - Intervals <= 1000, times 0..10^9.

COMPLEXITY:
  Sorting dominates: O(n log n) time, O(n) extra space; both sweeps are
  a single linear pass after the sort.

TIPS:
  The "starts and ends sorted separately" trick counts maximum overlap
  in O(n log n) WITHOUT a heap. A min-heap of end times solves the
  same problem in the streaming version - see 07-trees-and-heaps/heap.c.
  LeetCode 1288 counts intervals not covered by any other; sorting by
  start ascending and end descending makes the greedy comparison local:
  once the widest interval with the earliest start has recorded the
  running maximum end, every later interval ending at or before it is
  covered.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_INTERVALS 1000

typedef struct {
    long long start;
    long long end;
} Interval;

Interval intervals[MAX_INTERVALS + 1];
Interval merged[MAX_INTERVALS + 1];
long long starts[MAX_INTERVALS + 1];
long long ends[MAX_INTERVALS + 1];

int compareIntervals(const void* a, const void* b) {
    const Interval* x = a;
    const Interval* y = b;
    if (x->start != y->start) return x->start < y->start ? -1 : 1;
    /* Equal starts: the WIDER interval first (descending end) so that
     * LeetCode 1288 can mark the narrower one as covered. Merging stays
     * correct because it keeps the maximum end anyway. */
    if (x->end != y->end) return x->end > y->end ? -1 : 1;
    return 0;
}

int compareLongs(const void* a, const void* b) {
    long long x = *(const long long*)a;
    long long y = *(const long long*)b;
    return (x > y) - (x < y);
}

int mergeIntervals(int n) {
    qsort(intervals, (size_t)n, sizeof(intervals[0]), compareIntervals);
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (count > 0 && intervals[i].start < merged[count - 1].end) {
            if (intervals[i].end > merged[count - 1].end) {
                merged[count - 1].end = intervals[i].end;
            }
        } else {
            merged[count++] = intervals[i];
        }
    }
    return count;
}

int minimumRooms(int n) {
    for (int i = 0; i < n; i++) {
        starts[i] = intervals[i].start;
        ends[i] = intervals[i].end;
    }
    qsort(starts, (size_t)n, sizeof(starts[0]), compareLongs);
    qsort(ends, (size_t)n, sizeof(ends[0]), compareLongs);
    int rooms = 0;
    int best = 0;
    int i = 0;
    int j = 0;
    while (i < n) {
        if (starts[i] < ends[j]) {
            rooms++;
            if (rooms > best) best = rooms;
            i++;
        } else {
            rooms--;
            j++;
        }
    }
    return best;
}

int countCovered(int n) {
    qsort(intervals, (size_t)n, sizeof(intervals[0]), compareIntervals);
    int covered = 0;
    long long maxEnd = -1;
    int hasMax = 0;
    for (int i = 0; i < n; i++) {
        if (hasMax && intervals[i].end <= maxEnd) {
            covered++;
        } else {
            maxEnd = intervals[i].end;
            hasMax = 1;
        }
    }
    return covered;
}

int main(void) {
    int n;

    printf("Interval Problems - Merge, Insert, Rooms, Covered\n\n");
    printf("How many intervals (1-%d)? ", MAX_INTERVALS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_INTERVALS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d intervals as 'start end' (half-open, 0 <= start < end):\n",
           n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld %lld", &intervals[i].start, &intervals[i].end) != 2 ||
            intervals[i].start < 0 ||
            intervals[i].start >= intervals[i].end ||
            intervals[i].end > 1000000000LL) {
            printf("Invalid interval\n");
            return 1;
        }
    }

    int mergedCount = mergeIntervals(n);
    printf("\nMerged intervals (%d):", mergedCount);
    for (int i = 0; i < mergedCount; i++) {
        printf(" [%lld,%lld)", merged[i].start, merged[i].end);
    }
    /* One room is enough exactly when no two intervals overlapped, i.e.
     * the merge sweep joined nothing and produced n blocks. */
    printf("\nOne room is enough: %s",
           mergedCount == n ? "yes (no two intervals overlap)" : "no");
    if (mergedCount != n) {
        printf(" - the %d intervals overlap and collapse into %d busy "
               "block(s), so at least 2 rooms are needed",
               n, mergedCount);
    }
    printf("\n");
    printf("Minimum rooms needed: %d\n", minimumRooms(n));
    printf("Covered intervals (LeetCode 1288): %d\n", countCovered(n));

    long long newStart;
    long long newEnd;
    printf("\nInsert an interval: start end (or 0 0 to stop): ");
    if (scanf("%lld %lld", &newStart, &newEnd) != 2) {
        printf("Invalid interval\n");
        return 1;
    }
    if (newStart != 0 || newEnd != 0) {
        if (newStart < 0 || newStart >= newEnd || newEnd > 1000000000LL) {
            printf("Invalid interval\n");
            return 1;
        }
        intervals[n].start = newStart;
        intervals[n].end = newEnd;
        int total = mergeIntervals(n + 1);
        printf("After inserting [%lld,%lld) -> %d intervals:", newStart, newEnd,
               total);
        for (int i = 0; i < total; i++) {
            printf(" [%lld,%lld)", merged[i].start, merged[i].end);
        }
        printf("\n");
    }
    return 0;
}
