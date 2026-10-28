/*
Vertex Cover - 2-Approximation via Maximal Matching (Advanced)
-------------------------------------------------------------
PROBLEM DEFINITION:
A vertex cover is a set of vertices touching every edge. Finding the
SMALLEST one is NP-hard, so no fast exact algorithm is known. This
algorithm is fast and provably close: scan the edges; whenever an edge
has neither endpoint chosen yet, take BOTH endpoints. The result is
never more than twice the optimum. For small graphs (n <= 20) the
program also finds the true optimum by brute force and reports the
actual ratio.

REAL-LIFE EXAMPLE:
Place security cameras at street intersections so that every street
is watched by a camera at one of its ends; or choose the fewest
servers to monitor every network link. Cameras cost money, and "at
most twice the minimum" is a guarantee a planner can sign off on.

DIAGRAM (sample graph):
     1 ----- 3 ----- 4 ----- 5
     |       |       |
     0 ----- 2       6
  edges in input order: 0-1 0-2 1-3 2-3 3-4 4-5 4-6
  scan: 0-1 uncovered -> take 0,1;  0-2 covered;  1-3 covered;
        2-3 uncovered -> take 2,3;  3-4 covered;
        4-5 uncovered -> take 4,5;  4-6 covered
  approximate cover {0,1,2,3,4,5} (6)   optimum {1,2,4} (3)

PSEUDO CODE:
  cover = empty; matching = empty
  for each edge (u, v):
      if u not in cover and v not in cover:
          add u and v to cover; add (u, v) to matching
  Why <= 2 * OPT: the matched edges share no endpoints, and ANY cover
  needs a different vertex for each of them, so OPT >= |matching|,
  while our cover has exactly 2 * |matching| vertices.

SAMPLE INPUT & DRY RUN:
  7 7
  0 1
  0 2
  1 3
  2 3
  3 4
  4 5
  4 6
  matching: 0-1, 2-3, 4-5  ->  approximate cover size 6
  exact search: no cover of size 2 exists; {1,2,4} has size 3
  ratio 6 / 3 = 2.00 - the guarantee is TIGHT on this graph.

SPECIAL CASES:
  - No edges: both covers are empty (ratio reported as 1).
  - Self-loops (u = v) force u into every cover; they are handled by
    the same rule (taking "both" ends adds u once).
  - The greedy "take the highest-degree vertex" heuristic sounds
    better but can be a factor of ln(n) worse than optimal; the
    matching rule is the one with the constant-factor proof.
  - 1 <= n <= 100, 0 <= m <= 2000; exact search only when n <= 20.

COMPLEXITY:
  Approximation: O(n + m). Exact: O(2^n * n) using neighbour bitmasks
  (a mask is a cover iff every vertex outside it has all its
  neighbours inside it).

TIPS:
  Approximation ratio = (our cost) / (optimal cost), proved against a
  LOWER BOUND on OPT (here: the matching size) rather than OPT itself.
  The same style appears in metric-tsp-approx (lower bound = MST) and
  set-cover-greedy. Compare graph-coloring and hamiltonian-cycle for
  other NP-hard problems solved exactly by search.
*/

#include <stdio.h>

#define MAX_N 100
#define MAX_M 2000
#define EXACT_LIMIT 20

int n;
int m;
int edgeU[MAX_M];
int edgeV[MAX_M];
int inCover[MAX_N];
unsigned int neighbours[EXACT_LIMIT];

int popcount(unsigned int x) {
    int count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
}

int isCover(unsigned int mask) {
    for (int v = 0; v < n; v++) {
        if (!((mask >> v) & 1u) && (neighbours[v] & ~mask) != 0) return 0;
    }
    return 1;
}

void printMask(unsigned int mask) {
    printf("{");
    int first = 1;
    for (int v = 0; v < n; v++) {
        if ((mask >> v) & 1u) {
            printf(first ? "%d" : ",%d", v);
            first = 0;
        }
    }
    printf("}");
}

int main(void) {
    printf("Vertex Cover - 2-Approximation\n\n");
    printf("Vertices n (1-%d) and edges m (0-%d): ", MAX_N, MAX_M);
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || n > MAX_N || m < 0 ||
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
    }

    int coverSize = 0;
    printf("\nMatched edges:");
    for (int i = 0; i < m; i++) {
        int u = edgeU[i];
        int v = edgeV[i];
        if (!inCover[u] && !inCover[v]) {
            inCover[u] = 1;
            inCover[v] = 1;
            coverSize += u == v ? 1 : 2;
            printf(" %d-%d", u, v);
        }
    }
    if (coverSize == 0) printf(" none");
    printf("\nApproximate cover (%d vertices): {", coverSize);
    int first = 1;
    for (int v = 0; v < n; v++) {
        if (inCover[v]) {
            printf(first ? "%d" : ",%d", v);
            first = 0;
        }
    }
    printf("}\n");

    if (n <= EXACT_LIMIT) {
        for (int i = 0; i < m; i++) {
            neighbours[edgeU[i]] |= 1u << edgeV[i];
            neighbours[edgeV[i]] |= 1u << edgeU[i];
        }
        unsigned int best = (1u << n) - 1;
        for (unsigned int mask = 0; mask < (1u << n); mask++) {
            if (popcount(mask) < popcount(best) && isCover(mask)) best = mask;
        }
        int optimum = popcount(best);
        printf("Optimal cover (%d vertices): ", optimum);
        printMask(best);
        printf("\nRatio: %.2f (guarantee: at most 2)\n",
               optimum == 0 ? 1.0 : (double)coverSize / optimum);
    }
    return 0;
}
