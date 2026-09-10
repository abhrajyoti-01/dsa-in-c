/*
Weighted Interval Scheduling - DP with Binary Search (Advanced)
--------------------------------------------------------------
PROBLEM DEFINITION:
Select non-overlapping jobs to maximize total profit. Each job has a
start time, finish time, and profit. Unlike activity selection, the
goal is maximum VALUE, not maximum number of jobs.

REAL-LIFE EXAMPLE:
A freelance consultant chooses contracts of different durations and
payments. The earliest-finishing contract need not be the best choice.

DIAGRAM:
  job 1: [1---3)       profit 5
  job 2:   [2-----5)   profit 6
  job 3:         [4---6) profit 5
  job 4:           [5-----8) profit 11
  Choose 2 and 4: finish(2) == start(4), total 17.

PSEUDO CODE:
  Sort jobs by finish time
  p[i] = last earlier job with finish <= start[i] (binary search)
  dp[0] = 0
  For i = 0..n-1:
      take = profit[i] + dp[p[i]+1]
      dp[i+1] = max(dp[i], take)
  Walk backwards: if taking improves dp, select i and jump to p[i]
                  otherwise skip i

SAMPLE INPUT & DRY RUN:
  4
  1 3 5
  2 5 6
  4 6 5
  5 8 11
  Sorted order is unchanged. p = [-1,-1,0,1].
  dp = [0,5,6,10,17]. Reconstruct job 4, then job 2.
  Print original job IDs in chronological order: 2 4.

SPECIAL CASES:
  - Intervals are half-open [start,finish): touching jobs are compatible.
  - Empty input (n=0) or all non-positive profits gives an empty schedule.
  - Require start < finish; zero-length jobs are rejected.
  - Ties skip the current job for a deterministic optimal schedule.
  - n <= 200, times 0..10^9, profits within +/-10^9; use long long totals.

COMPLEXITY:
  O(n log n) time including sorting/searches; O(n) auxiliary storage.

TIPS:
  dp[i] means the best profit among the FIRST i sorted jobs, not a
  schedule that must end at job i. Keep original IDs before sorting.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200
#define VALUE_LIMIT 1000000000LL

typedef struct {
    long long start;
    long long finish;
    long long profit;
    int id;
} Job;

int compareJobs(const void* a, const void* b) {
    const Job* x = a;
    const Job* y = b;
    if (x->finish != y->finish) return x->finish < y->finish ? -1 : 1;
    return (x->id > y->id) - (x->id < y->id);
}

int previousJob(const Job jobs[], int i) {
    int lo = 0;
    int hi = i - 1;
    int answer = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (jobs[mid].finish <= jobs[i].start) {
            answer = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return answer;
}

int main(void) {
    Job jobs[MAX_N];
    long long dp[MAX_N + 1] = {0};
    int previous[MAX_N];
    int selected[MAX_N];
    int count = 0;
    int n;

    printf("Weighted Interval Scheduling\n\n");
    printf("How many jobs (0-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 0 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter jobs as 'start finish profit' (times 0..10^9, profit +/-10^9):\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%lld %lld %lld", &jobs[i].start, &jobs[i].finish,
                  &jobs[i].profit) != 3 || jobs[i].start < 0 ||
            jobs[i].start >= jobs[i].finish || jobs[i].finish > VALUE_LIMIT ||
            jobs[i].profit < -VALUE_LIMIT || jobs[i].profit > VALUE_LIMIT) {
            printf("Invalid job\n");
            return 1;
        }
        jobs[i].id = i + 1;
    }
    qsort(jobs, (size_t)n, sizeof(jobs[0]), compareJobs);

    printf("\nDP trace (sorted by finish time):\n");
    for (int i = 0; i < n; i++) {
        previous[i] = previousJob(jobs, i);
        long long take = jobs[i].profit + dp[previous[i] + 1];
        dp[i + 1] = take > dp[i] ? take : dp[i];
        printf("  Job %d: take=%lld, skip=%lld, best=%lld\n",
               jobs[i].id, take, dp[i], dp[i + 1]);
    }
    for (int i = n - 1; i >= 0;) {
        if (jobs[i].profit + dp[previous[i] + 1] > dp[i]) {
            selected[count++] = jobs[i].id;
            i = previous[i];
        } else {
            i--;
        }
    }
    printf("Maximum profit: %lld\n", dp[n]);
    printf("Selected job IDs:");
    if (count == 0) printf(" (none)");
    for (int i = count - 1; i >= 0; i--) printf(" %d", selected[i]);
    printf("\n");
    return 0;
}
