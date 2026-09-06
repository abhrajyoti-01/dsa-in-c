/*
 PROBLEM DEFINITION:
 --------------------
 Depth-First Search (DFS) is a graph traversal algorithm that explores
 as FAR as possible along one branch before BACKTRACKING:
 - Start at a source vertex, move to an unvisited neighbor
 - Repeat recursively until a dead end is reached
 - Backtrack to the last vertex with unexplored neighbors and continue
 It uses a STACK (explicitly, or the function-call stack via recursion)
 and a VISITED array to avoid infinite loops in cyclic graphs.

 DFS naturally produces a DFS TREE/FOREST and classifies edges; it is
 the backbone for cycle detection, topological sorting, and connectivity.

 REAL-LIFE EXAMPLE - Solving a Maze (or Escape Room):
 -----------------------------------------------------
 Imagine walking through a hedge maze:
 - Keep your hand on the left wall and walk forward (choose one path).
 - Follow corridors deeply until you hit a dead end.
 - Turn around and backtrack to the last junction with an untried path.
 - Repeat until you find the exit.
 This "go deep first, retreat when stuck" strategy is exactly DFS.
 Explorers, spelunkers mapping caves, and even game AI searching level
 maps use this depth-first exploration strategy.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of DFS as exploring a building floor by floor, room by room,
 without skipping - and marking every door already used:

 1. START:
    - Mark the source vertex as visited and output it.

 2. GO DEEPER (the recursion):
    - For each neighbor V of the current vertex U (in increasing order):
        If V is not visited:
            Recurse: DFS(V)
    The call stack remembers where we came from (our "breadcrumb trail").

 3. BACKTRACK:
    - When a vertex has no unvisited neighbors, the function returns.
    - Control pops back to the previous vertex, which tries its next
      unvisited neighbor.

 4. TERMINATION:
    - Done when every vertex reachable from the source is visited.
    - For DISCONNECTED graphs, loop over all vertices and start a new
      DFS from any unvisited one.

 KEY PROPERTIES:
    - Discovery and finish times define intervals (used in topological sort).
    - Back edges reveal CYCLES in the graph.
    - Recursion depth can reach O(V) - deep graphs may need an iterative
      stack-based version to avoid stack overflow.

 PSEUDO CODE:
 ------------
 START
   Function DFS(graph, u, visited):
     visited[u] = TRUE
     Process(u)

     For each neighbor V of u:
         If visited[V] == FALSE:
             DFS(graph, V, visited)

   Function DFSTraversal(graph):
     Mark all vertices as not visited
     For each vertex s in graph:
         If visited[s] == FALSE:
             DFS(graph, s, visited)

   Iterative version (explicit stack):
   Function DFSIterative(graph, source):
     Create empty Stack S
     Push source onto S
     While S is not empty:
         U = Pop(S)
         If U is not visited:
             Mark U visited and Process(U)
             Push all unvisited neighbors of U onto S
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

 DFS starting from vertex 0 (neighbors explored in increasing order):

 Step 1: Visit 0. Output: 0        Call stack: [0]
 Step 2: Neighbor 1 unvisited -> go deeper.
         Visit 1. Output: 0 1      Call stack: [0, 1]
 Step 3: Neighbor of 1 is 3 (0 visited) -> go deeper.
         Visit 3. Output: 0 1 3    Call stack: [0, 1, 3]
 Step 4: Neighbor of 3 is 5 (1 visited) -> go deeper.
         Visit 5. Output: 0 1 3 5  Call stack: [0, 1, 3, 5]
 Step 5: Neighbors of 5: 3, 4 (3 visited; 4 unvisited) -> go deeper.
         Visit 4. Output: 0 1 3 5 4  Call stack: [0, 1, 3, 5, 4]
 Step 6: Neighbors of 4: 2 (unvisited), 5 -> go deeper.
         Visit 2. Output: 0 1 3 5 4 2   Call stack: [..., 4, 2]
 Step 7: Neighbors of 2: 0, 4 - both visited. Dead end -> BACKTRACK.
         Pop 2 -> Pop 4 -> Pop 5 -> Pop 3 -> Pop 1 -> Pop 0.
 Step 8: All neighbors of 0 visited. Stack empty. DONE.

 DFS Traversal Order: 0 1 3 5 4 2
 (Notice it goes DEEP to 5 before visiting 2 - unlike BFS's 0 1 2 3 4 5)

 TIME COMPLEXITY:
 - Adjacency Matrix: O(V^2) - full row scan per vertex
 - Adjacency List:   O(V + E) - each vertex and edge processed once

 SPACE COMPLEXITY: O(V) for visited array + recursion/stack depth

 COMMON USE CASES:
 - Maze and puzzle solving (backtracking problems)
 - Cycle detection in graphs (back edges)
 - Topological sorting (via finish times, on DAGs)
 - Finding connected components and strongly connected components
 - Path finding (does a path exist between two nodes?)
 - Solving games: exploring all possible move sequences
DIAGRAM (DFS dives deep first, then backtracks):
       A
      / \
     B   C       visit A -> B -> D (dead end, backtrack)
     |           -> E -> back to B -> back to A -> C -> F
     D-E  \      order: A B D E C F
            F

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

/*
 * Graph Structure:
 * Uses an ADJACENCY MATRIX representation for clarity.
 * adj[u][v] = 1 means an edge exists between u and v.
 */
