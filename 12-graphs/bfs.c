/*
 PROBLEM DEFINITION:
 --------------------
 Breadth-First Search (BFS) is a graph traversal algorithm that explores
 vertices level by level, starting from a source vertex:
 - First visits all vertices at distance 1 (direct neighbors)
 - Then all vertices at distance 2, and so on
 It uses a QUEUE (FIFO) to keep track of the next vertex to visit and a
 VISITED array to avoid revisiting vertices (prevents infinite loops in
 graphs with cycles).

 BFS also computes the SHORTEST PATH (in number of edges) from the source
 to every other vertex in an unweighted graph.

 REAL-LIFE EXAMPLE - Social Network Friend Suggestions (Facebook/LinkedIn):
 ---------------------------------------------------------------------------
 When Facebook suggests "People You May Know", it works like BFS:
 - Level 0: You
 - Level 1: Your direct friends (mutual friends = 1)
 - Level 2: Friends of your friends (mutual friends = 2, shown as suggestions)
 The suggestion engine explores outward level by level, exactly how BFS
 expands a frontier ring by ring around the starting person.
 Another example: a rumor spreading in a crowd - it spreads to your direct
 friends first, then their friends, ring by ring.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of BFS as ripples spreading on water from where a stone drops:

 1. SETUP:
    - Mark all vertices as NOT visited.
    - Create an empty queue and enqueue the source vertex.
    - Mark the source as visited (enqueue-time marking prevents duplicates).

 2. MAIN LOOP - Expand the frontier ring by ring:
    - Dequeue the front vertex U.
    - Process/print U.
    - For every neighbor V of U (in increasing order):
        If V is not visited:
            Mark V as visited
            Enqueue V
            (Optionally record distance: dist[V] = dist[U] + 1)

 3. TERMINATION:
    - Stop when the queue becomes empty.
    - Every vertex reachable from the source has now been visited.

 KEY PROPERTY:
    The first time BFS reaches a vertex, it has found the minimum number
    of edges from the source (shortest path in unweighted graphs).

 PSEUDO CODE:
 ------------
 START
   Function BFS(graph, source):
     Create empty Queue Q
     Mark source as visited
     Enqueue source into Q

     While Q is not empty:
         U = Dequeue(Q)
         Process(U)

         For each neighbor V of U:
             If V is not visited:
                 Mark V as visited
                 dist[V] = dist[U] + 1
                 parent[V] = U
                 Enqueue V into Q
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Graph (6 vertices, 0 to 5) with edges:
   0 - 1, 0 - 2, 1 - 3, 2 - 4, 3 - 5, 4 - 5

 As a picture:
         0
        / \
       1   2
       |   |
       3   4
        \ /
         5

 BFS starting from vertex 0:

 Step 1: Enqueue 0.            Queue: [0]        Visited: {0}
 Step 2: Dequeue 0. Neighbors: 1, 2.
         Enqueue both.         Queue: [1, 2]     Visited: {0,1,2}
         Output: 0
 Step 3: Dequeue 1. Neighbor: 3 (0 already visited).
         Enqueue 3.            Queue: [2, 3]     Visited: {0,1,2,3}
         Output: 0 1
 Step 4: Dequeue 2. Neighbor: 4 (0 already visited).
         Enqueue 4.            Queue: [3, 4]     Visited: {0,1,2,3,4}
         Output: 0 1 2
 Step 5: Dequeue 3. Neighbor: 5.
         Enqueue 5.            Queue: [4, 5]     Visited: {0,1,2,3,4,5}
         Output: 0 1 2 3
 Step 6: Dequeue 4. Neighbor: 5 (already visited - skip).
                               Queue: [5]
         Output: 0 1 2 3 4
 Step 7: Dequeue 5. No new neighbors.
                               Queue: []         DONE
         Output: 0 1 2 3 4 5

 Shortest distances from 0:
   dist[0]=0, dist[1]=1, dist[2]=1, dist[3]=2, dist[4]=2, dist[5]=3

 TIME COMPLEXITY:
 - Adjacency Matrix: O(V^2) - scans a full row for every vertex
 - Adjacency List:   O(V + E) - visits each vertex and edge once

 SPACE COMPLEXITY: O(V) for the queue, visited array, and distance array

 COMMON USE CASES:
 - Shortest path in unweighted graphs (GPS with equal-cost edges)
 - Social networks: degrees of separation, friend suggestions
 - Web crawlers exploring pages level by level
 - Peer-to-peer networks (finding closest node)
 - Broadcasting in networks, garbage collection (mark phase)
DIAGRAM (BFS from A - explore in rings):
       A
      / \
     B   C        queue: A -> B C -> D E F
     |   |        distance: A=0, B=C=1, D=E=F=2
     D   E
      \ /
       F

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

/*
 * Graph Structure:
 * Uses an ADJACENCY MATRIX representation.
 * adj[u][v] = 1 means an edge exists between u and v.
 * Matrix form keeps the code simple for learning; adjacency lists are
 * preferred for sparse graphs (see complexity notes in the header).
 */
typedef struct {
    int vertices;
    int adj[MAX_VERTICES][MAX_VERTICES];
} Graph;

