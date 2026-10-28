/*
Metric TSP - 2-Approximation with a Minimum Spanning Tree (Advanced)
-------------------------------------------------------------------
PROBLEM DEFINITION:
Visit every city exactly once and return to the start, minimizing the
total distance (the Travelling Salesman Problem). It is NP-hard, but
when distances obey the triangle inequality d(a,c) <= d(a,b) + d(b,c)
(a "metric"), a fast tour at most TWICE the optimum is easy:
  1. build a minimum spanning tree (MST),
  2. walk around the tree in DFS preorder,
  3. visit cities in that order, skipping repeats ("shortcutting").
For n <= 15 the program also computes the exact optimum with the
Held-Karp bitmask DP and reports the real ratio.

REAL-LIFE EXAMPLE:
A delivery van leaving a depot, a technician visiting service calls,
or a drill head visiting holes on a circuit board. Road distances are
naturally metric (a detour is never shorter), so the guarantee holds.

DIAGRAM (sample: depot 0 in the middle of four suburbs on a ring):
          1 ---(4)--- 3
          | \       / |
         (4) 3     3 (4)      spokes 0-1, 0-2, 0-3, 0-4 cost 3
          |     0     |       ring edges 1-3, 3-2, 2-4, 4-1 cost 4
          |  3     3  |       diagonals 1-2 and 3-4 (not drawn) cost 6
          | /       \ |
          4 ---(4)--- 2
  MST = the star around 0 (weight 12). Preorder 0,1,2,3,4 jumps
  ACROSS the ring twice: 3+6+4+6+3 = 22. Going around the ring
  (0,4,2,3,1 or its mirror 0,1,3,2,4) costs only 18.

PSEUDO CODE:
  Validate: symmetric, zero diagonal, positive distances, and
            d[i][k] <= d[i][j] + d[j][k] for all i, j, k
  MST = Prim from city 0 (ties -> lower index)
  tour = DFS preorder of the MST from 0 (children in index order), then 0
  Proof of the factor 2:
      MST <= OPT        (deleting one edge of the best tour leaves a
                         spanning tree)
      walk around tree = 2 * MST, and shortcuts never lengthen it
                        (triangle inequality)
      so tour <= 2 * MST <= 2 * OPT
  Exact (n <= 15): dp[mask][j] = shortest path from 0 through mask
                   ending at j; answer min dp[all][j] + d[j][0]

SAMPLE INPUT & DRY RUN:
  5
  0 3 3 3 3
  3 0 6 4 4
  3 6 0 4 4
  3 4 4 0 6
  3 4 4 6 0
  Prim: every spoke costs 3, no ring edge is cheaper -> star, MST = 12
  preorder tour: 0 1 2 3 4 0 = 3 + 6 + 4 + 6 + 3 = 22
  Held-Karp optimum: 0 4 2 3 1 0 = 3 + 4 + 4 + 4 + 3 = 18
      (the mirror tour 0 1 3 2 4 0 ties; the DP reports this one)
  ratio 22 / 18 = 1.22;  bounds: MST 12 <= OPT 18 <= 22 <= 2*MST 24

SPECIAL CASES:
  - n = 1: the tour is just 0 -> 0 with cost 0; n = 2: 0 -> 1 -> 0.
  - If the triangle inequality fails the program refuses: without it
    the shortcuts can make the tour arbitrarily bad (and no constant
    approximation exists unless P = NP).
  - Ties in Prim and in the DP go to the lower index, so the tours
    shown are reproducible.
  - 1 <= n <= 100 cities, distances 1..10^6 off the diagonal; the exact
    DP runs only for n <= 15.

COMPLEXITY:
  Approximation: O(n^2) with array-based Prim (plus O(n^3) to verify
  the metric). Held-Karp: O(n^2 * 2^n) time, O(n * 2^n) memory.

TIPS:
  Christofides' algorithm adds a minimum perfect matching on the
  odd-degree MST vertices and improves the factor to 1.5. In practice,
  improve any tour with 2-opt swaps. Compare prim (step 1),
  tsp-bitmask-dp (the exact DP) and tsp-branch-and-bound.
*/

#include <stdio.h>

#define MAX_N 100
#define EXACT_LIMIT 15
#define DISTANCE_LIMIT 1000000
#define INF 1000000000

int n;
int dist[MAX_N][MAX_N];
int parent[MAX_N];
int tour[MAX_N + 1];
int tourLength;
int dp[1 << EXACT_LIMIT][EXACT_LIMIT];
signed char from[1 << EXACT_LIMIT][EXACT_LIMIT];

