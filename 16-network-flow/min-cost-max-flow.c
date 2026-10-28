/*
Min-Cost Max-Flow - Successive Shortest Augmenting Paths (Master)
----------------------------------------------------------------
PROBLEM DEFINITION:
Each edge of a directed network has a capacity and a cost per unit of
flow. Send the MAXIMUM possible flow from source s to sink t, and among
all maximum flows choose one of MINIMUM total cost. Repeatedly find the
cheapest s-t path in the residual graph (Bellman-Ford, because reverse
edges carry negative cost), push as much flow as it allows, and stop
when t is unreachable.

REAL-LIFE EXAMPLE:
A logistics company ships goods from a factory to a city through
warehouses. Roads have truck limits (capacity) and fuel cost per
truck (cost). Ship as much as the network can carry, as cheaply as
possible. Assignment problems (workers to jobs with costs) are the
special case where every capacity is 1.

DIAGRAM (sample network, label = capacity/cost):
               2/1          1/3
          0 ---------> 1 ---------> 3
          |            |            ^
       1/2|         1/1|            | 2/1
          |            v            |
          +----------> 2 -----------+
  Reverse residual edge v->u has capacity 0 at first and cost -c:
  pushing flow back "refunds" the cost.

PSEUDO CODE:
  for every edge (u, v, cap, cost): add u->v (cap, cost), v->u (0, -cost)
  flow = 0, cost = 0
  loop:
      Bellman-Ford from s over edges with residual > 0
      if t unreachable: stop
      push f = min residual along the cheapest path
      flow += f;  cost += f * dist[t]
      residual[e] -= f;  residual[reverse e] += f
  (no negative cycles appear in the residual graph, so the
   shortest-path step is always well defined)

SAMPLE INPUT & DRY RUN:
  4 5
  0 1 2 1
  0 2 1 2
  1 2 1 1
  1 3 1 3
  2 3 2 1
  0 3
  augmentation 1: 0 -> 2 -> 3, cost 3 per unit, push 1
                  (0->1->2->3 also costs 3; edge order breaks the tie)
  augmentation 2: 0 -> 1 -> 2 -> 3, cost 3, push 1
  augmentation 3: 0 -> 1 -> 3, cost 4, push 1
  maximum flow 3, minimum cost 3 + 3 + 4 = 10
  edge flows: 0->1 2, 0->2 1, 1->2 1, 1->3 1, 2->3 2

SPECIAL CASES:
  - If t is unreachable the flow and cost are 0.
  - Parallel and antiparallel edges keep separate residual pairs.
  - Zero-cost edges are fine; NEGATIVE input costs are rejected
    because they could form a negative cycle (that needs cycle
    cancelling first).
  - Path costs are non-decreasing from one augmentation to the next -
    a quick sanity check when debugging.
  - 2 <= n <= 100, edges <= 1000, capacity 0..10^6, cost 0..10^6;
    source and sink must differ.

COMPLEXITY:
  O(F * V * E) with Bellman-Ford, where F is the number of
  augmentations (at most the total flow). Using Dijkstra with vertex
  potentials (Johnson's trick) gives O(F * E log V).

TIPS:
  To send exactly D units instead of the maximum, stop when the flow
  reaches D (push min(f, D - flow) on the last path). Compare
  dinic-max-flow (flow only, no costs) and assignment-bitmask-dp (the
  same assignment problem, exponential but tiny code).
*/

#include <stdio.h>

#define MAX_V 100
#define MAX_E 1000
#define LIMIT 1000000LL
#define INF 1000000000000000000LL

typedef struct {
    int from;
    int to;
    long long residual;
    long long cost;
} Edge;

Edge edges[2 * MAX_E];
long long originalCapacity[MAX_E];
int edgeCount;
int n;

void addEdge(int u, int v, long long capacity, long long cost) {
    edges[edgeCount] = (Edge){u, v, capacity, cost};
    edges[edgeCount + 1] = (Edge){v, u, 0, -cost};
    edgeCount += 2;
}

int main(void) {
    int m;
    int source;
    int sink;
    long long dist[MAX_V];
    int inEdge[MAX_V];

    printf("Min-Cost Max-Flow (Successive Shortest Paths)\n\n");
    printf("Vertices n (2-%d) and edges m (0-%d): ", MAX_V, MAX_E);
    if (scanf("%d %d", &n, &m) != 2 || n < 2 || n > MAX_V || m < 0 ||
        m > MAX_E) {
        printf("Invalid size\n");
        return 1;
    }
    for (int i = 0; i < m; i++) {
        int u;
        int v;
        long long capacity;
        long long cost;
        printf("Edge %d: from to capacity cost: ", i + 1);
        if (scanf("%d %d %lld %lld", &u, &v, &capacity, &cost) != 4 || u < 0 ||
            u >= n || v < 0 || v >= n || capacity < 0 || capacity > LIMIT ||
            cost < 0 || cost > LIMIT) {
            printf("Invalid edge\n");
            return 1;
        }
        originalCapacity[i] = capacity;
        addEdge(u, v, capacity, cost);
    }
    printf("Source and sink: ");
    if (scanf("%d %d", &source, &sink) != 2 || source < 0 || source >= n ||
        sink < 0 || sink >= n || source == sink) {
        printf("Invalid source/sink\n");
        return 1;
    }

    long long flow = 0;
    long long totalCost = 0;
    int round = 0;
    printf("\n");
    for (;;) {
        for (int v = 0; v < n; v++) {
            dist[v] = INF;
            inEdge[v] = -1;
        }
        dist[source] = 0;
        for (int pass = 0; pass < n - 1; pass++) {
            int changed = 0;
            for (int e = 0; e < edgeCount; e++) {
                Edge *edge = &edges[e];
                if (edge->residual > 0 && dist[edge->from] < INF &&
                    dist[edge->from] + edge->cost < dist[edge->to]) {
                    dist[edge->to] = dist[edge->from] + edge->cost;
                    inEdge[edge->to] = e;
                    changed = 1;
                }
            }
            if (!changed) break;
        }
        if (dist[sink] >= INF) break;

        long long push = INF;
        for (int v = sink; v != source; v = edges[inEdge[v]].from) {
            if (edges[inEdge[v]].residual < push) push = edges[inEdge[v]].residual;
        }
        int path[MAX_V];
        int length = 0;
        for (int v = sink; v != source; v = edges[inEdge[v]].from) {
            edges[inEdge[v]].residual -= push;
            edges[inEdge[v] ^ 1].residual += push;
            path[length++] = v;
        }
        flow += push;
        totalCost += push * dist[sink];
        round++;
        printf("augmentation %d: %d", round, source);
        for (int i = length - 1; i >= 0; i--) printf(" -> %d", path[i]);
        printf(", cost %lld per unit, push %lld\n", dist[sink], push);
    }

    printf("\nMaximum flow %lld, minimum cost %lld\n", flow, totalCost);
    printf("Edge flows:\n");
    for (int i = 0; i < m; i++) {
        Edge *edge = &edges[2 * i];
        long long used = originalCapacity[i] - edge->residual;
        printf("  %d -> %d: %lld / %lld (cost %lld each)\n", edge->from,
               edge->to, used, originalCapacity[i], edge->cost);
    }
    return 0;
}
