/*
HackerRank & LeetCode Sorting Classics (Basic -> Advanced)
----------------------------------------------------------
PLATFORM PROBLEMS:
  HackerRank "Big Sorting"                  - sort numeric strings
  HackerRank "Mark and Toys"                - maximize items in budget
  HackerRank "Maximum Perimeter Triangle"   - greedy + feasibility scan
  HackerRank "Priyanka and Toys"            - count containers by range
  LeetCode 435   - Non-overlapping Intervals
  LeetCode 1029  - Two City Scheduling
  LeetCode 1710  - Maximum Units on a Truck

PROBLEM DEFINITION:
Six platform staples that look unrelated but share one idea: sort by
the RIGHT key, then make one greedy pass. The sort is the algorithm;
the pass only reads.

REAL-LIFE EXAMPLE:
Shipping: pack the heaviest-value items per box first (truck), buy the
cheapest components to maximize part count (toys), group devices whose
weights differ by at most 4 into the fewest bins (toys/groups).

DIAGRAM (two-city scheduling - sort by price difference):
  costs A/B: (10,20) (30,200) (400,50) (30,20)
  difference A - B: -10, -170, +350, +10
  sorted ascending: -170, -10, +10, +350
  the cheapest-to-city-A half goes to A, the rest to B
  sorted pairs: (30,200) (10,20) (30,20) (400,50)
  two to A: 30 + 10 = 40      two to B: 20 + 50 = 70
  total = 40 + 70 = 110       (optimal for n = 2 people per city)

PSEUDO CODE:
  BigSort: sort strings by (length, lexicographic) - never parse
  MarkToys: sort prices ascending, buy while budget allows
  MaxPerimeterTriangle: sort ascending; for i from the top, the best
      triple is (a[i-2], a[i-1], a[i]) if a[i-2] + a[i-1] > a[i]
  Priyanka: sort weights; each group covers [w, w + 4] inclusively
  NonOverlapping: sort by END, take every interval that starts at or
      after the last accepted end
  TwoCity: sort by (costA - costB); the first half goes to A, the rest
      to B (the people count must be EVEN; a negative difference means
      city A is the cheaper choice for that person)
  MaxUnits: sort by units descending, fill boxes greedily

SAMPLE INPUT & DRY RUN:
  5                <- Big Sorting: five numeric strings
  8 100 10 9 1000
  3                <- Mark and Toys
  1 2 4
  5                <- budget
  5                <- maximum perimeter triangle
  30 40 50 60 70
  4                <- Priyanka
  1 2 3 4
  3                <- non-overlapping intervals
  1 3
  2 4
  3 5
  4                <- two-city: four people
  10 20
  30 200
  400 50
  30 20
  3                <- truck: three box types
  10 5
  5 10
  7 2
  12               <- truck size in boxes
  BigSort: 8 9 10 100 1000 (length before lexicographic).
  Mark and Toys with budget 5 and prices 1 2 4 -> 1 + 2 = 3 (2 toys);
  adding 4 costs 7 > 5 -> answer 2.
  Maximum perimeter triangle from 30 40 50 60 70 -> 50 + 60 + 70 = 180
  (50 + 60 > 70).
  Priyanka with weights 1 2 3 4 -> one container [1..5] covers all 4.
  Non-overlapping [1,3) [2,4) [3,5): sorted by end, keep [1,3) and
  [3,5) (touching is compatible) -> kept 2, removed 1.
  TwoCity -> 110 (see the diagram). Truck: units 10*5 = 50,
  7*2 = 14, 5*5 = 25 -> 89 units from 12 boxes.

SPECIAL CASES:
  - BigSort must compare by LENGTH first: "9" < "10" as numbers, but as
    strings "10" < "9". Parsing into long long breaks on 10^5 digits.
  - BigSort leading zeros: "007" and "7" are numerically equal but
    compare as different strings. This program REJECTS any digit string
    with a leading zero (except the single character "0") so that the
    (length, lexicographic) ordering is exactly numeric ordering.
  - Mark and Toys with an item costing more than the budget: skip it and
    keep buying cheaper ones (a later item may still fit).
  - Triangle degeneracy: a + b == c is NOT a triangle (needs >).
  - Non-overlapping uses half-open half-closed logic: touching ends are
    compatible, so prev.end <= next.start.
  - TwoCity requires an EVEN number of people (half fly to each city);
    an odd count is rejected rather than silently mis-split.
  - Truck sizes and box counts can exceed 32 bits, so every accumulation
    uses long long.
  - n <= 1000 per part, prices/weights within 0..10^9.

COMPLEXITY:
  Every part is O(n log n) sorting (qsort) plus O(n) scanning; O(n)
  storage. The paired sorts compare through an index array so that two
  fields stay together without hand-written quadratic sorts.

TIPS:
  This file is a tour of "sort by the key the greedy step needs". Write
  the key on paper first (end time, price difference, units, weight),
  then the code writes itself. Exchange-argument proofs are the reason
  each greedy choice is safe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 1000
#define MAX_DIGITS 64

long long values[MAX_ITEMS];
long long costA[MAX_ITEMS];
long long costB[MAX_ITEMS];
char numbers[MAX_ITEMS][MAX_DIGITS];
int order[MAX_ITEMS];

/* Pair sort used by TwoCity: compare by (costA - costB). */
int compareCityPairs(const void* a, const void* b) {
    int i = *(const int*)a;
    int j = *(const int*)b;
    long long differenceI = costA[i] - costB[i];
    long long differenceJ = costA[j] - costB[j];
    if (differenceI != differenceJ) {
        return differenceI < differenceJ ? -1 : 1;
    }
    return i - j;                     /* stable, deterministic */
}

