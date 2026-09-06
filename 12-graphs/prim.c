/*
Prim's Minimum Spanning Tree
----------------------------
What this program solves:
Suppose electricity must be supplied to a group of villages and we want to lay
the minimum amount of wire. Starting from one village, we keep connecting the
nearest new village until every village becomes part of the network. This program
implements that idea.

Real life example:
Imagine village 1 is already electrified. From village 1, the cheapest new
connection may be village 2. After connecting 2, we now consider roads or wires
coming from both 1 and 2 and choose the cheapest connection to a village not yet
included. The network grows step by step in the cheapest possible way.

DIAGRAM (grow one tree from A, always cheapest frontier edge):
       B(4)          start {A}; cheapest edge A-B(1)
      /              add B; frontier: A-C(4) B-C(2) -> take B-C
  A --1-- B          add C; frontier: C-D(3) -> take
      \              MST weight 1+2+3
       C --2-- D

How the process works:
1. Start from one chosen vertex, here vertex 1.
2. Mark it as visited.
3. Find the smallest edge that connects any visited vertex to any unvisited vertex.
4. Add that edge to the answer.
5. Mark the new vertex as visited.
6. Repeat until all vertices are connected.

Pseudo code:
START
Read graph
Mark starting vertex as visited
While number of selected edges < n - 1
    Find the minimum edge from visited set to unvisited set
    If no such edge exists
        Stop
    Add the edge to MST
    Mark new vertex as visited
Print MST edges and total cost
END

Sample input and dry run:
Suppose edges are (1,2,3), (1,3,5), (2,3,1), (2,4,4)
Start from vertex 1.
Cheapest edge from visited set is (1,2) with cost 3.
Now visited vertices are 1 and 2.
Next cheapest connection to an unvisited vertex is (2,3) with cost 1, then (2,4) with cost 4.
So the tree grows step by step with minimum added cost.

Difference from Kruskal:
Prim always grows one connected tree, while Kruskal selects cheapest safe edges
from the whole graph.
*/
#include <stdio.h>

#define MAX 20
#define INF 999999

int main() {
    int cost[MAX][MAX], visited[MAX] = {0};
    int n, e, i, j, u, v, w;
    int min, minCost = 0, edgeCount = 0;
    int a = -1, b = -1;

    printf("Prim's Minimum Spanning Tree Algorithm\n");
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
        printf("Enter cost of edge %d: ", i + 1);
        scanf("%d", &w);
        cost[u][v] = w;
        cost[v][u] = w;
    }

    visited[1] = 1;

    printf("\nEdges in the Minimum Cost Spanning Tree:\n");

    while (edgeCount < n - 1) {
        min = INF;
        a = -1;
        b = -1;

        for (i = 1; i <= n; i++) {
            if (visited[i]) {
                for (j = 1; j <= n; j++) {
                    if (!visited[j] && cost[i][j] < min) {
                        min = cost[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
        }

        if (a == -1 || b == -1) {
            break;
        }

        printf("Edge (%d, %d) with cost %d\n", a, b, min);
        minCost += min;
        visited[b] = 1;
        edgeCount++;
    }

    if (edgeCount != n - 1) {
        printf("No spanning tree exists.\n");
    } else {
        printf("Total Minimum Cost: %d\n", minCost);
    }

    return 0;
}

