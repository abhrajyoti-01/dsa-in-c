/*
Dinic's Algorithm - Level Graphs and Blocking Flows (Master)
----------------------------------------------------------
PROBLEM DEFINITION:
Find maximum flow from source to sink in a directed capacity network.
Dinic uses BFS levels and sends a blocking flow before rebuilding levels.
Also print a minimum cut as a certificate of the maximum flow value.

REAL-LIFE EXAMPLE:
Measure the maximum throughput of a communication or supply network.
A minimum cut identifies links whose combined capacity is the bottleneck.

DIAGRAM:
         3          2
    s ------> a ------> t
    |         |         ^
   2|        1|         |3
    v         v         |
    b ------------------+
  First level graph permits s-a-t (2) and s-b-t (2).
  A later level graph permits s-a-b-t (1). Total flow = 5.

PSEUDO CODE:
  For every input edge, add a forward residual edge and a reverse edge
  While BFS assigns a level to sink:
      Reset current-edge pointers
      DFS sends flow along positive residual edges with level[v]=level[u]+1
      Update forward/reverse capacities; repeat until blocking flow reached
  BFS one final time: reachable vertices form the source side of a min cut

SAMPLE INPUT & DRY RUN:
  4 5
  0 1 3
  0 2 2
  1 2 1
  1 3 2
  2 3 3
  0 3
  Phase 1 pushes 4; phase 2 pushes 1; maximum flow = 5.
  Final source side is {0}; cut edges 0->1 (3), 0->2 (2), total 5.

SPECIAL CASES:
  - Parallel and antiparallel edges keep independent residual pairs.
  - Zero capacities, self-loops, and a disconnected sink are supported.
  - Source and sink MUST differ; reject equal or invalid vertices.
  - Vertices 0..n-1, 2 <= n <= 100, edges <= 2000, capacities 0..10^9.
    long long holds total flow and cut capacity, even above 32-bit range.

COMPLEXITY:
  O(V^2 * E) general-case time; O(V+E) storage, O(V) recursive DFS stack.
  Current-edge pointers avoid repeatedly scanning exhausted edges.

TIPS:
  Reverse edge index is e ^ 1 because pairs occupy adjacent even/odd slots.
  Reverse capacity means "undo flow", not another original input edge.
  Compare with max-flow (Edmonds-Karp), which augments one BFS path at a time.
*/

#include <stdio.h>
#include <limits.h>

#define MAX_V 100
#define MAX_E 2000
#define CAPACITY_LIMIT 1000000000LL

typedef struct {
    int to;
    int next;
    long long residual;
} Edge;

Edge edges[2 * MAX_E];
int head[MAX_V];
int level[MAX_V];
int current[MAX_V];
int edgeCount;
int originalFrom[MAX_E];
long long originalCapacity[MAX_E];

void addEdge(int u, int v, long long capacity) {
    edges[edgeCount] = (Edge){v, head[u], capacity};
    head[u] = edgeCount++;
    edges[edgeCount] = (Edge){u, head[v], 0};
    head[v] = edgeCount++;
}

int buildLevels(int n, int source, int sink) {
    int queue[MAX_V];
    int front = 0;
    int rear = 0;
    for (int i = 0; i < n; i++) level[i] = -1;
    level[source] = 0;
    queue[rear++] = source;
    while (front < rear) {
        int u = queue[front++];
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (edges[e].residual > 0 && level[v] == -1) {
                level[v] = level[u] + 1;
                queue[rear++] = v;
            }
        }
    }
    return level[sink] != -1;
}

long long sendFlow(int u, int sink, long long available) {
    if (u == sink) return available;
    for (int* e = &current[u]; *e != -1; *e = edges[*e].next) {
        Edge* edge = &edges[*e];
        if (edge->residual > 0 && level[edge->to] == level[u] + 1) {
            long long limit = available < edge->residual ? available : edge->residual;
            long long pushed = sendFlow(edge->to, sink, limit);
            if (pushed > 0) {
                edge->residual -= pushed;
                edges[*e ^ 1].residual += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int main(void) {
    int n;
    int m;
    int source;
    int sink;
    printf("Dinic's Maximum Flow\n\n");
    printf("Vertices (2-%d) and directed edges (0-%d): ", MAX_V, MAX_E);
    if (scanf("%d %d", &n, &m) != 2 ||
        n < 2 || n > MAX_V || m < 0 || m > MAX_E) {
        printf("Invalid graph size\n");
        return 1;
    }
    for (int i = 0; i < n; i++) head[i] = -1;
    printf("Enter edges as 'u v capacity' (zero-based, capacity 0..10^9):\n");
    for (int i = 0; i < m; i++) {
        int u;
        int v;
        long long capacity;
        if (scanf("%d %d %lld", &u, &v, &capacity) != 3 ||
            u < 0 || u >= n || v < 0 || v >= n ||
            capacity < 0 || capacity > CAPACITY_LIMIT) {
            printf("Invalid edge\n");
            return 1;
        }
        originalFrom[i] = u;
        originalCapacity[i] = capacity;
        addEdge(u, v, capacity);
    }
    printf("Source and sink: ");
    if (scanf("%d %d", &source, &sink) != 2 ||
        source < 0 || source >= n || sink < 0 || sink >= n || source == sink) {
        printf("Invalid source/sink: choose distinct vertices\n");
        return 1;
    }

    long long total = 0;
    int phase = 0;
    while (buildLevels(n, source, sink)) {
        for (int i = 0; i < n; i++) current[i] = head[i];
        long long phaseFlow = 0;
        long long pushed;
        while ((pushed = sendFlow(source, sink, LLONG_MAX)) > 0) {
            phaseFlow += pushed;
        }
        total += phaseFlow;
        printf("Phase %d: pushed %lld, total %lld\n", ++phase, phaseFlow, total);
    }
    printf("Maximum flow: %lld\n", total);
    printf("Min-cut edges (source side -> sink side):\n");
    long long cut = 0;
    for (int i = 0; i < m; i++) {
        int u = originalFrom[i];
        int v = edges[2 * i].to;
        if (level[u] != -1 && level[v] == -1 && originalCapacity[i] > 0) {
            printf("  %d -> %d (capacity %lld)\n", u, v, originalCapacity[i]);
            cut += originalCapacity[i];
        }
    }
    printf("Minimum cut capacity: %lld\n", cut);
    return 0;
}
