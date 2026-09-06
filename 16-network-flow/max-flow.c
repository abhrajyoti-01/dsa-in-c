/*
Max Flow - Ford-Fulkerson with Edmonds-Karp (BFS)
-------------------------------------------------
What this program solves:
Finds the MAXIMUM FLOW from source s to sink t in a flow network
- how much "stuff" (water, data, traffic) can move through a
network of pipes with capacities.

Real life example:
Internet traffic engineering and logistics: how much flow per
second can pipes/links carry from source to destination? Min-cut
identifies the bottleneck links.

BEGINNER EXPLANATION (the simplest way to think):
Think of water pipes with capacities. Send ANY path of water from
s to t (a "path" = every pipe on it has spare capacity). Push as
much as the tightest pipe allows. Then crucially: for every pipe
you used, add a REVERSE pipe ("undo" capability). Repeat finding
paths. When NO more augmenting paths exist, the total you pushed
is the maximum flow. The reverse edges let a later path "reroute"
earlier decisions - that is the entire magic.

SUPER SIMPLE EXAMPLE:
s->a cap 10, a->t cap 5. Path s-a-t: push 5 (bottleneck).
No more paths (a->t saturated). Max flow = 5.
The bottleneck (min capacity along path) limits the push.

DIAGRAM (the two-pipe network):
          10
    s --------> a
                |
                | 5
                v
    t <--------
          5
Path s-a-t: pipes carry 10 and 5 -> bottleneck 5 -> push 5.
Residual: s->a has 5 left, a->t has 0 left, a->s gets +5.
No other path (a->t full) -> MAX FLOW = 5.
Min-cut: {a->t} is the cut - its capacity (5) equals the flow.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Cross edges can BLOCK direct paths - and reverse edges fix it:
  Classic example: s->a=10, s->b=10, a->b=1(?), a->t=10, b->t=10.
  A greedy path s-a-t then s-b-t already gets 20, but if the first
  path chosen was s-a-b-t (cap 1), pushing 1 there would waste
  capacity - the reverse edge b->a lets a later path s-b-a-t push
  10 through, effectively REROUTING that 1. Total is still optimal.
  Without reverse edges, a greedy first choice could get stuck at
  a suboptimal flow.
- Parallel/zero-capacity edges: zero capacity = unusable pipe.
- Disconnected sink: flow = 0.
- The 0/1 capacity case: max flow = max number of edge-disjoint
  paths (that is how bipartite matching works - next file).

DRY RUN (Edmonds-Karp = BFS always picks the SHORTEST path):
Graph: s->a(10), s->b(5), a->t(10), b->a(15), b->t(5)
BFS 1: shortest path s-b-t (2 edges), bottleneck 5 -> flow 5.
       reverse edges t->b(5), b->s(5) appear.
BFS 2: shortest path s-a-t, bottleneck 10 -> flow 15.
BFS 3: shortest path s-b-a-t, bottleneck min(0?,...) - s->b is
       saturated (5 used of 5) -> NO augmenting path. Stop.
Max flow = 15. Note how s-b-a-t used the reverse edge a->b? No
need here - but it would if BFS 1 had picked s-a-b-t.

TIPS:
- Edmonds-Karp = Ford-Fulkerson where BFS (shortest augmenting
  path) guarantees O(V*E^2) - the path choice cannot run away
  like pure DFS with huge capacities can.
- Max-flow = Min-cut theorem: the flow value equals the smallest
  set of pipes whose removal disconnects t from s. Interview gold.
- Implementation trick: store capacity AND flow in one matrix;
  residual = cap - flow; reverse edge capacity = flow pushed.
*/

#include <stdio.h>
#include <string.h>

#define MAX_V 30

int capacity[MAX_V][MAX_V];
int residual[MAX_V][MAX_V];
int parent[MAX_V];
int visited[MAX_V];
int vCount;

/*
 * bfsAugment - Finds shortest augmenting path; fills parent[].
 */
int bfsAugment(int s, int t) {
    int queue[MAX_V];
    int front = 0;
    int rear = 0;

    memset(visited, 0, sizeof(visited));
    visited[s] = 1;
    parent[s] = -1;
    queue[rear] = s;
    rear = rear + 1;

    while (front < rear) {
        int u = queue[front];
        front = front + 1;
        for (int v = 0; v < vCount; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                visited[v] = 1;
                parent[v] = u;
                queue[rear] = v;
                rear = rear + 1;
                if (v == t) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int main() {
    int e;
    int u;
    int v;
    int cap;
    int source;
    int sink;

    printf("Max Flow (Ford-Fulkerson / Edmonds-Karp)\n\n");

    printf("How many vertices (1-%d)? ", MAX_V);
    scanf("%d", &vCount);

    if (vCount < 1 || vCount > MAX_V) {
        printf("Invalid count\n");
        return 1;
    }

    printf("How many directed edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'u v capacity' (vertex 0 = source):\n", e);
    for (int i = 0; i < e; i++) {
        scanf("%d %d %d", &u, &v, &cap);
        if (u < 0 || u >= vCount || v < 0 || v >= vCount || cap < 0) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        capacity[u][v] = capacity[u][v] + cap;
    }

    printf("Source vertex: ");
    scanf("%d", &source);
    printf("Sink vertex: ");
    scanf("%d", &sink);

    if (source < 0 || source >= vCount || sink < 0 || sink >= vCount) {
        printf("Invalid source/sink\n");
        return 1;
    }

    memcpy(residual, capacity, sizeof(capacity));

    int maxFlow = 0;
    int round = 0;

    /* STEP: repeatedly find augmenting path, push bottleneck */
    while (bfsAugment(source, sink)) {
        int bottleneck = 1000000000;
        for (int v = sink; v != source; v = parent[v]) {
            if (residual[parent[v]][v] < bottleneck) {
                bottleneck = residual[parent[v]][v];
            }
        }

        for (int v = sink; v != source; v = parent[v]) {
            int p = parent[v];
            residual[p][v] = residual[p][v] - bottleneck;
            residual[v][p] = residual[v][p] + bottleneck;
        }

        maxFlow = maxFlow + bottleneck;
        round = round + 1;
        printf("Round %d: path %d", round, sink);
        for (int v = sink; v != source; v = parent[v]) {
            printf(" <- %d", parent[v]);
        }
        printf(", pushed %d, total flow %d\n", bottleneck, maxFlow);
    }

    printf("\nMaximum flow from %d to %d = %d\n", source, sink, maxFlow);

    /* STEP: min-cut vertices = reachable in final residual graph */
    bfsAugment(source, sink);
    printf("Min-cut edges (s-side -> t-side, saturated):\n");
    for (int i = 0; i < vCount; i++) {
        if (visited[i]) {
            for (int j = 0; j < vCount; j++) {
                if (!visited[j] && capacity[i][j] > 0 &&
                    residual[i][j] == 0) {
                    printf("  %d -> %d (capacity %d)\n",
                           i, j, capacity[i][j]);
                }
            }
        }
    }

    return 0;
}
