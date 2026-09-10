/*
Binary Search on the Answer - Koko, Ship Capacity, Split Array (Intermediate -> Advanced)
-----------------------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 875 - Koko Eating Bananas
  LeetCode 1011 - Capacity to Ship Packages Within D Days
  LeetCode 410 - Split Array Largest Sum
  LeetCode 69  - Sqrt(x) (the same shape on numbers)
  HackerRank "Cutting Paper Squares" style feasibility searches

PROBLEM DEFINITION:
Search not over an ARRAY but over the set of possible ANSWERS. If
"is answer x feasible?" is monotone (false...false true...true), the
smallest feasible x can be found with binary search using the
feasibility test as the comparison.

REAL-LIFE EXAMPLE:
Choosing a truck size: a tiny truck needs many trips, a huge truck few
trips. "Can this capacity move everything within d days?" is easy to
simulate; the smallest capacity that works is found by halving the
candidate range instead of testing every size.

DIAGRAM (bananas [3,6,7,11], h = 8 hours):
  speed 1 -> 3+6+7+11 = 27 hours   too slow
  speed 3 -> 1+2+3+4  = 10 hours   too slow
  speed 4 -> 1+2+2+3  =  8 hours   feasible  <-- answer
  monotone: feasible(x) is false below 4 and true from 4 upward
             x: 1 2 3 | 4 5 6 ...
          feas : F F F | T T T ...

PSEUDO CODE:
  hoursNeeded(piles, speed) = sum over piles of ceil(pile / speed)
  lo = 1, hi = max(piles)                // hi is always feasible
  while lo < hi:
      mid = lo + (hi - lo) / 2
      if feasible(mid): hi = mid
      else:             lo = mid + 1
  answer = lo

SAMPLE INPUT & DRY RUN:
  4
  3 6 7 11
  8
  5
  1 2 3 4 5
  6
  Banana problem: speed 4 finishes in 8 hours (1+2+2+3), speed 3 needs
  10 -> minimum speed 4.
  Ship problem: weights 1 2 3 4 5 sum 15 with 6 days allowed. Capacity
  3 packs [1+2][3][4][5] = 4 days <= 6. Capacity 2 packs
  [1][2][3][4][5] = 5 days <= 6. Capacity 1 packs
  [1][2][3][4][5] = 5 days <= 6 as well, so the SMALLEST feasible
  capacity is 1. (A crate of weight 1 always fits in its own day, and
  5 <= 6, so 1 is genuinely optimal - there is no lower candidate.)
  The program prints the days needed for the reported capacity, so the
  boundary is auditable.

SPECIAL CASES:
  - hi = max(pile) is always feasible (one pile per unit time), so the
    invariant "answer lies in [lo, hi]" never breaks.
  - hours < number of piles is INFEASIBLE: Koko can eat at most one
    pile per hour, so h >= n is required. Without that check the
    program would still return max(piles) even though the schedule is
    impossible; the input validation rejects such a request up front.
  - Impossible monotonicity breaks the method: a non-monotone
    feasibility test returns a garbage boundary. Test it separately.
  - Sums use long long: 10^9 values times 1000 items overflow int.
  - Ceiling division (pile + speed - 1) / speed avoids floating point.
  - Items <= 1000, days/hours n..1000 (a budget below the item count
    cannot work), pile sizes 1..10^9.

COMPLEXITY:
  O(n log(max value)) time: log(10^9) ~ 30 feasibility sweeps of O(n);
  O(n) storage.

TIPS:
  The two questions to ask in an interview: "what is the search space?"
  and "is the predicate monotone?" Once both are answered, the code is
  ten lines. Never binary search on a floating-point answer when an
  integer reformulation exists.
*/

#include <stdio.h>

#define MAX_ITEMS 1000
#define MAX_DAYS 1000
#define VALUE_LIMIT 1000000000LL

long long items[MAX_ITEMS];

long long unitsNeeded(const long long a[], int n, long long speed) {
    long long total = 0;
    for (int i = 0; i < n; i++) {
        total += (a[i] + speed - 1) / speed;
    }
    return total;
}

long long daysNeeded(const long long a[], int n, long long capacity) {
    long long days = 1;
    long long load = 0;
    for (int i = 0; i < n; i++) {
        if (load + a[i] <= capacity) {
            load += a[i];
        } else {
            days++;
            load = a[i];
        }
    }
    return days;
}

int main(void) {
    int n;
    long long limit;
    long long best;

    printf("Binary Search on the Answer\n\n");
    printf("Part 1 - eating speed. How many piles (1-%d)? ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d pile sizes (1..10^9):\n", n);
    long long maximum = 0;
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &items[i]) != 1 ||
            items[i] < 1 || items[i] > VALUE_LIMIT) {
            printf("Invalid pile size\n");
            return 1;
        }
        if (items[i] > maximum) maximum = items[i];
    }
    /* One pile per hour is the hard floor: fewer hours than piles is
     * infeasible no matter how fast Koko eats. */
    printf("Hours available (%d-%d): ", n, MAX_DAYS);
    if (scanf("%lld", &limit) != 1 || limit < n || limit > MAX_DAYS) {
        printf("Invalid hour budget: at least %d hours are required "
               "(one pile per hour)\n", n);
        return 1;
    }
    long long lo = 1;
    long long hi = maximum;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (unitsNeeded(items, n, mid) <= limit) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    printf("Minimum eating speed = %lld (hours for that speed = %lld)\n", lo,
           unitsNeeded(items, n, lo));

    printf("\nPart 2 - ship capacity. How many packages (1-%d)? ", MAX_ITEMS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d package weights (1..10^9):\n", n);
    long long total = 0;
    maximum = 0;
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &items[i]) != 1 ||
            items[i] < 1 || items[i] > VALUE_LIMIT) {
            printf("Invalid weight\n");
            return 1;
        }
        total += items[i];
        if (items[i] > maximum) maximum = items[i];
    }
    /* Capacity max(packages) guarantees one package per day at worst,
     * so fewer days than packages can never be satisfied. */
    printf("Days available (%d-%d): ", 1, MAX_DAYS);
    if (scanf("%lld", &limit) != 1 || limit < 1 || limit > MAX_DAYS) {
        printf("Invalid day budget\n");
        return 1;
    }
    lo = maximum;
    hi = total;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (daysNeeded(items, n, mid) <= limit) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    best = lo;
    printf("Minimum ship capacity = %lld (days for that capacity = %lld)\n",
           best, daysNeeded(items, n, best));
    printf("Load per day for the optimal capacity:");
    long long load = 0;
    for (int i = 0; i < n; i++) {
        if (load + items[i] <= best) {
            load += items[i];
        } else {
            printf(" %lld", load);
            load = items[i];
            limit--;
        }
    }
    printf(" %lld\n", load);
    return 0;
}
