/*
Minimum-Cost Assignment - Bitmask Dynamic Programming (Master)
--------------------------------------------------------------
PROBLEM DEFINITION:
Assign n workers to n jobs, one job per worker and one worker per job,
minimizing total cost. Print both the optimal cost and an assignment.

REAL-LIFE EXAMPLE:
Different technicians take different amounts of time on each repair.
Choose who performs each task to minimize the total working time.

DIAGRAM (mask tracks assigned JOBS, workers assigned in order):
  costs:      job0 job1 job2
    worker0     9    2    7
    worker1     6    4    3
    worker2     5    8    1
  000 --worker0/job1--> 010 --worker1/job0--> 011 --worker2/job2--> 111
              +2                    +6                    +1 = 9

PSEUDO CODE:
  dp[0] = 0; other states = INF
  For mask = 0..(1<<n)-1:
      worker = number of set bits in mask
      For each unused job:
          next = mask | (1<<job)
          Relax dp[next] using dp[mask] + cost[worker][job]
          Save the chosen job for next when improved
  Walk backwards from full mask, clearing each saved job bit

SAMPLE INPUT & DRY RUN:
  3
  9 2 7
  6 4 3
  5 8 1
  dp[010]=2; dp[011]=8 via worker1/job0; dp[111]=9 via worker2/job2.
  Optimal assignment: worker0->job1, worker1->job0, worker2->job2.

SPECIAL CASES:
  - n=1 assigns the only job, even if its cost is negative.
  - Negative/zero costs are valid; every job must still be assigned.
  - Multiple optima: strict improvements keep the first optimum found.
  - n is 1..16; each cost within +/-10^9. All pairs are allowed (no
    special "unavailable" value); long long safely holds the total.
  - Empty assignment is not accepted by this input-driven lesson.

COMPLEXITY:
  O(n * 2^n) time; O(2^n + n^2) space, including the cost matrix.

TIPS:
  The worker index is implicit in popcount(mask), avoiding an extra DP
  dimension. For larger n, study the polynomial-time Hungarian algorithm;
  this lesson demonstrates the general subset-DP pattern.
*/

#include <stdio.h>
#include <limits.h>

#define MAX_N 16
#define MAX_MASK (1 << MAX_N)
#define VALUE_LIMIT 1000000000LL
#define INF (LLONG_MAX / 4)

long long cost[MAX_N][MAX_N];
long long dp[MAX_MASK];
int chosenJob[MAX_MASK];
int bitCount[MAX_MASK];

int main(void) {
    int n;
    int assignment[MAX_N];
    printf("Minimum-Cost Assignment - Bitmask DP\n\n");
    printf("How many workers/jobs (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter the %dx%d cost matrix (within +/-10^9):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%lld", &cost[i][j]) != 1 ||
                cost[i][j] < -VALUE_LIMIT || cost[i][j] > VALUE_LIMIT) {
                printf("Invalid cost\n");
                return 1;
            }
        }
    }
    int full = (1 << n) - 1;
    for (int mask = 1; mask <= full; mask++) {
        dp[mask] = INF;
        bitCount[mask] = bitCount[mask >> 1] + (mask & 1);
    }
    for (int mask = 0; mask < full; mask++) {
        int worker = bitCount[mask];
        for (int job = 0; job < n; job++) {
            if (!(mask & (1 << job))) {
                int next = mask | (1 << job);
                long long candidate = dp[mask] + cost[worker][job];
                if (candidate < dp[next]) {
                    dp[next] = candidate;
                    chosenJob[next] = job;
                }
            }
        }
    }
    int mask = full;
    for (int worker = n - 1; worker >= 0; worker--) {
        int job = chosenJob[mask];
        assignment[worker] = job;
        mask ^= 1 << job;
    }
    printf("Minimum assignment cost: %lld\n", dp[full]);
    printf("Assignment (zero-based IDs):\n");
    for (int worker = 0; worker < n; worker++) {
        int job = assignment[worker];
        printf("  Worker %d -> job %d (cost %lld)\n", worker, job, cost[worker][job]);
    }
    return 0;
}
