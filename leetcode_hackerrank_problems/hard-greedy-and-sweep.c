/*
Hard Greedy & Sweep - Skyline, Task Scheduler, Remove K Digits (Advanced)
-------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 218 - The Skyline Problem (sweep line over enter/leave events)
  LeetCode 621 - Task Scheduler (forced idle gaps from the greedy layout)
  LeetCode 402 - Remove K Digits (monotone stack -> smallest number)
  LeetCode 316 - Remove Duplicate Letters (the same monotone stack idea)
  related: HackerRank "Greedy for Maximum Minimum" - the same
           sort-then-scan family already solved in
           hackerrank-sorting-classics.c.

PROBLEM DEFINITION:
Two sweeps and one monotone stack. Skyline turns each building into a
left-edge enter event and a right-edge leave event, sorts them, sweeps
in x order and records a key whenever the current MAXIMUM height
changes. Task Scheduler arranges the most frequent task first and counts
the idle gaps that arrangement forces. Remove K Digits keeps a stack of
digits that only ever grows onto a smaller-or-equal top, deleting the
biggest digits as early as possible.

REAL-LIFE EXAMPLE:
City planning renders a skyline from footprints, a CPU or job scheduler
has to insert cooldown gaps between identical jobs, and a pricing page
shows the cheapest code after deleting a few digits.

DIAGRAM (skyline of [2,9,10] [3,7,15] [5,12,12] [15,20,10] [19,24,8]):
  each building is [left, right, height], so the events are
  (2,+10) (9,-10) (3,+15) (7,-15) (5,+12) (12,-12) (15,+10) (20,-10)
  (19,+8) (24,-8); sorted by x with enters before leaves:
  events: (2,+10) (3,+15) (5,+12) (7,-15) (9,-10) (12,-12) (15,+10)
          (19,+8) (20,-10) (24,-8)
  sweep, tracking the current maximum:
     x=2  +10 -> max 10   key (2,10)
     x=3  +15 -> max 15   key (3,15)
     x=5  +12 -> max 15   no change (12 is lower)
     x=7  -15 -> max 12   key (7,12)
     x=9  -10 -> max 12   no change
     x=12 -12 -> max 0    key (12,0)
     x=15 +10 -> max 10   key (15,10)
     x=19 +8  -> max 10   no change (8 is lower)
     x=20 -10 -> max 8    key (20,8)
     x=24 -8  -> max 0    key (24,0)
  => (2,10) (3,15) (7,12) (12,0) (15,10) (20,8) (24,0)

PSEUDO CODE:
  Skyline(buildings):
      events = [(left, +h), (right, -h)] for every building
      sort by x, and for the same x process ENTER before LEAVE
      active = max-multiset of heights
      for each event: add or remove the height; if the maximum changed
          emit (x, newMaximum)
  TaskScheduler(tasks, cooldown):
      maxFreq = highest task frequency
      countMax = how many tasks share maxFreq
      answer = max(number of tasks, (maxFreq - 1) * (cooldown + 1)
                                       + countMax)
  RemoveKDigits(number, k):
      stack = []
      for each digit d:
          while k > 0 and stack not empty and top > d: pop; k--
          push d
      while k > 0: pop the tail; k--          (digits still too many)
      strip leading zeros; empty result -> "0"

SAMPLE INPUT & DRY RUN:
  5
  2 9 10
  3 7 15
  5 12 12
  15 20 10
  19 24 8
  AAABBB
  2
  1432219
  3
  The order is fixed: buildings, then tasks, then the cooldown, then ONE
  number and how many of its digits to remove.
  Skyline: (2,10) (3,15) (7,12) (12,0) (15,10) (20,8) (24,0).
  Task scheduler for AAABBB with cooldown 2: A and B both appear 3 times,
  so the forced layout is A B _ A B _ A B -> 8 slots (the 6 tasks alone
  would need 6, but the cooldown stretches it to 8).
  "1432219" remove 3 -> "1219" (drop 4, 3 and 2 at the right moments).
  A second hand-check you can type in instead: "10200" remove 1 -> "200"
  (the leading 0 is stripped).

SPECIAL CASES:
  - Same-x events: an enter at x must be seen BEFORE a leave at x, or a
    skyline step of the correct height is missed and the output has a
    phantom zero gap. State the tie-break explicitly.
  - Equal heights must not emit duplicate keys: compare the new maximum
    with the previous one and emit only on a change.
  - The city floor is height 0, so the last event always drops back to
    0; a skyline that never returns to 0 is a bug.
  - Task Scheduler: when the idle formula is smaller than the task count
    the tasks themselves fill the gaps, hence the max() - for AAABBB with
    cooldown 0 the layout needs only 4 slots, so the answer falls back to
    the 6 tasks themselves (6), while cooldown 2 forces 10 slots.
  - Remove K Digits: k may exceed the digit count only in malformed
    input; k == number of digits leaves "0", and leading zeros are
    stripped AFTER the removals (not before), or "10200" breaks.
  - Buildings/events are limited to 100, heights and counts fit in
    int, task letters are 'A'..'Z', digits are 0-9.

COMPLEXITY:
  Skyline: O(n log n) sorting plus O(n log n) for the ordered max
  (this program uses a sorted active list, which is enough at n <= 100).
  Task Scheduler: O(n) counting. Remove K Digits: O(d) with a stack of
  at most d digits. All three are linear or linearithmic in the input.

TIPS:
  Sweep-line answers have three parts: the EVENT type (enter/leave), the
  tie-break at equal x, and the STATE that summarises "what is active
  now". Write those three down before coding and the rest is bookkeeping.
  For "make the number smallest by deleting digits", the greedy is local
  (delete while the next digit is smaller) - the stack just remembers
  where to resume.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUILDINGS 100
#define MAX_EVENTS 200
#define MAX_TASKS 100
#define MAX_DIGITS 100

typedef struct {
    long long x;
    long long height;
    int enter;                  /* 1 = left edge (enter), 0 = right edge */
} Event;

