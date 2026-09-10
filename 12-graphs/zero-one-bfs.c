/*
0-1 BFS - Shortest Paths with a Deque (Advanced)
------------------------------------------------
PROBLEM DEFINITION:
Find shortest distances from a source in a DIRECTED graph whose edge
weights are only 0 or 1. A deque replaces Dijkstra's priority queue.

REAL-LIFE EXAMPLE:
Travel along preferred roads costs 0; reversing a road costs 1.
The shortest distance measures the fewest reversals, not the most
geographically direct route.

DIAGRAM:
  0 --1--> 1 --1--> 3
  |        ^
  0        0
  v        |
  2 -------+
  From 0: distances [0,0,0,1]. Ordinary BFS would miss the cheaper detour.

PSEUDO CODE:
  dist[source] = 0; others = INF; push source
  While deque not empty:
      Pop front (u, savedDistance)
      Skip stale or already-finalized entries; finalize u
      For each edge u->v of weight w:
          If dist[u] + w improves dist[v]:
              Update dist[v]
              Push FRONT if w == 0, otherwise push BACK

SAMPLE INPUT & DRY RUN:
  4 4
  0 1 1
  0 2 0
  2 1 0
  1 3 1
  0
  Process 0: vertex 2 goes to front, vertex 1 to back.
  Process 2: improve dist[1] from 1 to 0, push it to front.
  Process 1: dist[3] = 1. The old (1,1) entry is stale and skipped.

SPECIAL CASES:
  - Zero-weight cycles, parallel edges, and self-loops are supported.
  - Unreachable vertices print "No path"; source distance is always 0.
  - Weights other than 0/1 are rejected; use Dijkstra/Bellman-Ford instead.
  - Vertices 0..n-1, n <= 200, edges <= 4000.
  - To model an undirected edge, enter both directions (counts as 2 edges).

COMPLEXITY:
  O(V+E) time and storage using adjacency lists. Each vertex is finalized
  once, each edge scanned once. At most E successful relaxations enqueue
  entries, so a deque of E+1 slots also accommodates stale entries.

TIPS:
  Do NOT mark visited on insertion: a later zero-cost route can improve
  a queued vertex. Finalize only when removed with its current distance.
*/

#include <stdio.h>

#define MAX_V 200
#define MAX_E 4000
#define DEQUE_SIZE (MAX_E + 1)
#define INF 1000000000

typedef struct {
    int to;
    int weight;
    int next;
} Edge;

typedef struct {
    int vertex;
    int distance;
} Entry;

Edge edges[MAX_E];
int head[MAX_V];
int dist[MAX_V];
int finalized[MAX_V];
Entry deque[DEQUE_SIZE];

void zeroOneBfs(int n, int source) {
    int front = 0;
    int count = 1;
    for (int i = 0; i < n; i++) dist[i] = INF;
    dist[source] = 0;
    deque[front] = (Entry){source, 0};

    while (count > 0) {
        Entry entry = deque[front];
        front = (front + 1) % DEQUE_SIZE;
        count--;
        int u = entry.vertex;
        if (entry.distance != dist[u] || finalized[u]) continue;
        finalized[u] = 1;
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            int w = edges[e].weight;
            if (!finalized[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                Entry next = {v, dist[v]};
                if (w == 0) {
                    front = (front + DEQUE_SIZE - 1) % DEQUE_SIZE;
                    deque[front] = next;
                } else {
                    deque[(front + count) % DEQUE_SIZE] = next;
                }
                count++;
            }
        }
    }
}

int main(void) {
    int n;
    int m;
    int source;
    printf("0-1 BFS - Directed Shortest Paths\n\n");
    printf("Vertices (1-%d) and edges (0-%d): ", MAX_V, MAX_E);
    if (scanf("%d %d", &n, &m) != 2 ||
        n < 1 || n > MAX_V || m < 0 || m > MAX_E) {
        printf("Invalid graph size\n");
        return 1;
    }
    for (int i = 0; i < n; i++) head[i] = -1;
    printf("Enter directed edges as 'u v weight' (weight 0 or 1):\n");
    for (int i = 0; i < m; i++) {
        int u;
        int v;
        int w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 ||
            u < 0 || u >= n || v < 0 || v >= n || (w != 0 && w != 1)) {
            printf("Invalid edge\n");
            return 1;
        }
        edges[i] = (Edge){v, w, head[u]};
        head[u] = i;
    }
    printf("Source vertex: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= n) {
        printf("Invalid source\n");
        return 1;
    }
    zeroOneBfs(n, source);
    printf("\nShortest distances from %d:\n", source);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) printf("To %d: No path\n", i);
        else printf("To %d: %d\n", i, dist[i]);
    }
    return 0;
}
