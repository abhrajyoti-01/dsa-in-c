/*
Multistage Graph - Shortest Path by Forward and Backward DP (Intermediate)
-------------------------------------------------------------------------
PROBLEM DEFINITION:
A multistage graph splits its vertices into stages V1, V2, ..., Vk; every
edge goes from one stage to the next, V1 = {source} and Vk = {sink}.
Find a minimum-cost source-to-sink path. Vertices are numbered stage
by stage (1 = source, n = sink), so every edge (u, v) has u < v and the
DP can simply scan vertex numbers - this is the classic Horowitz-Sahni
formulation, solved two ways:
  forward approach:  cost(j)  = cheapest path from j to the sink
  backward approach: bcost(j) = cheapest path from the source to j

REAL-LIFE EXAMPLE:
Planning a project in phases: phase 1 picks a supplier, phase 2 a
factory, phase 3 a warehouse, phase 4 a retailer. Each choice
constrains the next, and edge costs are transport or switching costs.
Resource allocation ("give x units to project 1, y to project 2...")
is also modelled as a multistage graph.

DIAGRAM (the classic 5-stage, 12-vertex textbook graph):
   stage:   V1      V2       V3       V4       V5
                     2        6        9
             1  -->  3  -->   7  -->  10  -->  12
                     4        8       11
                     5
  The source 1 connects to 2..5, stage-2 vertices to 6..8, stage-3
  vertices to 9..11, and 9, 10, 11 lead to the sink 12. Edge costs are
  in the sample input; every edge moves exactly one stage right.

PSEUDO CODE:
  Forward:
      cost(n) = 0
      for j = n-1 down to 1:
          cost(j) = min over edges (j, l) of c(j, l) + cost(l)
          next(j) = the l that achieved the minimum
      path: 1, next(1), next(next(1)), ..., n
  Backward:
      bcost(1) = 0
      for j = 2 up to n:
          bcost(j) = min over edges (l, j) of bcost(l) + c(l, j)
          prev(j) = the l that achieved the minimum
      path: follow prev() from n back to 1, then reverse

SAMPLE INPUT & DRY RUN:
  12 21
  1 2 9
  1 3 7
  1 4 3
  1 5 2
  2 6 4
  2 7 2
  2 8 1
  3 6 2
  3 7 7
  4 8 11
  5 7 11
  5 8 8
  6 9 6
  6 10 5
  7 9 4
  7 10 3
  8 10 5
  8 11 6
  9 12 4
  10 12 2
  11 12 5
  Forward, from the sink back:
    cost(12)=0  cost(11)=5  cost(10)=2  cost(9)=4
    cost(8)=min(5+2, 6+5)=7      cost(7)=min(4+4, 3+2)=5
    cost(6)=min(6+4, 5+2)=7      cost(5)=min(11+5, 8+7)=15
    cost(4)=11+7=18              cost(3)=min(2+7, 7+5)=9
    cost(2)=min(4+7, 2+5, 1+7)=7
    cost(1)=min(9+7, 7+9, 3+18, 2+15)=16  (tie 2 vs 3: first kept)
  Forward path:  1 -> 2 -> 7 -> 10 -> 12, cost 16
  Backward path: 1 -> 3 -> 6 -> 10 -> 12, cost 16
  Two different paths, both optimal - ties are broken differently.

SPECIAL CASES:
  - If the sink cannot be reached, the program says so.
  - Parallel edges keep the cheapest; edges with u >= v are rejected
    (they would break the stage-by-stage numbering).
  - Vertices that are skipped by every path keep cost "inf".
  - 2 <= n <= 100, edges <= 5000, costs 0..10^6.

COMPLEXITY:
  O(V + E) with adjacency lists; O(V^2) with the cost matrix used here.
  Space O(V^2) for the matrix.

TIPS:
  This is shortest path in a DAG: the vertex numbering IS a topological
  order. The principle of optimality - every tail of an optimal path is
  optimal - is what makes the DP valid. Compare topo-sort, dijkstra and
  the knapsack DPs (stages = items).
*/

#include <stdio.h>

#define MAX_N 100
#define MAX_EDGES 5000
#define COST_LIMIT 1000000
#define NO_EDGE -1
#define INF 1000000000000000000LL

long long edgeCost[MAX_N + 1][MAX_N + 1];
long long cost[MAX_N + 1];
long long bcost[MAX_N + 1];
int nextVertex[MAX_N + 1];
int prevVertex[MAX_N + 1];

void printCost(long long value) {
    if (value >= INF) printf("%5s", "inf");
    else printf("%5lld", value);
}

int main(void) {
    int n;
    int m;

    printf("Multistage Graph - Forward and Backward DP\n\n");
    printf("Vertices n (2-%d, numbered stage by stage, 1 = source, n = sink) "
           "and edges m: ", MAX_N);
    if (scanf("%d %d", &n, &m) != 2 || n < 2 || n > MAX_N || m < 0 ||
        m > MAX_EDGES) {
        printf("Invalid size\n");
        return 1;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) edgeCost[i][j] = NO_EDGE;
    }
    for (int i = 0; i < m; i++) {
        int u;
        int v;
        long long c;
        printf("Edge %d: u v cost (u < v): ", i + 1);
        if (scanf("%d %d %lld", &u, &v, &c) != 3 || u < 1 || v > n ||
            u >= v || c < 0 || c > COST_LIMIT) {
            printf("Invalid edge\n");
            return 1;
        }
        if (edgeCost[u][v] == NO_EDGE || c < edgeCost[u][v]) edgeCost[u][v] = c;
    }

    cost[n] = 0;
    nextVertex[n] = 0;
    for (int j = n - 1; j >= 1; j--) {
        cost[j] = INF;
        nextVertex[j] = 0;
        for (int l = j + 1; l <= n; l++) {
            if (edgeCost[j][l] == NO_EDGE || cost[l] >= INF) continue;
            if (edgeCost[j][l] + cost[l] < cost[j]) {
                cost[j] = edgeCost[j][l] + cost[l];
                nextVertex[j] = l;
            }
        }
    }
    bcost[1] = 0;
    prevVertex[1] = 0;
    for (int j = 2; j <= n; j++) {
        bcost[j] = INF;
        prevVertex[j] = 0;
        for (int l = 1; l < j; l++) {
            if (edgeCost[l][j] == NO_EDGE || bcost[l] >= INF) continue;
            if (bcost[l] + edgeCost[l][j] < bcost[j]) {
                bcost[j] = bcost[l] + edgeCost[l][j];
                prevVertex[j] = l;
            }
        }
    }

    if (n <= 20) {
        printf("\nvertex:");
        for (int j = 1; j <= n; j++) printf("%5d", j);
        printf("\ncost:  ");
        for (int j = 1; j <= n; j++) printCost(cost[j]);
        printf("\nbcost: ");
        for (int j = 1; j <= n; j++) printCost(bcost[j]);
        printf("\n");
    }
    if (cost[1] >= INF) {
        printf("\nThe sink %d cannot be reached from the source 1.\n", n);
        return 0;
    }

    printf("\nForward approach:  minimum cost %lld, path 1", cost[1]);
    for (int v = nextVertex[1]; v != 0; v = nextVertex[v]) printf(" -> %d", v);

    int path[MAX_N];
    int length = 0;
    for (int v = n; v != 0; v = prevVertex[v]) path[length++] = v;
    printf("\nBackward approach: minimum cost %lld, path ", bcost[n]);
    for (int i = length - 1; i >= 0; i--) {
        printf(i == length - 1 ? "%d" : " -> %d", path[i]);
    }
    printf("\n");
    return 0;
}
