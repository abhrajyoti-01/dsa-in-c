/*
Tarjan's Algorithm - Strongly Connected Components in One DFS (Advanced)
-----------------------------------------------------------------------
PROBLEM DEFINITION:
Split a directed graph into strongly connected components (SCCs):
maximal groups in which every vertex can reach every other. Kosaraju
needs two DFS passes and the reversed graph; Tarjan needs ONE pass,
using discovery times, low-link values and an explicit stack. Also
print the condensation: the DAG obtained by shrinking each SCC to a
single node.

REAL-LIFE EXAMPLE:
In a package manager, modules that import each other in a cycle must
be compiled together - each cycle is an SCC. The condensation DAG then
gives a valid build order for the groups. Social networks use SCCs to
find communities where everyone can reach everyone through follows.

DIAGRAM (sample graph):
     0 ---> 1          6 <---> 7
     ^      |          |
     |      v          v
     +----- 2 ---> 3 ---> 4
                   ^      |
                   |      v
                   +----- 5
  SCCs: {0,1,2}  {3,4,5}  {6,7}
  Condensation: {0,1,2} -> {3,4,5} <- {6,7}

PSEUDO CODE:
  DFS(u):
      disc[u] = low[u] = ++time;  push u;  onStack[u] = true
      for each edge u -> v:
          if v unvisited:  DFS(v);  low[u] = min(low[u], low[v])
          else if onStack[v]:      low[u] = min(low[u], disc[v])
      if low[u] == disc[u]:            (u is the root of an SCC)
          pop vertices down to u; they form one SCC
  Run DFS from every unvisited vertex.
  SCCs come out in REVERSE topological order of the condensation.

SAMPLE INPUT & DRY RUN:
  8 10
  0 1
  1 2
  2 0
  2 3
  3 4
  4 5
  5 3
  6 5
  6 7
  7 6
  DFS(0): disc 0=1, 1=2, 2=3; edge 2->0 hits the stack: low[2] = 1
          2->3: disc 3=4, 4=5, 5=6; edge 5->3 on stack: low[5] = 4
          back at 3: low[3] = disc[3] = 4 -> pop 5 4 3 = SCC 0
          back at 0: low[0] = disc[0] = 1 -> pop 2 1 0 = SCC 1
  DFS(6): disc 6=7, 7=8; 6->5 is finished (not on stack): ignored
          7->6 on stack: low[7] = 7 -> pop 7 6 = SCC 2
  Condensation edges: SCC 1 -> SCC 0 (2->3), SCC 2 -> SCC 0 (6->5)

SPECIAL CASES:
  - A vertex with no cycle through it is an SCC of size 1.
  - Self-loops and parallel edges are allowed and change nothing.
  - Edges to vertices that are finished but NOT on the stack belong to
    an already-closed SCC and must be ignored - the classic bug.
  - 1 <= n <= 1000, 0 <= m <= 10000, vertices 0..n-1. Recursion depth
    is at most n.

COMPLEXITY:
  O(V + E) time, O(V) extra space (stack, disc, low).

TIPS:
  low[u] = the earliest discovered vertex still on the stack that u's
  subtree can reach. Compare kosaraju-scc (two passes) and
  bridges-articulation (the same low-link idea on undirected graphs).
  2-SAT (two-sat.c) is the most famous application.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_M 10000
#define TRACE_LIMIT 20

int n;
int m;
int edgeFrom[MAX_M];
int edgeTo[MAX_M];
int start[MAX_N + 1];
int adjacency[MAX_M];
int disc[MAX_N];
int low[MAX_N];
int onStack[MAX_N];
int component[MAX_N];
int stack[MAX_N];
int top;
int timer;
int componentCount;
unsigned char linked[MAX_N][MAX_N];

/* Builds compressed adjacency lists that keep the input edge order. */
void buildAdjacency(void) {
    for (int i = 0; i < m; i++) start[edgeFrom[i] + 1]++;
    for (int u = 0; u < n; u++) start[u + 1] += start[u];
    int fill[MAX_N];
    for (int u = 0; u < n; u++) fill[u] = start[u];
    for (int i = 0; i < m; i++) adjacency[fill[edgeFrom[i]]++] = edgeTo[i];
}

void dfs(int u) {
    disc[u] = low[u] = ++timer;
    stack[top++] = u;
    onStack[u] = 1;
    for (int i = start[u]; i < start[u + 1]; i++) {
        int v = adjacency[i];
        if (disc[v] == 0) {
            dfs(v);
            if (low[v] < low[u]) low[u] = low[v];
        } else if (onStack[v] && disc[v] < low[u]) {
            low[u] = disc[v];
        }
    }
    if (low[u] == disc[u]) {
        printf("  SCC %d (root %d):", componentCount, u);
        int v;
        do {
            v = stack[--top];
            onStack[v] = 0;
            component[v] = componentCount;
            printf(" %d", v);
        } while (v != u);
        printf("\n");
        componentCount++;
    }
}

int main(void) {
    printf("Tarjan's Algorithm - Strongly Connected Components\n\n");
    printf("Vertices n (1-%d) and edges m (0-%d): ", MAX_N, MAX_M);
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || n > MAX_N || m < 0 ||
        m > MAX_M) {
        printf("Invalid size\n");
        return 1;
    }
    for (int i = 0; i < m; i++) {
        printf("Edge %d: from to: ", i + 1);
        if (scanf("%d %d", &edgeFrom[i], &edgeTo[i]) != 2 || edgeFrom[i] < 0 ||
            edgeFrom[i] >= n || edgeTo[i] < 0 || edgeTo[i] >= n) {
            printf("Invalid edge\n");
            return 1;
        }
    }
    buildAdjacency();

    printf("\nComponents in the order Tarjan closes them:\n");
    for (int u = 0; u < n; u++) {
        if (disc[u] == 0) dfs(u);
    }

    if (n <= TRACE_LIMIT) {
        printf("\nvertex:");
        for (int u = 0; u < n; u++) printf("%4d", u);
        printf("\ndisc:  ");
        for (int u = 0; u < n; u++) printf("%4d", disc[u]);
        printf("\nlow:   ");
        for (int u = 0; u < n; u++) printf("%4d", low[u]);
        printf("\nSCC:   ");
        for (int u = 0; u < n; u++) printf("%4d", component[u]);
        printf("\n");
    }

    printf("\n%d strongly connected component%s\n", componentCount,
           componentCount == 1 ? "" : "s");
    printf("Condensation DAG edges:");
    int edges = 0;
    for (int i = 0; i < m; i++) {
        int a = component[edgeFrom[i]];
        int b = component[edgeTo[i]];
        if (a != b && !linked[a][b]) {
            linked[a][b] = 1;
            printf("\n  SCC %d -> SCC %d  (via %d -> %d)", a, b, edgeFrom[i],
                   edgeTo[i]);
            edges++;
        }
    }
    printf(edges == 0 ? " none\n" : "\n");
    return 0;
}
