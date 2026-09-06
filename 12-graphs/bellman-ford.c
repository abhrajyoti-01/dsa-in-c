/*
Bellman Ford Algorithm
----------------------
What this program solves:
This program finds the shortest distance from one source vertex to all other
vertices, even when some roads or edges have negative weights.

Real life example:
Imagine a financial network where moving from one city to another may give a
discount or cashback, so some route costs can become negative. Dijkstra is not
safe in that situation, but Bellman Ford can still compute correct shortest paths
and can also detect whether a harmful negative cycle exists.

DIAGRAM (relax ALL edges V-1 times):
  edges: A->B(4) A->C(2) C->B(-1)
  round 1: dist B=4, C=2; relax C->B: 2-1=1 < 4 -> B=1
  round 2: no change -> done (a round-3 change means NEGATIVE CYCLE)

How the process works:
1. Set source distance to 0 and all other distances to infinity.
2. Check every edge and relax it.
3. Repeat that process n-1 times because a shortest path can use at most n-1 edges.
4. Finally, check all edges one more time.
5. If a distance still improves, a negative cycle exists.

Pseudo code:
START
Read number of vertices, edges, and source
Initialize all distances as INF
Set distance of source = 0
Repeat n - 1 times
    For each edge (u, v, w)
        If distance[u] is not INF and distance[u] + w < distance[v]
            Update distance[v]
Check all edges once more
    If any distance can still be reduced
        Negative cycle exists
Print all shortest distances
END

Sample input and dry run:
Suppose edges are 1->2 = 4, 1->3 = 5, 2->3 = -2 and source = 1.
Initially dist[1] = 0, dist[2] = INF, dist[3] = INF.
After relaxation, dist[2] becomes 4 and dist[3] becomes 5.
Then edge 2->3 updates dist[3] from 5 to 2.
So the shortest path from 1 to 3 becomes 1 -> 2 -> 3 with total cost 2.

Main logic to understand:
Repeated relaxation slowly spreads the best known distances through the graph.
*/
#include <stdio.h>

#define MAX 50
#define INF 999999

struct Edge {
    int u;
    int v;
    int w;
};

int main() {
    struct Edge edges[MAX];
    int dist[MAX];
    int n, e, i, j, source;

    printf("Bellman Ford Algorithm\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &e);

    for (i = 0; i < e; i++) {
        printf("Enter starting vertex of edge %d: ", i + 1);
        scanf("%d", &edges[i].u);
        printf("Enter ending vertex of edge %d: ", i + 1);
        scanf("%d", &edges[i].v);
        printf("Enter weight of edge %d: ", i + 1);
        scanf("%d", &edges[i].w);
    }

    printf("Enter the source vertex: ");
    scanf("%d", &source);

    for (i = 1; i <= n; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    for (i = 1; i <= n - 1; i++) {
        for (j = 0; j < e; j++) {
            if (dist[edges[j].u] != INF && dist[edges[j].u] + edges[j].w < dist[edges[j].v]) {
                dist[edges[j].v] = dist[edges[j].u] + edges[j].w;
            }
        }
    }

    for (j = 0; j < e; j++) {
        if (dist[edges[j].u] != INF && dist[edges[j].u] + edges[j].w < dist[edges[j].v]) {
            printf("Negative weight cycle exists.\n");
            return 0;
        }
    }

    printf("\nShortest distances from vertex %d:\n", source);
    for (i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            printf("To %d: No path\n", i);
        } else {
            printf("To %d: %d\n", i, dist[i]);
        }
    }

    return 0;
}