typedef struct {
    int vertices;
    int adj[MAX_VERTICES][MAX_VERTICES];
} Graph;

/*
 * Stack Structure:
 * Used for the ITERATIVE version of DFS.
 * The recursive version uses the implicit function-call stack instead.
 */
typedef struct {
    int items[MAX_VERTICES];
    int top;
} Stack;

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
 */
void addEdge(Graph* g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

/*
 * initStack - Resets the stack to empty state.
 */
void initStack(Stack* s) {
    s->top = -1;
}

/*
 * isEmpty - Checks whether the stack has no elements.
 */
int isEmpty(Stack* s) {
    return s->top == -1;
}

/*
 * push - Adds a vertex on top of the stack.
 */
void push(Stack* s, int value) {
    if (s->top < MAX_VERTICES - 1) {
        s->items[++s->top] = value;
    }
}

/*
 * pop - Removes and returns the top vertex of the stack.
 * LIFO ordering is what drives DFS depth-first behavior.
 */
int pop(Stack* s) {
    if (!isEmpty(s)) {
        return s->items[s->top--];
    }
    return -1;
}

/*
 * dfsRecursive - The heart of DFS: go deep, backtrack on dead ends.
 *
 * Analogy: Walking a maze with your hand on the wall - forward until a
 * dead end, then retreat to the last junction with an untried path.
 */
void dfsRecursive(Graph* g, int u, int visited[], int depth) {
    visited[u] = 1;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("Visit %d -> OUTPUT %d (recursion depth %d)\n", u, u, depth);

    for (int v = 0; v < g->vertices; v++) {
        if (g->adj[u][v] == 1 && !visited[v]) {
            for (int i = 0; i < depth; i++) {
                printf("  ");
            }
            printf("  %d has unvisited neighbor %d -> go deeper\n", u, v);
            dfsRecursive(g, v, visited, depth + 1);
            for (int i = 0; i < depth; i++) {
                printf("  ");
            }
            printf("  Back at %d (backtracked from %d)\n", u, v);
        }
    }
}

/*
 * dfsIterative - DFS with an explicit stack (no recursion).
 * Useful for very deep graphs where recursion might overflow the stack.
 */
void dfsIterative(Graph* g, int source) {
    int visited[MAX_VERTICES] = {0};
    Stack s;
    initStack(&s);

    printf("\n  Iterative DFS from vertex %d:\n", source);
    printf("  (Uses explicit stack instead of recursion)\n\n");

    push(&s, source);

    while (!isEmpty(&s)) {
        int u = pop(&s);

        if (!visited[u]) {
            visited[u] = 1;
            printf("  Pop %d -> OUTPUT %d   Stack top is now: ", u, u);
            if (isEmpty(&s)) {
                printf("empty");
            } else {
                printf("%d", s.items[s.top]);
            }
            printf("\n");

            for (int v = g->vertices - 1; v >= 0; v--) {
                if (g->adj[u][v] == 1 && !visited[v]) {
                    push(&s, v);
                }
            }
        }
    }
    printf("\n");
}

/*
 * hasCycleUtil - Recursive helper that detects cycles.
 * For an UNDIRECTED graph, the edge back to the immediate parent is not
 * a cycle, so it is skipped via the parent parameter. Any other visited
 * vertex still in the recursion stack means a back edge -> cycle.
 */
int hasCycleUtil(Graph* g, int u, int visited[], int recStack[], int parent) {
    visited[u] = 1;
    recStack[u] = 1;

    for (int v = 0; v < g->vertices; v++) {
        if (g->adj[u][v] == 1) {
            if (!visited[v]) {
                if (hasCycleUtil(g, v, visited, recStack, u)) {
                    return 1;
                }
            } else if (v != parent && recStack[v]) {
                return 1;
            }
        }
    }

    recStack[u] = 0;
    return 0;
}

/*
 * hasCycle - Returns 1 if the graph contains a cycle, 0 otherwise.
 * Checks every component (graph may be disconnected).
 */
int hasCycle(Graph* g) {
    int visited[MAX_VERTICES] = {0};
    int recStack[MAX_VERTICES] = {0};

    for (int i = 0; i < g->vertices; i++) {
        if (!visited[i]) {
            if (hasCycleUtil(g, i, visited, recStack, -1)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    Graph g;
    int v;
    int e;
    int source;

    printf("Depth-First Search (DFS) - Graph Traversal\n\n");

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

    printf("Enter source vertex for DFS (0..%d): ", v - 1);
    scanf("%d", &source);

    if (source < 0 || source >= v) {
        printf("Invalid source vertex\n");
        return 1;
    }

    /* STEP: recursive DFS from the chosen source, with trace */
    int visited[MAX_VERTICES] = {0};
    dfsRecursive(&g, source, visited, 0);

    /* STEP: iterative DFS using an explicit stack */
    dfsIterative(&g, source);

    /* STEP: cycle detection on the same graph */
    printf("\nCycle detection on the entered graph: hasCycle = %s\n",
           hasCycle(&g) ? "YES" : "NO");

    return 0;
}
