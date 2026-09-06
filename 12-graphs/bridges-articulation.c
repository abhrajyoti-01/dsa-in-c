/*
Bridges and Articulation Points (Tarjan's Algorithm)
----------------------------------------------------
What this program solves:
Finds every BRIDGE (edge whose removal disconnects the graph) and
every ARTICULATION POINT (vertex whose removal disconnects it) in
one DFS using discovery times and low-link values.

Real life example:
Network reliability: a bridge is a cable whose cut splits the
network; an articulation point is a router whose failure disconnects
offices. Designers add redundancy precisely at these points.

DIAGRAM (low-link values expose bridges/articulation points):
  1 - 2
  |   |      subtree of 3 reaches no ancestor (low[3] >= disc[2])
  3 - 4      -> edge 2-3 (and nodes 2, 3) are critical
  low[u] = min(disc[u], disc of back-edges from u's subtree)

How the process works:
1. Track disc[u] (DFS discovery time) and low[u] (earliest
   reachable disc through u's subtree + one back edge).
2. Back edge (u, v) with v already on stack: low[u] =
   min(low[u], disc[v]).
3. Tree edge u->v: recurse; then low[u] = min(low[u], low[v]).
   - v is ARTICULATION if low[v] >= disc[u] (and u is not root,
     or u has 2+ DFS children).
   - (u, v) is a BRIDGE if low[v] > disc[u].

Pseudo code:
START
DFS(u, parent):
    disc[u] = low[u] = ++timer
    FOR v in adj[u] (skip parent once)
        IF unvisited: DFS(v); low[u] = min(low[u], low[v])
            IF low[v] > disc[u]: (u,v) is a bridge
            IF low[v] >= disc[u] AND u has parent: u = articulation
        ELSE IF v != parent: low[u] = min(low[u], disc[v])
END

Sample input and dry run:
5 vertices, edges: 1-0 0-2 2-1 0-3 3-4
Bridges: 3-4, 0-3; Articulation points: 0, 3

Main logic to understand:
low[v] > disc[u] means the subtree of v cannot climb back above u
- cut! low[v] >= disc[u] is the weaker condition for vertex cuts
(v can reattach only AT u). O(V + E) time, single DFS.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_V 100

int adj[MAX_V][MAX_V];
int disc[MAX_V];
int low[MAX_V];
int visited[MAX_V];
int isArticulation[MAX_V];
int timer = 0;
int vCount;

/*
 * minInt - Utility minimum.
 */
int minInt(int a, int b) {
    return (a < b) ? a : b;
}

/*
 * dfsAP - Tarjan DFS computing low links, bridges, articulation.
 */
void dfsAP(int u, int parent) {
    visited[u] = 1;
    disc[u] = timer;
    low[u] = timer;
    timer = timer + 1;
    int children = 0;

    for (int v = 0; v < vCount; v++) {
        if (adj[u][v]) {
            if (!visited[v]) {
                children = children + 1;
                dfsAP(v, u);
                low[u] = minInt(low[u], low[v]);

                if (low[v] > disc[u]) {
                    printf("Bridge: %d - %d\n", u, v);
                }
                if (parent != -1 && low[v] >= disc[u]) {
                    isArticulation[u] = 1;
                }
                if (parent == -1 && children > 1) {
                    isArticulation[u] = 1;
                }
            } else if (v != parent) {
                low[u] = minInt(low[u], disc[v]);
            }
        }
    }
}

int main() {
    int e;
    int u;
    int v;

    printf("Bridges and Articulation Points (Tarjan)\n\n");

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

    printf("\nBridges:\n");
    for (int i = 0; i < vCount; i++) {
        if (!visited[i]) {
            dfsAP(i, -1);
        }
    }

    printf("\nArticulation points:\n");
    int count = 0;
    for (int i = 0; i < vCount; i++) {
        if (isArticulation[i]) {
            printf("Vertex %d\n", i);
            count = count + 1;
        }
    }
    if (count == 0) {
        printf("(none)\n");
    }

    return 0;
}
