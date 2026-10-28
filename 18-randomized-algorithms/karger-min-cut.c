/*
Karger's Algorithm - Global Minimum Cut by Random Contraction (Advanced)
-----------------------------------------------------------------------
PROBLEM DEFINITION:
Split the vertices of a connected undirected multigraph into two
non-empty groups so that as few edges as possible cross between them
(the global minimum cut). Karger's idea: pick a random edge, merge
(contract) its two endpoints, repeat until two super-vertices remain;
the edges between them form a cut. One run finds a minimum cut with
probability at least 2 / (n(n-1)), so the algorithm repeats many
independent trials and keeps the best - a Monte Carlo algorithm.

REAL-LIFE EXAMPLE:
Network reliability: "what is the smallest number of cables whose
failure splits the data centre network?" Image segmentation and
clustering also look for sparse cuts between dense groups.

DIAGRAM (sample: two dense groups joined by two edges):
     0 --- 1          4 --- 5
     | \ / |          | \ / |
     | / \ |          | / \ |
     2 --- 3          6 --- 7
  each group is a complete K4; bridges 0-4 and 3-7
  minimum cut = 2: {0,1,2,3} | {4,5,6,7}
  A random edge is far more likely to lie INSIDE a dense group, so
  contraction tends to merge each group before touching the bridges.

PSEUDO CODE:
  Trial():
      shuffle the edge list (Fisher-Yates)
      components = n; union-find with every vertex alone
      for each edge (u, v) in shuffled order, while components > 2:
          if find(u) != find(v): union(u, v); components--
      return number of edges with find(u) != find(v)
  (contracting a uniformly random remaining edge is equivalent to
   scanning a random permutation and skipping self-loops)
  Repeat Trial() T times, keep the smallest cut.

SAMPLE INPUT & DRY RUN:
  8 14
  0 1
  0 2
  0 3
  1 2
  1 3
  2 3
  4 5
  4 6
  4 7
  5 6
  5 7
  6 7
  0 4
  3 7
  100
  Most trials end with a cut of 3 or more: e.g. vertex 5 alone, whose
  3 edges all cross. The first 20 trials of this seed NEVER find the
  2-edge cut - run the sample with 20 trials and the exact check
  reports "Karger MISSED it". That is the Monte Carlo trade-off.
  With 100 trials: 15 of 100 (15%) find the cut of 2 edges,
  side A = {0,1,2,3}; the guaranteed rate 2/(8*7) is only 3.6%.
  Exact check over all 127 bipartitions: 2 -> Karger found it.

SPECIAL CASES:
  - A disconnected graph has minimum cut 0; it is detected up front.
  - Self-loops never cross a cut and are ignored; parallel edges count
    once per copy (multigraph).
  - The random generator is a fixed-seed xorshift, so the run is
    reproducible on every compiler.
  - For n <= 16 the exact answer is also found by trying all 2^(n-1)
    bipartitions, to show the Monte Carlo result can be checked.
  - 2 <= n <= 100, 1 <= m <= 2000, 1 <= trials <= 10000.

COMPLEXITY:
  O(m alpha(n)) per trial with union-find, O(T * m) in total.
  T = n^2 ln n trials push the failure probability below 1/n.
  Karger-Stein (recursive contraction) needs far fewer: O(n^2 log^3 n).

TIPS:
  Monte Carlo = fixed running time, small chance of a wrong answer;
  Las Vegas (randomized quicksort) = always right, random running time.
  Compare kruskal (same "union in edge order" loop, sorted instead of
  shuffled) and dinic-max-flow (s-t minimum cuts, deterministic).
*/

#include <stdio.h>

#define MAX_N 100
#define MAX_M 2000
#define MAX_TRIALS 10000
#define EXACT_LIMIT 16
#define SEED 777u

int n;
int m;
int edgeU[MAX_M];
int edgeV[MAX_M];
int order[MAX_M];
int parent[MAX_N];
int trialCut[MAX_TRIALS];
unsigned int rngState = SEED;

