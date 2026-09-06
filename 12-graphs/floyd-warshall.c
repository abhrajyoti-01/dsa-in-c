/*
Floyd Warshall Algorithm
------------------------
What this program solves:
This program finds the shortest paths between every pair of vertices in a graph.

Real life example:
Imagine a travel company wants to know the cheapest fare between every pair of
cities, not just from one source city. A direct bus route may be costly, but
going through another city may reduce the total fare. Floyd Warshall checks all
such intermediate possibilities.

DIAGRAM (try every vertex k as a stopover: i -> k -> j):
  dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
  k = 1, then 2, then 3... after all k the table is all-pairs
  shortest paths. O(V^3), tiny code.

How the process works:
1. Start with the adjacency matrix of direct costs.
2. Choose one vertex k as an intermediate stop.
3. For each pair (i, j), compare:
   direct known cost
   versus cost of going i -> k -> j
4. If going through k is cheaper, update the matrix.
5. Repeat for all vertices as intermediate stops.

Pseudo code:
START
Read the cost adjacency matrix
For each intermediate vertex k
    For each source vertex i
        For each destination vertex j
            If path i -> k -> j is shorter than current path i -> j
                Update dist[i][j]
Print final shortest path matrix
END

Sample input and dry run:
Suppose dist[1][3] = 10, dist[1][2] = 3, and dist[2][3] = 4.
When vertex 2 is chosen as an intermediate point,
the algorithm compares direct cost 10 with 3 + 4 = 7.
Since 7 is smaller, dist[1][3] is updated to 7.
This means the route 1 -> 2 -> 3 is better than the direct edge.

Main logic to understand:
The algorithm improves the answer step by step by allowing more intermediate
vertices in the route.
*/
#include <stdio.h>

#define MAX 20
#define INF 999999

int main() {
    int dist[MAX][MAX];
    int n, i, j, k;

    printf("Floyd Warshall Algorithm\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the cost adjacency matrix:\n");
    printf("Use %d for no direct edge.\n", INF);
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    for (k = 1; k <= n; k++) {
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("\nAll pairs shortest path matrix:\n");
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (dist[i][j] >= INF) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }

    return 0;
}