/* Pair sort used by MaxUnits: compare by units per box, descending. */
int compareTruckPairs(const void* a, const void* b) {
    int i = *(const int*)a;
    int j = *(const int*)b;
    if (values[i] != values[j]) return values[i] > values[j] ? -1 : 1;
    return i - j;
}

/* Pair sort used by NonOverlapping: compare by interval END. */
long long endOf[MAX_ITEMS];
int compareIntervalEnds(const void* a, const void* b) {
    int i = *(const int*)a;
    int j = *(const int*)b;
    if (endOf[i] != endOf[j]) return endOf[i] < endOf[j] ? -1 : 1;
    return i - j;
}

int compareLongs(const void* a, const void* b) {
    long long x = *(const long long*)a;
    long long y = *(const long long*)b;
    return (x > y) - (x < y);
}

int compareLongsDescending(const void* a, const void* b) {
    return -compareLongs(a, b);
}

int compareBigNumbers(const void* a, const void* b) {
    const char* x = a;
    const char* y = b;
    size_t lenX = strlen(x);
    size_t lenY = strlen(y);
    if (lenX != lenY) return lenX < lenY ? -1 : 1;
    return strcmp(x, y);
}

int main(void) {
    int n;

    printf("HackerRank & LeetCode Sorting Classics\n\n");

    printf("== Big Sorting: how many numeric strings (1-%d)? ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d digit strings (length 1..%d):\n", n, MAX_DIGITS - 1);
    for (int i = 0; i < n; i++) {
        if (scanf("%63s", numbers[i]) != 1 || numbers[i][0] == '\0') {
            printf("Invalid number\n");
            return 1;
        }
        for (int j = 0; numbers[i][j] != '\0'; j++) {
            if (numbers[i][j] < '0' || numbers[i][j] > '9') {
                printf("Invalid digit string\n");
                return 1;
            }
        }
        /* Leading zeros would break the (length, lexicographic) order:
         * "007" and "7" are numerically equal yet sort apart. */
        if ((int)strlen(numbers[i]) > 1 && numbers[i][0] == '0') {
            printf("Invalid number \"%s\": leading zeros are not allowed ",
                   numbers[i]);
            printf("(use \"0\" for zero)\n");
            return 1;
        }
    }
    qsort(numbers, (size_t)n, sizeof(numbers[0]), compareBigNumbers);
    printf("Sorted:");
    for (int i = 0; i < n; i++) printf(" %s", numbers[i]);
    printf("\n");

    printf("\n== Mark and Toys: how many prices (1-%d)? ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d prices (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 || values[i] < 0 ||
            values[i] > 1000000000LL) {
            printf("Invalid price\n");
            return 1;
        }
    }
    long long budget;
    printf("Budget (0..10^12): ");
    if (scanf("%lld", &budget) != 1 || budget < 0 ||
        budget > 1000000000000LL) {
        printf("Invalid budget\n");
        return 1;
    }
    qsort(values, (size_t)n, sizeof(values[0]), compareLongs);
    long long spent = 0;
    int bought = 0;
    for (int i = 0; i < n && spent + values[i] <= budget; i++) {
        spent += values[i];
        bought++;
    }
    printf("Toys bought = %d, total spent = %lld\n", bought, spent);

    printf("\n== Maximum perimeter triangle with n sticks (3-%d): ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 3 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d side lengths (1..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 || values[i] < 1 ||
            values[i] > 1000000000LL) {
            printf("Invalid length\n");
            return 1;
        }
    }
    qsort(values, (size_t)n, sizeof(values[0]), compareLongs);
    int foundTriangle = 0;
    for (int i = n - 1; i >= 2; i--) {
        if (values[i - 2] + values[i - 1] > values[i]) {
            printf("Best non-degenerate triangle: %lld %lld %lld (perimeter %lld)\n",
                   values[i - 2], values[i - 1], values[i],
                   values[i - 2] + values[i - 1] + values[i]);
            foundTriangle = 1;
            break;
        }
    }
    if (!foundTriangle) {
        printf("No non-degenerate triangle exists\n");
    }

    printf("\n== Priyanka and Toys: how many weights (1-%d)? ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d weights (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 || values[i] < 0 ||
            values[i] > 1000000000LL) {
            printf("Invalid weight\n");
            return 1;
        }
    }
    qsort(values, (size_t)n, sizeof(values[0]), compareLongs);
    int containers = 0;
    for (int i = 0; i < n;) {
        long long limit = values[i] + 4;
        containers++;
        while (i < n && values[i] <= limit) i++;
    }
    printf("Containers needed = %d\n", containers);

    printf("\n== Non-overlapping intervals: how many intervals (1-%d)? ",
           MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d intervals as 'start end' (0 <= start < end):\n", n);
    long long* intervalStart = malloc(sizeof(long long) * (size_t)n);
    long long* intervalEnd = malloc(sizeof(long long) * (size_t)n);
    if (intervalStart == NULL || intervalEnd == NULL) {
        printf("Out of memory\n");
        free(intervalStart);
        free(intervalEnd);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (scanf("%lld %lld", &intervalStart[i], &intervalEnd[i]) != 2 ||
            intervalStart[i] < 0 || intervalStart[i] >= intervalEnd[i] ||
            intervalEnd[i] > 1000000000LL) {
            printf("Invalid interval\n");
            free(intervalStart);
            free(intervalEnd);
            return 1;
        }
        endOf[i] = intervalEnd[i];
        order[i] = i;
    }
    qsort(order, (size_t)n, sizeof(order[0]), compareIntervalEnds);
    int kept = 0;
    long long lastEnd = -1;
    for (int i = 0; i < n; i++) {
        int index = order[i];
        if (kept == 0 || intervalStart[index] >= lastEnd) {
            kept++;
            lastEnd = intervalEnd[index];
        }
    }
    printf("Intervals kept = %d, removed = %d\n", kept, n - kept);
    free(intervalStart);
    free(intervalEnd);

    printf("\n== Two-city scheduling: how many people (2-%d, must be even)? ",
           MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 2 || n > MAX_ITEMS || n % 2 != 0) {
        printf("Invalid count: the number of people must be EVEN so that\n");
        printf("exactly n/2 fly to each city\n");
        return 1;
    }
    printf("Enter %d lines of 'costA costB' (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld %lld", &costA[i], &costB[i]) != 2 || costA[i] < 0 ||
            costA[i] > 1000000000LL || costB[i] < 0 ||
            costB[i] > 1000000000LL) {
            printf("Invalid cost\n");
            return 1;
        }
        order[i] = i;
    }
    qsort(order, (size_t)n, sizeof(order[0]), compareCityPairs);
    long long total = 0;
    for (int i = 0; i < n; i++) {
        int index = order[i];
        total += (i < n / 2) ? costA[index] : costB[index];
    }
    printf("Minimum total cost (%d to city A, %d to city B) = %lld\n",
           n / 2, n / 2, total);

    printf("\n== Maximum units on a truck: how many box types (1-%d)? ",
           MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d lines of 'unitsPerBox boxCount' (units 1..1000, ", n);
    printf("boxes 1..10^9):\n");
    for (int i = 0; i < n; i++) {
        long long boxes;
        if (scanf("%lld %lld", &values[i], &boxes) != 2 || values[i] < 1 ||
            values[i] > 1000 || boxes < 1 || boxes > 1000000000LL) {
            printf("Invalid box description\n");
            return 1;
        }
        costA[i] = boxes;
        order[i] = i;
    }
    long long truckSize;
    printf("Truck size in boxes (1..10^12): ");
    if (scanf("%lld", &truckSize) != 1 || truckSize < 1 ||
        truckSize > 1000000000000LL) {
        printf("Invalid truck size\n");
        return 1;
    }
    qsort(order, (size_t)n, sizeof(order[0]), compareTruckPairs);
    long long loaded = 0;
    long long units = 0;
    for (int i = 0; i < n && loaded < truckSize; i++) {
        int index = order[i];
        long long take = costA[index];
        if (take > truckSize - loaded) take = truckSize - loaded;
        units += take * values[index];
        loaded += take;
    }
    printf("Total units loaded = %lld (boxes loaded = %lld)\n", units, loaded);
    printf("\nDone: every part sorted by its own greedy key.\n");
    return 0;
}