unsigned int nextRandom(void) {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int countCrossing(void) {
    int cut = 0;
    for (int i = 0; i < m; i++) {
        if (find(edgeU[i]) != find(edgeV[i])) cut++;
    }
    return cut;
}

/* Contracts edges in a random order until `target` groups remain. */
int contract(int target) {
    for (int i = 0; i < n; i++) parent[i] = i;
    for (int i = m - 1; i > 0; i--) {
        int j = (int)(nextRandom() % (unsigned int)(i + 1));
        int t = order[i];
        order[i] = order[j];
        order[j] = t;
    }
    int components = n;
    for (int k = 0; k < m && components > target; k++) {
        int a = find(edgeU[order[k]]);
        int b = find(edgeV[order[k]]);
        if (a != b) {
            parent[a] = b;
            components--;
        }
    }
    return components;
}

void printSide(int representative) {
    printf("{");
    int first = 1;
    for (int v = 0; v < n; v++) {
        if (find(v) == representative) {
            printf(first ? "%d" : ",%d", v);
            first = 0;
        }
    }
    printf("}");
}

int main(void) {
    int trials;

    printf("Karger's Randomized Minimum Cut\n\n");
    printf("Vertices n (2-%d) and edges m (1-%d): ", MAX_N, MAX_M);
    if (scanf("%d %d", &n, &m) != 2 || n < 2 || n > MAX_N || m < 1 ||
        m > MAX_M) {
        printf("Invalid size\n");
        return 1;
    }
    for (int i = 0; i < m; i++) {
        printf("Edge %d: u v: ", i + 1);
        if (scanf("%d %d", &edgeU[i], &edgeV[i]) != 2 || edgeU[i] < 0 ||
            edgeU[i] >= n || edgeV[i] < 0 || edgeV[i] >= n) {
            printf("Invalid edge\n");
            return 1;
        }
        order[i] = i;
    }
    printf("Number of trials (1-%d): ", MAX_TRIALS);
    if (scanf("%d", &trials) != 1 || trials < 1 || trials > MAX_TRIALS) {
        printf("Invalid trial count\n");
        return 1;
    }

    for (int i = 0; i < n; i++) parent[i] = i;
    int components = n;
    for (int i = 0; i < m; i++) {
        int a = find(edgeU[i]);
        int b = find(edgeV[i]);
        if (a != b) {
            parent[a] = b;
            components--;
        }
    }
    if (components > 1) {
        printf("\nThe graph is disconnected: minimum cut 0, e.g. ");
        printSide(find(0));
        printf(" versus the rest\n");
        return 0;
    }

    int best = MAX_M + 1;
    int bestSide[MAX_N];
    for (int t = 0; t < trials; t++) {
        contract(2);
        trialCut[t] = countCrossing();
        if (trialCut[t] < best) {
            best = trialCut[t];
            for (int v = 0; v < n; v++) bestSide[v] = find(v) == find(0);
        }
    }
    int hits = 0;
    for (int t = 0; t < trials; t++) hits += trialCut[t] == best;

    printf("\nCut found by each trial:");
    for (int t = 0; t < trials && t < 40; t++) printf(" %d", trialCut[t]);
    if (trials > 40) printf(" ...");
    printf("\nMinimum cut found: %d edges\n  side A: {", best);
    int first = 1;
    for (int v = 0; v < n; v++) {
        if (bestSide[v]) {
            printf(first ? "%d" : ",%d", v);
            first = 0;
        }
    }
    printf("}\nTrials that found it: %d of %d (%.0f%%); guaranteed lower "
           "bound per trial: 2/(n(n-1)) = %.1f%%\n", hits, trials,
           100.0 * hits / trials, 200.0 / ((double)n * (n - 1)));

    if (n <= EXACT_LIMIT) {
        int exact = MAX_M + 1;
        for (int mask = 1; mask < (1 << (n - 1)); mask++) {
            int cut = 0;
            for (int i = 0; i < m; i++) {
                int sideU = edgeU[i] == 0 ? 0 : (mask >> (edgeU[i] - 1)) & 1;
                int sideV = edgeV[i] == 0 ? 0 : (mask >> (edgeV[i] - 1)) & 1;
                if (sideU != sideV) cut++;
            }
            if (cut < exact) exact = cut;
        }
        printf("Exact minimum cut (all %d bipartitions): %d -> %s\n",
               (1 << (n - 1)) - 1, exact,
               exact == best ? "Karger found it" : "Karger MISSED it");
    }
    return 0;
}
