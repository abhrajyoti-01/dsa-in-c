/*
Dijkstra's Shortest Path Algorithm
----------------------------------
What this program solves:
This program finds the shortest path from one source vertex to all other vertices
in a graph when edge weights are non-negative.

Real life example:
Imagine Google Maps trying to find the shortest route from your home to all other
important places in town. It first checks the directly connected roads. Then it
selects the currently nearest unfinished location and uses that information to
improve the routes to other places.

DIAGRAM (source = 1, edges 1->2=2, 1->3=5, 2->3=1):
        2        5
   (1)------->(2)------\
    |                   v
    +----------------->(3)
             5

Step-by-step relaxation:
dist: 1:0 | 2:2 | 3:5          (start)
pick 2 (smallest unvisited) -> edge 2->3 (1):
dist: 1:0 | 2:2 | 3:2+1=3      (5 was beaten by 3!)
pick 3 -> done. Shortest 1->3 = 3 via 1->2->3.

How the process works:
1. Start with the source vertex.
2. Store direct distances from the source.
3. Choose the unvisited vertex with the smallest current distance.
4. Mark it as finalized.
5. Use it to update the distances of neighboring vertices.
6. Repeat until all reachable vertices are processed.

Pseudo code:
START
Read graph and source vertex
Initialize distance array using source row
Mark all vertices unvisited
Mark source visited and set distance[source] = 0
Repeat for remaining vertices
    Select unvisited vertex with minimum distance
    Mark it visited
    For each neighbor
        If shorter path found through selected vertex
            Update its distance
Print all shortest distances from source
END

Sample input and dry run:
Suppose source = 1 and edges are 1->2 = 2, 1->3 = 5, 2->3 = 1.
Initially dist[1] = 0, dist[2] = 2, dist[3] = 5.
Vertex 2 is selected first because it has the smallest distance.
Using vertex 2, dist[3] becomes 2 + 1 = 3, which is better than 5.
So the shortest path from 1 to 3 becomes 1 -> 2 -> 3 with total cost 3.

Important note:
This method works correctly only when edge weights are not negative.
*/
#include <stdio.h>

#define MAX 20
#define INF 999999

int main() {
    int cost[MAX][MAX], dist[MAX], visited[MAX] = {0};
    int n, e, i, j, u, v, w, source;
    int min, nextNode;

    printf("Dijkstra's Shortest Path Algorithm\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &e);

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (i == j) {
                cost[i][j] = 0;
            } else {
                cost[i][j] = INF;
            }
        }
    }

    for (i = 0; i < e; i++) {
        printf("Enter starting vertex of edge %d: ", i + 1);
        scanf("%d", &u);
        printf("Enter ending vertex of edge %d: ", i + 1);
        scanf("%d", &v);
        printf("Enter weight of edge %d: ", i + 1);
        scanf("%d", &w);
        cost[u][v] = w;
    }

    printf("Enter the source vertex: ");
    scanf("%d", &source);

    for (i = 1; i <= n; i++) {
        dist[i] = cost[source][i];
    }

    visited[source] = 1;
    dist[source] = 0;

    for (i = 1; i < n; i++) {
        min = INF;
        nextNode = -1;

        for (j = 1; j <= n; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                nextNode = j;
            }
        }

        if (nextNode == -1) {
            break;
        }

        visited[nextNode] = 1;

        for (j = 1; j <= n; j++) {
            if (!visited[j] && dist[nextNode] + cost[nextNode][j] < dist[j]) {
                dist[j] = dist[nextNode] + cost[nextNode][j];
            }
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

