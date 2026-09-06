/*
Bipartite Check (2-Coloring with BFS)
-------------------------------------
What this program solves:
Determines whether a graph is BIPARTITE - its vertices can be
split into two groups with no edge inside a group - by trying to
2-color it with BFS.

Real life example:
Scheduling: tasks (vertices) that conflict (edges) must go into
different time slots. If the graph is bipartite, 2 slots suffice;
else it needs more. Also: matching problems, team division,
Venn diagram of enemies (two teams, no enemies inside a team).

How the process works:
1. color[] starts as -1 (uncolored).
2. BFS from every uncolored vertex: color it 0, neighbours 1,
   alternating by BFS level.
3. If a neighbour ever has the SAME color -> odd cycle exists ->
   NOT bipartite.

Pseudo code:
START
FOR each component: BFS(source)
    color[source] = 0
    WHILE queue not empty
        u = dequeue
        FOR v in adj[u]
            IF uncolored: color[v] = 1 - color[u]; enqueue
            ELSE IF color[v] == color[u]: NOT bipartite
END

Sample input and dry run:
Graph: 0-1, 1-2, 2-3, 3-0 -> even cycle -> bipartite
teams: {0, 2} and {1, 3}
Adding edge 0-2 makes an odd cycle -> not bipartite.

Main logic to understand:
A graph is bipartite IFF it has no odd-length cycle. BFS
level-parity IS the 2-coloring. O(V + E) time; works component by
component for disconnected graphs.
*/

#include <stdio.h>

#define MAX_V 100

int adj[MAX_V][MAX_V];
int color[MAX_V];
int queueArr[MAX_V];
int vCount;

/*
 * bfsCheck - 2-colors one component via BFS; 1 = conflict found.
 */
int bfsCheck(int source) {
    int front = 0;
    int rear = 0;

    color[source] = 0;
    queueArr[rear] = source;
    rear = rear + 1;

    while (front < rear) {
        int u = queueArr[front];
        front = front + 1;

        for (int v = 0; v < vCount; v++) {
            if (adj[u][v]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    queueArr[rear] = v;
                    rear = rear + 1;
                } else if (color[v] == color[u]) {
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

    printf("Bipartite Check (2-Coloring)\n\n");

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

    for (int i = 0; i < vCount; i++) {
        color[i] = -1;
    }

    int conflict = 0;
    for (int i = 0; i < vCount; i++) {
        if (color[i] == -1) {
            if (bfsCheck(i)) {
                conflict = 1;
                break;
            }
        }
    }

    if (conflict) {
        printf("\nNOT bipartite (odd cycle exists)\n");
    } else {
        printf("\nBIPARTITE!\n");
        printf("Group 0: ");
        for (int i = 0; i < vCount; i++) {
            if (color[i] == 0) {
                printf("%d ", i);
            }
        }
        printf("\nGroup 1: ");
        for (int i = 0; i < vCount; i++) {
            if (color[i] == 1) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    return 0;
}