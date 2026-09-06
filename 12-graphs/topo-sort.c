/*
 PROBLEM DEFINITION:
 --------------------
 Topological Sorting produces a LINEAR ordering of vertices in a Directed
 Acyclic Graph (DAG) such that for every directed edge U -> V, vertex U
 comes before V in the ordering. It is impossible if the graph has a cycle
 (which is why it only applies to DAGs).

 Two standard methods:
   1. DFS-based:  output vertices in decreasing order of finish time.
   2. Kahn's:     repeatedly remove vertices with in-degree 0.

 REAL-LIFE EXAMPLE - Course Prerequisites & Build Systems:
 ----------------------------------------------------------
 University enrollment is a classic DAG: "Data Structures" requires
 "Intro to Programming"; "Algorithms" requires "Data Structures". A
 topological order is a valid semester plan where every course appears
 AFTER its prerequisites. The same idea drives:
   - Make/Gradle build systems (compile dependencies before dependents)
   - Task/job scheduling in workflow engines (Airflow DAGs)
   - Package managers (install dependencies first)
   - Compiler instruction ordering

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Kahn's Algorithm is like clearing a to-do list where tasks unlock others:

 1. COMPUTE IN-DEGREES:
    - For every vertex, count how many edges point INTO it.
    - In-degree 0 means "no prerequisites - ready to do now".

 2. SEED THE QUEUE:
    - Enqueue all vertices with in-degree 0.

 3. PROCESS THE QUEUE:
    - Dequeue a vertex U, add it to the result order.
    - For each neighbor V of U (U -> V):
        Decrease V's in-degree by 1 (one prerequisite satisfied).
        If V's in-degree becomes 0, enqueue V.

 4. CYCLE CHECK:
    - If the result contains all V vertices -> valid topological order.
    - If fewer than V -> a cycle exists (some vertices never freed up).

 WHY IT WORKS:
    A vertex is emitted only after ALL its prerequisites are emitted,
    so the edge-before-vertex rule is satisfied everywhere.

 PSEUDO CODE:
 ------------
 START
   Function TopologicalSort(graph):
     Compute in-degree for every vertex
     Q = empty queue
     For each vertex v:
         If inDegree[v] == 0: enqueue v

     order = empty list
     count = 0

     While Q not empty:
         U = dequeue Q
         Add U to order
         count = count + 1
         For each neighbor V of U:
             inDegree[V] = inDegree[V] - 1
             If inDegree[V] == 0: enqueue V

     If count == number of vertices:
         Return order
     Else:
         Report "Graph has a cycle - no topological order"
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 DAG with 6 vertices (course prerequisites):
   5 -> 2,  5 -> 0,  4 -> 0,  4 -> 1,  2 -> 3,  3 -> 1

 In-degrees:
   vertex 0: 2 (from 5, 4)
   vertex 1: 2 (from 4, 3)
   vertex 2: 1 (from 5)
   vertex 3: 1 (from 2)
   vertex 4: 0
   vertex 5: 0

 Kahn's execution:
   Start queue: [4, 5] (in-degree 0)
   Dequeue 4 -> order [4]; free 0 (indeg 1), free 1 (indeg 1)
   Dequeue 5 -> order [4,5]; free 2 (indeg 0 -> enqueue), free 0 (indeg 0 -> enqueue)
   Queue: [2, 0]
   Dequeue 2 -> order [4,5,2]; free 3 (indeg 0 -> enqueue)
   Dequeue 0 -> order [4,5,2,0]
   Queue: [3]
   Dequeue 3 -> order [4,5,2,3]; free 1 (indeg 0 -> enqueue)
   Queue: [1]
   Dequeue 1 -> order [4,5,2,3,0,1]

 All 6 vertices emitted -> valid topological order.
 (Note: topological order is not unique; [5,4,2,0,3,1] is also valid.)

 TIME COMPLEXITY: O(V + E) - each vertex and edge processed once
 SPACE COMPLEXITY: O(V) for queue, in-degree array, and result

 COMMON USE CASES:
 - Course prerequisite planning
 - Build systems (Make, Gradle, Bazel)
 - Task scheduling in workflow engines
 - Resolving package dependencies (npm, pip)
 - Detecting circular dependencies
DIAGRAM (DFS finish order reversed = topological order):
   A -> B -> C      finish: C, B, A (reverse) -> A B C
   A -> C           every edge points FORWARD in the final order.

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

/*
 * Graph Structure:
 * Directed graph using an adjacency list for efficiency.
 * Each vertex keeps a linked list of its outgoing neighbors.
 */
typedef struct AdjNode {
    int vertex;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    int vertices;
    AdjNode* adjList[MAX_VERTICES];
} Graph;

/*
 * initGraph - Creates an empty directed graph.
 */
