/*
Warshall's Algorithm
--------------------
What this program solves:
This program finds reachability between every pair of vertices. It tells whether
one vertex can reach another vertex, directly or through intermediate vertices.

Real life example:
Imagine a railway network. There may be no direct train from city 1 to city 3,
but if city 1 connects to city 2 and city 2 connects to city 3, then city 1 can
still reach city 3. Warshall's algorithm discovers all such reachable pairs.

DIAGRAM (reachability: is there ANY path i -> j?):
  R[k][i][j] = R[k-1][i][j] OR (R[k-1][i][k] AND R[k-1][k][j])
  "i reaches j if it already does, or via a stopover at k"
  1 = reachable, 0 = not; k sweeps 1..V.

How the process works:
1. Start with the adjacency matrix.
2. Choose one vertex k as a possible intermediate point.
3. For every pair (i, j), check whether i can reach k and k can reach j.
4. If yes, then i can also reach j.
5. Repeat for every vertex.

Pseudo code:
START
Read adjacency matrix
For each intermediate vertex k
    For each pair of vertices i and j
        If i can reach k and k can reach j
            Mark i can reach j
Print transitive closure matrix
END

Sample input and dry run:
Suppose 1 can reach 2 and 2 can reach 3.
Initially path[1][3] may be 0 because there is no direct edge.
When k = 2, the algorithm checks path[1][2] and path[2][3].
Since both are 1, path[1][3] becomes 1.
So the final matrix shows that 1 can reach 3 through vertex 2.

Main logic to understand:
This algorithm answers "Can I reach there?" and not "What is the minimum cost?".
*/
#include <stdio.h>

#define MAX 20

int main() {
    int path[MAX][MAX];
    int n, i, j, k;

    printf("Warshall's Algorithm\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix:\n");
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            scanf("%d", &path[i][j]);
        }
    }

    for (k = 1; k <= n; k++) {
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= n; j++) {
                path[i][j] = path[i][j] || (path[i][k] && path[k][j]);
            }
        }
    }

    printf("Transitive closure matrix:\n");
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            printf("%d ", path[i][j]);
        }
        printf("\n");
    }

    return 0;
}