Event events[MAX_EVENTS];
long long activeHeights[MAX_EVENTS];    /* sorted ascending, like a multiset */
int activeCount;
char tasks[MAX_TASKS + 1];
char digits[MAX_DIGITS + 1];
char digitStack[MAX_DIGITS + 1];

/* Tie-break: process a left edge BEFORE a right edge at the same x, or a
 * skyline step of the correct height is lost. */
int compareEvents(const void* a, const void* b) {
    const Event* ea = (const Event*)a;
    const Event* eb = (const Event*)b;
    if (ea->x != eb->x) return ea->x < eb->x ? -1 : 1;
    if (ea->enter != eb->enter) return eb->enter - ea->enter;
    return 0;
}

void activeAdd(long long height) {
    int i = activeCount;
    while (i > 0 && activeHeights[i - 1] > height) {
        activeHeights[i] = activeHeights[i - 1];
        i--;
    }
    activeHeights[i] = height;
    activeCount++;
}

void activeRemove(long long height) {
    int i = 0;
    while (i < activeCount && activeHeights[i] != height) i++;
    if (i == activeCount) return;
    for (int k = i; k + 1 < activeCount; k++) activeHeights[k] = activeHeights[k + 1];
    activeCount--;
}

long long activeMax(void) {
    return activeCount == 0 ? 0 : activeHeights[activeCount - 1];
}

/* LeetCode 218: sweep the sorted events and emit a key ONLY when the
 * maximum active height actually changes. */
void skyline(int eventCount) {
    qsort(events, (size_t)eventCount, sizeof(events[0]), compareEvents);
    long long previousMax = 0;
    printf("Skyline:");
    for (int i = 0; i < eventCount; i++) {
        if (events[i].enter) activeAdd(events[i].height);
        else activeRemove(events[i].height);
        long long current = activeMax();
        if (current != previousMax) {
            printf(" (%lld,%lld)", events[i].x, current);
            previousMax = current;
        }
    }
    printf("\n");
}

/* LeetCode 621: the most frequent task defines the gaps; the answer is
 * whichever is larger, the forced layout or the task count itself. */
int taskScheduler(int cooldown) {
    long long frequency[26];
    for (int i = 0; i < 26; i++) frequency[i] = 0;
    int total = 0;
    for (int i = 0; tasks[i] != '\0'; i++) {
        frequency[tasks[i] - 'A']++;
        total++;
    }
    long long maxFreq = 0;
    int countMax = 0;
    for (int i = 0; i < 26; i++) {
        if (frequency[i] > maxFreq) maxFreq = frequency[i];
    }
    for (int i = 0; i < 26; i++) {
        if (frequency[i] == maxFreq) countMax++;
    }
    long long greedyLayout = (maxFreq - 1) * (cooldown + 1) + countMax;
    long long answer = greedyLayout > total ? greedyLayout : total;
    printf("Most frequent task appears %lld times; %d task(s) share that "
           "count\n", maxFreq, countMax);
    printf("Forced layout needs %lld slots, the %d tasks need %d -> ",
           greedyLayout, total, total);
    return (int)answer;
}