void preorder(int u) {
    tour[tourLength++] = u;
    for (int v = 0; v < n; v++) {
        if (v != u && parent[v] == u) preorder(v);
    }
}

int tourCost(const int *cities, int count) {
    int total = 0;
    for (int i = 0; i + 1 < count; i++) total += dist[cities[i]][cities[i + 1]];
    return total;
}

void printTour(const int *cities, int count) {
    for (int i = 0; i < count; i++) printf(i ? " %d" : "%d", cities[i]);
}

int main(void) {
    printf("Metric TSP - MST 2-Approximation\n\n");
    printf("Number of cities (1-%d): ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter the %d x %d distance matrix:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &dist[i][j]) != 1 || dist[i][j] < 0 ||
                dist[i][j] > DISTANCE_LIMIT) {
                printf("Invalid distance\n");
                return 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i == j && dist[i][j] != 0) || (i != j && dist[i][j] == 0) ||
                dist[i][j] != dist[j][i]) {
                printf("Not a valid symmetric distance matrix at (%d,%d)\n", i, j);
                return 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (dist[i][k] > dist[i][j] + dist[j][k]) {
                    printf("Triangle inequality fails: d(%d,%d) = %d > "
                           "d(%d,%d) + d(%d,%d) = %d\n", i, k, dist[i][k], i, j,
                           j, k, dist[i][j] + dist[j][k]);
                    return 1;
                }
            }
        }
    }

    int key[MAX_N];
    int done[MAX_N];
    for (int v = 0; v < n; v++) {
        key[v] = INF;
        done[v] = 0;
        parent[v] = -1;
    }
    key[0] = 0;
    int mstWeight = 0;
    printf("\nMST edges (Prim from 0):");
    for (int step = 0; step < n; step++) {
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!done[v] && (u < 0 || key[v] < key[u])) u = v;
        }
        done[u] = 1;
        if (parent[u] >= 0) {
            mstWeight += dist[parent[u]][u];
            printf(" %d-%d(%d)", parent[u], u, dist[parent[u]][u]);
        }
        for (int v = 0; v < n; v++) {
            if (!done[v] && dist[u][v] < key[v]) {
                key[v] = dist[u][v];
                parent[v] = u;
            }
        }
    }
    printf("%s\nMST weight: %d\n", n == 1 ? " none" : "", mstWeight);

    preorder(0);
    tour[tourLength++] = 0;
    int approxCost = tourCost(tour, tourLength);
    printf("Approximate tour (preorder): ");
    printTour(tour, tourLength);
    printf("  cost %d\n", approxCost);

    if (n > EXACT_LIMIT || n == 1) {
        printf("Guarantee: cost <= 2 * MST = %d\n", 2 * mstWeight);
        return 0;
    }
    int full = (1 << n) - 1;
    for (int mask = 0; mask <= full; mask++) {
        for (int j = 0; j < n; j++) dp[mask][j] = INF;
    }
    dp[1][0] = 0;
    for (int mask = 1; mask <= full; mask += 2) {
        for (int j = 0; j < n; j++) {
            if (dp[mask][j] >= INF) continue;
            for (int k = 1; k < n; k++) {
                if ((mask >> k) & 1) continue;
                int next = mask | (1 << k);
                int candidate = dp[mask][j] + dist[j][k];
                if (candidate < dp[next][k]) {
                    dp[next][k] = candidate;
                    from[next][k] = (signed char)j;
                }
            }
        }
    }
    int bestLast = 1;
    for (int j = 2; j < n; j++) {
        if (dp[full][j] + dist[j][0] < dp[full][bestLast] + dist[bestLast][0]) {
            bestLast = j;
        }
    }
    int optimum = dp[full][bestLast] + dist[bestLast][0];
    int exactTour[MAX_N + 1];
    int mask = full;
    int city = bestLast;
    exactTour[n] = 0;
    for (int i = n - 1; i >= 1; i--) {
        exactTour[i] = city;
        int previous = from[mask][city];
        mask ^= 1 << city;
        city = previous;
    }
    exactTour[0] = 0;
    printf("Optimal tour (Held-Karp):    ");
    printTour(exactTour, n + 1);
    printf("  cost %d\n", optimum);
    printf("Ratio %.2f;  MST %d <= OPT %d <= tour %d <= 2*MST %d\n",
           (double)approxCost / optimum, mstWeight, optimum, approxCost,
           2 * mstWeight);
    return 0;
}
