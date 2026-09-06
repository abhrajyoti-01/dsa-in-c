/*
Graph Problems Ladder - Cycle Detection & Representations (Basic -> Hard)
--------------------------------------------------------------------------
Real life example:
Build systems detect circular dependencies (cycle detection);
friend graphs and road maps are the adjacency structures shown.

DIAGRAM (adjacency matrix vs list; cycle via colors):
  matrix:  A B C     list: A -> B
  A [ 0 1 0 ]         B -> C -> A ... back-edge to a GRAY node
  B [ 0 0 1 ]           (in DFS) closes a cycle.
  C [ 1 0 0 ]  <- 1 at [C][A] means C->A edge exists

LADDER (easy -> hard inside this file):
  1. Adjacency matrix vs list (representation trade-off demo)
  2. Undirected cycle detection (DFS with parent - the classic)
  3. Directed cycle detection (3-color: white/gray/black)

BEGINNER EXPLANATION:
- Undirected cycle: DFS; if a neighbor is visited AND is not my
  parent, someone else already reached it - a second path = cycle.
- Directed cycle: colors. WHITE = untouched, GRAY = on the CURRENT
  recursion path, BLACK = finished. An edge to a GRAY node goes
  BACKWARD in time = cycle. Black edges are fine (cross edges).
  One color bit changes everything - that is the entire difference
  between directed and undirected cycle detection.

SUPER SIMPLE EXAMPLE:
Undirected: 0-1, 1-2, 2-0. DFS 0->1->2, then 2 sees 0 (visited,
NOT parent 1) -> cycle. Directed 0->1, 1->2, 2->0: 2->0 meets
GRAY 0 -> cycle. Same edges, different reason, same answer.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Two parallel paths to the same vertex in a DAG: undirected test
  would cry "cycle" (wrong!) - the directed test says NO because
  the second arrival sees BLACK, not GRAY. This is exactly why
  topo_sort.c can order DAGs that this undirected test rejects.
- Self-loop: directed self-loop = immediate cycle (edge to itself,
  gray). Undirected self-loop = cycle too.
- Disconnected graph: run the DFS from every unvisited vertex.
- Matrix vs list: matrix = O(1) edge lookup, O(V^2) memory (dense
  graphs); list = O(degree) lookup, O(V+E) memory (sparse graphs).
  The demo prints both for the same graph.

DRY RUN (3-color, DAG 0->1, 0->2, 1->3, 2->3):
DFS 0: gray. ->1 gray. ->3 gray. 3 has no out-edges -> black.
   back at 1 -> black. ->2 gray ->3 BLACK (no cycle!) -> black.
No gray edge ever taken -> DAG confirmed. Topological order
exists (0 1 2 3 or 0 2 1 3).

TIPS:
- The parent-skip rule (undirected) vs 3-color (directed) is a
  top interview discriminator - know BOTH.
- dfs.c already showed undirected cycle via hasCycle; this file
  adds the directed version that topo_sort and Kosaraju build on.
- Representations: choose matrix for dense/graph algorithms that
  probe edges constantly (Floyd-Warshall), list for sparse BFS/
  Dijkstra - the demo makes the memory trade-off visible.
*/

#include <stdio.h>
#include <string.h>

#define MAX_V 30

int adjMatrix[MAX_V][MAX_V];
int adjCount[MAX_V];
int adjList[MAX_V][MAX_V];
int vCount;

void addEdge(int u, int v, int directed) {
    adjMatrix[u][v] = 1;
    adjList[u][adjCount[u]] = v;
    adjCount[u] = adjCount[u] + 1;
    if (!directed) {
        adjMatrix[v][u] = 1;
        adjList[v][adjCount[v]] = u;
        adjCount[v] = adjCount[v] + 1;
    }
}

/* ---------- Undirected cycle via parent tracking ---------- */
int visitedU[MAX_V];

int undirectedCycleUtil(int u, int parent) {
    visitedU[u] = 1;
    for (int i = 0; i < adjCount[u]; i++) {
        int v = adjList[u][i];
        if (!visitedU[v]) {
            if (undirectedCycleUtil(v, u)) {
                return 1;
            }
        } else if (v != parent) {
            printf("  back edge %d -> %d (not parent)\n", u, v);
            return 1;
        }
    }
    return 0;
}

/* ---------- Directed cycle via 3 colors ---------- */
int color[MAX_V];

int directedCycleUtil(int u) {
    color[u] = 1;
    for (int i = 0; i < adjCount[u]; i++) {
        int v = adjList[u][i];
        if (color[v] == 1) {
            printf("  gray edge %d -> %d = cycle!\n", u, v);
            return 1;
        }
        if (color[v] == 0 && directedCycleUtil(v)) {
            return 1;
        }
    }
    color[u] = 2;
    return 0;
}

int main() {
    int e;
    int u;
    int v;
    int directed;
    int choice;

    printf("Graph Cycle Detection & Representations (easy -> hard)\n\n");

    printf("How many vertices (1-%d)? ", MAX_V);
    scanf("%d", &vCount);

    if (vCount < 1 || vCount > MAX_V) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Graph type: 1 = directed, 0 = undirected: ");
    scanf("%d", &directed);

    printf("How many edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'u v':\n", e);
    memset(adjCount, 0, sizeof(adjCount));
    memset(adjMatrix, 0, sizeof(adjMatrix));
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        if (u < 0 || u >= vCount || v < 0 || v >= vCount) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        addEdge(u, v, directed);
    }

    while (1) {
        printf("\n1. Show adjacency MATRIX\n");
        printf("2. Show adjacency LIST\n");
        printf("3. Cycle detection\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("    ");
            for (int j = 0; j < vCount; j++) {
                printf("%3d", j);
            }
            printf("\n");
            for (int i = 0; i < vCount; i++) {
                printf("%3d ", i);
                for (int j = 0; j < vCount; j++) {
                    printf("%3d", adjMatrix[i][j]);
                }
                printf("\n");
            }
            printf("Memory: %d cells (V^2 = %d)\n",
                   vCount * vCount, vCount * vCount);
        } else if (choice == 2) {
            printf("Adjacency list:\n");
            int total = 0;
            for (int i = 0; i < vCount; i++) {
                printf("  %d ->", i);
                for (int j = 0; j < adjCount[i]; j++) {
                    printf(" %d", adjList[i][j]);
                    total = total + 1;
                }
                printf("\n");
            }
            printf("Memory: %d entries (V + E style)\n", total);
        } else if (choice == 3) {
            if (directed) {
                memset(color, 0, sizeof(color));
                int found = 0;
                for (int i = 0; i < vCount; i++) {
                    if (color[i] == 0 && directedCycleUtil(i)) {
                        found = 1;
                        break;
                    }
                }
                printf("Directed graph %s a cycle\n",
                       found ? "HAS" : "has no (it is a DAG)");
            } else {
                memset(visitedU, 0, sizeof(visitedU));
                int found = 0;
                for (int i = 0; i < vCount; i++) {
                    if (!visitedU[i] && undirectedCycleUtil(i, -1)) {
                        found = 1;
                        break;
                    }
                }
                printf("Undirected graph %s a cycle\n",
                       found ? "HAS" : "has no");
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
