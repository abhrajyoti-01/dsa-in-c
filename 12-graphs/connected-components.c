/*
Connected Components (Undirected Graph)
---------------------------------------
What this program solves:
Counts and lists the connected components of an undirected graph
by launching BFS from every unvisited vertex.

Real life example:
Power grid sections: after storms, some areas stay connected while
others go dark. Each BFS flood from an unvisited node marks one
still-connected island of the grid.

DIAGRAM (3 components in one graph):
  A-B-C     D-E     F      each component visited by one
                           DFS/BFS launch
  components = number of times a fresh search had to start.

How the process works:
1. visited[] all false.
2. For each vertex i not visited: start a BFS, mark everything
   reachable - that BFS tree is component #k.
3. Record which component each vertex belongs to; print groups.

Pseudo code:
START
comp = 0
FOR v = 0..n-1
    IF not visited[v]:
        comp++; BFS/DFS from v marking visited[v] = comp
PRINT vertices grouped by component id
END

Sample input and dry run:
6 vertices, edges: 0-1, 1-2, 3-4
Components: {0,1,2}, {3,4}, {5} -> 3 components

Main logic to understand:
A component is a maximal connected subgraph. One BFS covers exactly
one component, so "BFS until everything is visited" partitions the
graph. O(V + E) total since each vertex/edge is touched once.
Union-Find solves the same problem for edge streams.
*/

#include <stdio.h>

#define MAX_V 100

int adj[MAX_V][MAX_V];
int visited[MAX_V];
int component[MAX_V];
int queueArr[MAX_V];
int vCount;

/*
 * bfsComponent - Floods one component starting at source.
 */
void bfsComponent(int source, int compId) {
    int front = 0;
    int rear = 0;

    visited[source] = 1;
    component[source] = compId;
    queueArr[rear] = source;
    rear = rear + 1;

    while (front < rear) {
        int u = queueArr[front];
        front = front + 1;
        for (int v = 0; v < vCount; v++) {
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                component[v] = compId;
                queueArr[rear] = v;
                rear = rear + 1;
            }
        }
    }
}

int main() {
    int e;
    int u;
    int v;

    printf("Connected Components (BFS)\n\n");

    printf("How many vertices (1-%d)? ", MAX_V);
    scanf("%d", &vCount);

    if (vCount < 1 || vCount > MAX_V) {
        printf("Invalid count\n");
        return 1;
    }

    printf("How many undirected edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'u v', vertices 0..%d:\n", e, vCount - 1);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        if (u < 0 || u >= vCount || v < 0 || v >= vCount) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    /* STEP: BFS from every unvisited vertex */
    int compCount = 0;
    for (int i = 0; i < vCount; i++) {
        if (!visited[i]) {
            compCount = compCount + 1;
            bfsComponent(i, compCount);
        }
    }

    printf("\nNumber of components: %d\n", compCount);
    for (int c = 1; c <= compCount; c++) {
        printf("Component %d: { ", c);
        for (int i = 0; i < vCount; i++) {
            if (component[i] == c) {
                printf("%d ", i);
            }
        }
        printf("}\n");
    }

    return 0;
}