/* LeetCode 402: keep a monotone non-decreasing stack; a digit that is
 * smaller than the top lets us drop a bigger earlier digit. */
void removeKDigits(const char* number, int k, char* out) {
    int length = (int)strlen(number);
    int top = -1;
    for (int i = 0; i < length; i++) {
        while (k > 0 && top >= 0 && digitStack[top] > number[i]) {
            top--;
            k--;
        }
        digitStack[++top] = number[i];
    }
    while (k > 0 && top >= 0) {          /* removed too few so far */
        top--;
        k--;
    }
    int start = 0;
    while (start <= top && digitStack[start] == '0') start++;
    int outLength = 0;
    for (int i = start; i <= top; i++) out[outLength++] = digitStack[i];
    if (outLength == 0) out[outLength++] = '0';
    out[outLength] = '\0';
}

int main(void) {
    char result[MAX_DIGITS + 1];

    printf("Hard Greedy & Sweep - Skyline, Task Scheduler, Remove K Digits\n\n");

    int buildingCount;
    printf("--- LeetCode 218: skyline ---\n");
    printf("How many buildings (1-%d)? ", MAX_BUILDINGS);
    if (scanf("%d", &buildingCount) != 1 || buildingCount < 1 ||
        buildingCount > MAX_BUILDINGS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d buildings as 'left right height' (left < right, "
           "height > 0):\n", buildingCount);
    for (int i = 0; i < buildingCount; i++) {
        long long left;
        long long right;
        long long height;
        if (scanf("%lld %lld %lld", &left, &right, &height) != 3 ||
            left >= right || height <= 0 || height > 1000000000LL) {
            printf("Invalid building\n");
            return 1;
        }
        events[2 * i].x = left;
        events[2 * i].height = height;
        events[2 * i].enter = 1;
        events[2 * i + 1].x = right;
        events[2 * i + 1].height = height;
        events[2 * i + 1].enter = 0;
    }
    activeCount = 0;
    skyline(2 * buildingCount);

    int cooldown;
    printf("\n--- LeetCode 621: task scheduler ---\n");
    printf("Enter the tasks as capital letters (1-%d, e.g. AAABBB): ",
           MAX_TASKS);
    if (scanf("%100s", tasks) != 1) {
        printf("Invalid tasks\n");
        return 1;
    }
    int taskLength = (int)strlen(tasks);
    if (taskLength > MAX_TASKS) {
        printf("Too many tasks\n");
        return 1;
    }
    for (int i = 0; i < taskLength; i++) {
        if (tasks[i] < 'A' || tasks[i] > 'Z') {
            printf("Invalid task letter '%c' (use A..Z)\n", tasks[i]);
            return 1;
        }
    }
    printf("Cooldown n (0-100): ");
    if (scanf("%d", &cooldown) != 1 || cooldown < 0 || cooldown > 100) {
        printf("Invalid cooldown\n");
        return 1;
    }
    printf("Minimum intervals = %d\n", taskScheduler(cooldown));

    int k;
    printf("\n--- LeetCode 402: remove k digits ---\n");
    printf("Enter a number (1-%d digits): ", MAX_DIGITS);
    if (scanf("%100s", digits) != 1) {
        printf("Invalid number\n");
        return 1;
    }
    int length = (int)strlen(digits);
    for (int i = 0; i < length; i++) {
        if (digits[i] < '0' || digits[i] > '9') {
            printf("Invalid digit '%c'\n", digits[i]);
            return 1;
        }
    }
    printf("How many digits to remove (0-%d)? ", length);
    if (scanf("%d", &k) != 1 || k < 0 || k > length) {
        printf("Invalid k (must be 0..%d)\n", length);
        return 1;
    }
    removeKDigits(digits, k, result);
    printf("Smallest number after removing %d digit(s): %s\n", k, result);
    return 0;
}