/*
 * Queue Structure:
 * A simple circular-buffer-free linear queue sized to MAX_VERTICES.
 * BFS enqueues each vertex at most once, so capacity V is sufficient.
 */
typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

/*
 * initGraph - Initializes a graph with given number of vertices.
 */
void initGraph(Graph* g, int vertices) {
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
}

/*
 * addEdge - Adds an undirected edge between u and v.
 * Undirected means the friendship/connection works both ways.
 */
void addEdge(Graph* g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

/*
 * initQueue - Resets the queue to empty state.
 */
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
}

/*
 * isEmpty - Checks whether the queue has no elements.
 */
int isEmpty(Queue* q) {
    return q->front > q->rear;
}

/*
 * enqueue - Adds a vertex at the rear of the queue.
 */
void enqueue(Queue* q, int value) {
    if (q->rear < MAX_VERTICES - 1) {
        q->items[++q->rear] = value;
    }
}

/*
 * dequeue - Removes and returns the vertex at the front of the queue.
 * This FIFO ordering is what makes BFS level-by-level.
 */
int dequeue(Queue* q) {
    if (!isEmpty(q)) {
        return q->items[q->front++];
    }
    return -1;
}

/*
 * bfs - Performs Breadth-First Search from a source vertex.
 * Prints traversal order and shortest distances (edge count).
 *
 * Analogy: Rumor spreading ring by ring through a crowd of friends.
 */
void bfs(Graph* g, int source) {
    int visited[MAX_VERTICES] = {0};
    int distance[MAX_VERTICES];
    int parent[MAX_VERTICES];
    int order[MAX_VERTICES];
    int orderCount = 0;
    Queue q;

    for (int i = 0; i < g->vertices; i++) {
        distance[i] = -1;
        parent[i] = -1;
    }

    initQueue(&q);

    printf("  Starting BFS from vertex %d\n\n", source);

    visited[source] = 1;
    distance[source] = 0;
    enqueue(&q, source);
    printf("  Enqueue source %d (dist=0). Queue: [%d]\n", source, source);

    int step = 1;

    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        order[orderCount++] = u;
        printf("\n  Step %d: Dequeue %d (dist=%d) -> OUTPUT %d", step, u, distance[u], u);
        printf("\n         Queue now: [");
        for (int i = q.front; i <= q.rear; i++) {
            printf("%d", q.items[i]);
            if (i < q.rear) printf(", ");
        }
        printf("]\n");

        for (int v = 0; v < g->vertices; v++) {
            if (g->adj[u][v] == 1 && !visited[v]) {
                visited[v] = 1;
                distance[v] = distance[u] + 1;
                parent[v] = u;
                enqueue(&q, v);
                printf("           Neighbor %d not visited -> mark, enqueue (dist=%d, parent=%d)\n",
                       v, distance[v], u);
            }
        }
        step++;
    }

    printf("\n  BFS Traversal Order: ");
    for (int i = 0; i < orderCount; i++) {
        printf("%d ", order[i]);
    }
    printf("\n");

    printf("\n  Shortest distances from vertex %d (in edges):\n", source);
    for (int i = 0; i < g->vertices; i++) {
        printf("    Vertex %d: distance = %d, parent = %d\n", i, distance[i], parent[i]);
    }
}

/*
 * printPath - Reconstructs the shortest path from source to target
 * using the parent[] array filled during BFS.
 */
void printPath(int parent[], int target) {
    if (parent[target] == -1) {
        printf("%d", target);
        return;
    }
    printPath(parent, parent[target]);
    printf(" -> %d", target);
}

int main() {
    Graph g;
    int v;
    int e;
    int source;

    printf("Breadth-First Search (BFS) - Graph Traversal\n\n");

    printf("How many vertices (1-%d)? ", MAX_VERTICES);
    scanf("%d", &v);

    if (v < 1 || v > MAX_VERTICES) {
        printf("Invalid vertex count\n");
        return 1;
    }

    initGraph(&g, v);

    printf("How many undirected edges? ");
    scanf("%d", &e);

    if (e < 0) {
        printf("Invalid edge count\n");
        return 1;
    }

    printf("Enter %d edges as 'u v' (both directions added), vertices 0..%d:\n", e, v - 1);
    for (int i = 0; i < e; i++) {
        int u;
        int w;
        scanf("%d %d", &u, &w);
        if (u < 0 || u >= v || w < 0 || w >= v) {
            printf("Invalid edge %d %d - skipped\n", u, w);
            continue;
        }
        addEdge(&g, u, w);
    }

    printf("Enter source vertex for BFS (0..%d): ", v - 1);
    scanf("%d", &source);

    if (source < 0 || source >= v) {
        printf("Invalid source vertex\n");
        return 1;
    }

    printf("\nAdjacency Matrix:\n");
    printf("    ");
    for (int i = 0; i < g.vertices; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < g.vertices; i++) {
        printf("  %d ", i);
        for (int j = 0; j < g.vertices; j++) {
            printf("%d ", g.adj[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* STEP: BFS from the chosen source with step-by-step trace */
    bfs(&g, source);

    /* STEP: BFS from a different source vertex */
    int other = (source + 1) % g.vertices;
    printf("\nRunning BFS from vertex %d to show source-independence:\n\n", other);
    bfs(&g, other);

    return 0;
}
