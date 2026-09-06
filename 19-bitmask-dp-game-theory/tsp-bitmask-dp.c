/*
tsp-backtracking with Bitmask DP (Held-Karp)
-------------------------------
What this program solves:
The TRAVELLING SALESMAN PROBLEM exactly with bitmask DP: visit
every city exactly once and return to start, minimizing total
distance. O(n^2 * 2^n) - beats the n! brute force dramatically.

Real life example:
Drone delivery loops and PCB drilling: exact optimal tour for up
to ~20 stops where brute force n! is impossible.

DIAGRAM (Held-Karp: dp[mask][city] = min cost visiting set mask, ending at city):
  dp[0001][0] = 0
  dp[0011][1] = dp[0001][0] + cost(0,1)
  dp[1111][c] -> + cost(c, 0) -> answer
  mask bit i = city i already visited; 2^n * n^2 states.

BEGINNER EXPLANATION (the simplest way to think):
"Which cities have I visited?" can be one NUMBER if each city is
a bit: mask 0b1011 = visited cities 0, 1, 3. DP question:
"cheapest route that starts at 0, is currently AT city j, and has
visited exactly the cities in mask." Extend one city at a time:
dp[mask | (1<<next)][next] = dp[mask][j] + dist[j][next].
At the end, add the return-to-start edge and take the minimum.

SUPER SIMPLE EXAMPLE (4 cities):
dist 0-1=10, 0-2=15, 0-3=20, 1-2=35, 1-3=25, 2-3=30
dp[{0}][0] = 0
dp[{0,1}][1] = 10; dp[{0,2}][2] = 15; dp[{0,3}][3] = 20
dp[{0,1,2}][2] = min(via 1: 10+35) = 45
dp[{0,1,3}][3] = min(via 1: 10+25) = 35
dp[{0,2,3}][3] = min(via 2: 15+30) = 45
full set {0,1,2,3}: end at 1: 45(from{0,2,3}via2) ... compute all:
  end 1: min(dp{0,2,3}[2]+30, dp{0,2,3}? ) = 45+30=75
  end 2: dp{0,1,3}[1]+35 = 35+35=70; dp{0,1,3}[3]+30=35+30=65 -> 65
  end 3: dp{0,1,2}[1]+25=45+25=70; dp{0,1,2}[2]+30=45+30=75 -> 70
answer = min(75, 65, 70) + nothing (0 is start, return edge
already included as dist to last city) = 65.
Optimal tour: 0 -> 1 -> 3 -> 2 -> 0 = 10+25+30+15 = 80? No -
0->1(10) +1->3(25) +3->2(30) +2->0(15) = 80... but DP says 65?
Recheck: end 2 = 65 came via dp{0,1,3}[3]=35 then +dist[3][2]=30
= 65, tour 0-1-3-2, return 2-0 = 15: TOTAL = 35+30 = 65 already
includes everything except... dp{0,1,3}[3] = 0->1->3 = 35, then
3->2 = 30, then 2->0 = 15 = 80. The final answer must ADD the
return edge dist[last][0]: 65 + 15 = 80. Exactly the tour cost.
Lesson: the DP value excludes the closing edge - add it at the end.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- n = 1: tour is trivially 0 (already there).
- n = 2: 2 * dist[0][1].
- Asymmetric distances (dist[i][j] != dist[j][i]): the DP handles
  it automatically - no assumption of symmetry anywhere.
- n > 20: 2^20 masks = 1M x 20 states - the practical limit of
  exact bitmask DP; beyond that, heuristics (2-opt, Lin-Kernighan)
  or branch & bound (folder 14) take over.
- Memory: (2^n) x n table - 2^20 x 20 x 4B = 80 MB. Watch RAM.

DRY RUN (the 4-city case above, mask in binary):
dp[0001][0] = 0
dp[0011][1] = dp[0001][0] + d01 = 10
dp[0101][2] = 15; dp[1001][3] = 20
dp[0111][2] = dp[0011][1] + d12 = 45; also dp[0101][2]+d02? mask
  0101 lacks city 1 -> not valid predecessor for {0,1,2}.
dp[1011][3] = dp[0011][1] + d13 = 35
dp[1101][3] = dp[0101][2] + d23 = 45
dp[1111][1] = min(dp[1101][2] + d20?) -> dp[1101][2] = 45, +d21=35 -> 80
dp[1111][2] = min(dp[1011][1] + d12=70, dp[1011][3] + d32=65) = 65
dp[1111][3] = min(dp[0111][1] + d13=70, dp[0111][2] + d23=75) = 70
answer = min(80, 65+15? no: +d[2][0]=15 -> 65+15=80 is wrong
because 65 = 35+30 which excluded the return; final = 65 + 15 = 80)
= 80. Tour 0 -> 1 -> 3 -> 2 -> 0.

TIPS:
- THE canonical DP-on-subsets problem: once mastered, subset-sum
  over masks, assignment problems, and Steiner trees all follow.
- Order of loops: iterate masks ascending; for each mask, for each
  last-city j IN mask, for each next-city NOT in mask.
- Path reconstruction: keep a parent table or recompute by walking
  backwards from the best final state.
- Interview framing: "n! brute force dies at n=12; bitmask DP
  handles n=20 - that is the exact-exponential frontier."
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 20
#define INF 1000000000

int dist[MAX_N][MAX_N];
int dp[1 << MAX_N][MAX_N];
int parent[1 << MAX_N][MAX_N];

int main() {
    int n;

    printf("tsp-backtracking - Bitmask Dynamic Programming (exact)\n\n");

    printf("Number of cities (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter the distance matrix row by row (%d values per row):\n", n);
    for (int i = 0; i < n; i++) {
        printf("  Row %d: ", i);
        for (int j = 0; j < n; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    int full = 1 << n;

    for (int mask = 0; mask < full; mask++) {
        for (int j = 0; j < n; j++) {
            dp[mask][j] = INF;
            parent[mask][j] = -1;
        }
    }

    dp[1][0] = 0;

    /* STEP: extend tours mask by mask */
    for (int mask = 1; mask < full; mask++) {
        for (int j = 0; j < n; j++) {
            if (!(mask & (1 << j))) {
                continue;
            }
            if (dp[mask][j] == INF) {
                continue;
            }
            for (int next = 0; next < n; next++) {
                if (mask & (1 << next)) {
                    continue;
                }
                int newMask = mask | (1 << next);
                int cand = dp[mask][j] + dist[j][next];
                if (cand < dp[newMask][next]) {
                    dp[newMask][next] = cand;
                    parent[newMask][next] = j;
                }
            }
        }
    }

    /* STEP: close the tour back to city 0 and find the best end */
    int best = INF;
    int bestEnd = 0;
    for (int j = 1; j < n; j++) {
        if (dp[full - 1][j] == INF) {
            continue;
        }
        int total = dp[full - 1][j] + dist[j][0];
        if (total < best) {
            best = total;
            bestEnd = j;
        }
    }

    printf("\nMinimum tour cost: %d\n", best);

    /* STEP: reconstruct the tour backwards */
    int tour[MAX_N + 1];
    int mask = full - 1;
    int j = bestEnd;
    int idx = n - 1;
    tour[idx] = j;
    while (j != 0) {
        int p = parent[mask][j];
        mask = mask ^ (1 << j);
        idx = idx - 1;
        tour[idx] = p;
        j = p;
    }
    tour[n] = 0;

    printf("Tour: ");
    for (int i = 0; i <= n; i++) {
        printf("%d ", tour[i]);
    }
    printf("\n");

    return 0;
}
