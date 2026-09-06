/*
Strongly Connected Components - Kosaraju's Algorithm
----------------------------------------------------
What this program solves:
Finds all strongly connected components (SCCs) of a directed
graph using two DFS passes: first compute finish-time order, then
DFS the REVERSED graph in decreasing finish time.

Real life example:
Web page clustering: pages linking within a closed loop form a
community. Finding SCCs groups mutually reachable pages, detects
cyclic dependencies in build systems, and ranks web importance.

DIAGRAM (two passes: DFS finish order, then DFS on reversed graph):
  pass 1: record finish times on G
  pass 2: process vertices by DECREASING finish time on G^T
  each pass-2 tree = one SCC (nodes that can all reach each other)

How the process works:
1. DFS on G, pushing each finished vertex onto a stack
   (finish time order).
2. Reverse every edge (transpose graph).
3. Pop vertices from the stack; each unvisited DFS tree in the
   transpose is exactly one SCC.

Pseudo code:
START
FILL_ORDER: FOR each v: IF unvisited: DFS(v); push v on finish
REVERSE all edges
POP stack: FOR each popped unvisited v: DFS(v) -> new SCC
END

Sample input and dry run:
5 vertices, edges 1->0 0->2 2->1 0->3 3->4
SCC 1: {0, 1, 2}; SCC 2: {3}; SCC 3: {4}

Main logic to understand:
Two DFS passes. The transpose "locks" SCCs: a DFS from the
last-finished vertex can reach exactly its own SCC, because all
edges LEAVING the SCC now point backwards into earlier territory.
O(V + E) time and space.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_V 100

int adj[MAX_V][MAX_V];
int radj[MAX_V][MAX_V];
int visited[MAX_V];
int stackArr[MAX_V];
int top = 0;
int vCount;

/*
 * addDirectedEdge - One-way edge u -> v.
 */
void addDirectedEdge(int u, int v) {
    adj[u][v] = 1;
    radj[v][u] = 1;
}

/*
 * dfsFill - First pass: record finish order.
 */
void dfsFill(int u) {
    visited[u] = 1;
    for (int v = 0; v < vCount; v++) {
        if (adj[u][v] && !visited[v]) {
            dfsFill(v);
        }
    }
    stackArr[top] = u;
    top = top + 1;
}

/*
 * dfsCollect - Second pass on the transpose: one SCC.
 */
void dfsCollect(int u, int* comp, int* compSize) {
    visited[u] = 0;
    comp[*compSize] = u;
    *compSize = *compSize + 1;
    for (int v = 0; v < vCount; v++) {
        if (radj[u][v] && visited[v] == 1) {
            visited[v] = 2;
            dfsCollect(v, comp, compSize);
        }
    }
}

int main() {
    int e;
    int u;
    int v;

    printf("Strongly Connected Components (Kosaraju)\n\n");

    printf("How many vertices (1-%d)? ", MAX_V);
    scanf("%d", &vCount);

    if (vCount < 1 || vCount > MAX_V) {
        printf("Invalid count\n");
        return 1;
    }

    printf("How many directed edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'u v' (u -> v), vertices 0..%d:\n", e, vCount - 1);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        if (u < 0 || u >= vCount || v < 0 || v >= vCount) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        addDirectedEdge(u, v);
    }

    for (int i = 0; i < vCount; i++) {
        visited[i] = 0;
    }

    /* STEP 1: finish-time order on the original graph */
    for (int i = 0; i < vCount; i++) {
        if (!visited[i]) {
            dfsFill(i);
        }
    }

    /* STEP 2: DFS on the transpose in reverse finish order */
    printf("\nStrongly connected components:\n");
    int sccCount = 0;
    int comp[MAX_V];

    while (top > 0) {
        top = top - 1;
        int s = stackArr[top];
        if (visited[s] == 1) {
            visited[s] = 2;
            int compSize = 0;
            dfsCollect(s, comp, &compSize);

            sccCount = sccCount + 1;
            printf("SCC %d: { ", sccCount);
            for (int i = 0; i < compSize; i++) {
                printf("%d ", comp[i]);
            }
            printf("}\n");
        }
    }

    printf("Total SCCs: %d\n", sccCount);

    return 0;
}