void initGraph(Graph* g, int vertices) {
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        g->adjList[i] = NULL;
    }
}

/*
 * addDirectedEdge - Adds a one-way edge from u to v (u must come first).
 *
 * Analogy: "u is a prerequisite for v".
 */
void addDirectedEdge(Graph* g, int u, int v) {
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->vertex = v;
    newNode->next = g->adjList[u];
    g->adjList[u] = newNode;
}

/*
 * computeInDegrees - Counts incoming edges for every vertex.
 * In-degree 0 means "ready to process now".
 */
void computeInDegrees(Graph* g, int inDegree[]) {
    for (int i = 0; i < g->vertices; i++) {
        inDegree[i] = 0;
    }
    for (int u = 0; u < g->vertices; u++) {
        AdjNode* current = g->adjList[u];
        while (current != NULL) {
            inDegree[current->vertex]++;
            current = current->next;
        }
    }
}

/*
 * topologicalSortKahn - Kahn's algorithm (BFS-style, in-degree based).
 * Returns the number of vertices successfully ordered.
 * If that number is less than total vertices, a cycle exists.
 */
int topologicalSortKahn(Graph* g, int order[]) {
    int inDegree[MAX_VERTICES];
    computeInDegrees(g, inDegree);

    int queue[MAX_VERTICES];
    int front = 0, rear = -1;

    for (int i = 0; i < g->vertices; i++) {
        if (inDegree[i] == 0) {
            queue[++rear] = i;
        }
    }

    int count = 0;

    while (front <= rear) {
        int u = queue[front++];
        order[count++] = u;

        AdjNode* current = g->adjList[u];
        while (current != NULL) {
            int v = current->vertex;
            inDegree[v]--;
            if (inDegree[v] == 0) {
                queue[++rear] = v;
            }
            current = current->next;
        }
    }

    return count;
}

/*
 * topologicalSortDFS - Alternative DFS-based method.
 * Pushes vertices onto a stack by finish time; popping gives the order.
 */
void dfsTopoUtil(Graph* g, int u, int visited[], int stack[], int* top) {
    visited[u] = 1;

    AdjNode* current = g->adjList[u];
    while (current != NULL) {
        if (!visited[current->vertex]) {
            dfsTopoUtil(g, current->vertex, visited, stack, top);
        }
        current = current->next;
    }

    stack[(*top)++] = u;
}

void topologicalSortDFS(Graph* g, int order[]) {
    int visited[MAX_VERTICES] = {0};
    int stack[MAX_VERTICES];
    int top = 0;

    for (int i = 0; i < g->vertices; i++) {
        if (!visited[i]) {
            dfsTopoUtil(g, i, visited, stack, &top);
        }
    }

    int idx = 0;
    while (top > 0) {
        order[idx++] = stack[--top];
    }
}

int main() {
    Graph g;
    int v;
    int e;

    printf("Topological Sort - Ordering a DAG\n\n");

    printf("How many vertices (1-%d)? ", MAX_VERTICES);
    scanf("%d", &v);

    if (v < 1 || v > MAX_VERTICES) {
        printf("Invalid vertex count\n");
        return 1;
    }

    initGraph(&g, v);

    printf("How many directed edges? ");
    scanf("%d", &e);

    if (e < 0) {
        printf("Invalid edge count\n");
        return 1;
    }

    printf("Enter %d directed edges as 'u v' (u -> v), vertices 0..%d:\n", e, v - 1);
    for (int i = 0; i < e; i++) {
        int u;
        int w;
        scanf("%d %d", &u, &w);
        if (u < 0 || u >= v || w < 0 || w >= v) {
            printf("Invalid edge %d %d - skipped\n", u, w);
            continue;
        }
        addDirectedEdge(&g, u, w);
    }

    printf("\nDAG edges (u -> v means u before v):\n");
    for (int i = 0; i < v; i++) {
        for (AdjNode* node = g.adjList[i]; node != NULL; node = node->next) {
            printf("%d->%d ", i, node->vertex);
        }
    }
    printf("\n");

    /* STEP: Kahn's algorithm (in-degree based) */
    int order[MAX_VERTICES];
    int count = topologicalSortKahn(&g, order);

    if (count == g.vertices) {
        printf("Topological order (Kahn's): ");
        for (int i = 0; i < count; i++) {
            printf("%d", order[i]);
            if (i < count - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("Graph has a cycle - no topological order exists\n");
        return 0;
    }

    /* STEP: DFS-based method (finish-time stack) */
    int order2[MAX_VERTICES];
    topologicalSortDFS(&g, order2);

    printf("Topological order (DFS): ");
    for (int i = 0; i < g.vertices; i++) {
        printf("%d", order2[i]);
        if (i < g.vertices - 1) printf(" -> ");
    }
    printf("\n(Both orders are valid - topological order is not unique)\n");

    return 0;
}
