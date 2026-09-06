/*
Kruskal's Minimum Spanning Tree
-------------------------------
What this program solves:
Suppose a company wants to connect several offices using network cables, or a
government wants to connect towns using roads. The goal is to connect every
place with minimum total cost, without creating unnecessary loops. This program
finds that minimum-cost connection.

Real life example:
Imagine 4 towns and possible roads:
(1,2,1), (2,3,2), (1,3,4), (3,4,3)
If we blindly take all roads, cost becomes high. Kruskal's idea is to sort roads
by cost and keep choosing the cheapest road that does not create a cycle.
So we may take (1,2), then (2,3), then (3,4), and skip the expensive road (1,3).

DIAGRAM (sort edges, take if no cycle - union-find checks):
  edges sorted: AB(1) BC(3) AC(4) CD(2)
  AB ok, CD ok, BC ok (3 < 4), AC would close a cycle -> skip
  MST: AB + CD + BC

How the process works:
1. Read all edges and sort them in increasing order of cost.
2. Treat each vertex as a separate group in the beginning.
3. Pick the cheapest edge.
4. If its endpoints are in different groups, include the edge in the answer.
5. Merge those groups.
6. If both endpoints are already in the same group, skip the edge because it
   would create a cycle.
7. Continue until n-1 edges are selected.

Pseudo code:
START
Read number of vertices and edges
Read all edges with their costs
Sort edges by increasing cost
Place each vertex in a separate set
For each edge in sorted order
    If endpoints belong to different sets
        Include the edge in MST
        Union the two sets
    Stop when n - 1 edges are selected
Print MST edges and total cost
END

Sample input and dry run:
Edges are (1,2,1), (2,3,2), (1,3,4), (3,4,3)
After sorting: cost 1, 2, 3, 4
Pick (1,2), then pick (2,3), then pick (3,4)
Now all 4 vertices are connected using 3 edges.
Edge (1,3) is skipped because it would create a cycle.

Why this greedy logic works:
At every step, the cheapest safe edge helps build the final minimum spanning tree.
*/
#include <stdio.h>

#define MAX 100

struct Edge {
    int u;
    int v;
    int cost;
};

int parent[MAX];

int findParent(int vertex) {
    while (parent[vertex] != vertex) {
        vertex = parent[vertex];
    }
    return vertex;
}

void unionSet(int u, int v) {
    int rootU = findParent(u);
    int rootV = findParent(v);
    parent[rootU] = rootV;
}

int main() {
    struct Edge edges[MAX], temp;
    int n, e, i, j;
    int minCost = 0, count = 0;

    printf("Kruskal's Minimum Spanning Tree Algorithm\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &e);

    for (i = 0; i < e; i++) {
        printf("Enter starting vertex of edge %d: ", i + 1);
        scanf("%d", &edges[i].u);
        printf("Enter ending vertex of edge %d: ", i + 1);
        scanf("%d", &edges[i].v);
        printf("Enter cost of edge %d: ", i + 1);
        scanf("%d", &edges[i].cost);
    }

    for (i = 1; i <= n; i++) {
        parent[i] = i;
    }

    for (i = 0; i < e - 1; i++) {
        for (j = 0; j < e - i - 1; j++) {
            if (edges[j].cost > edges[j + 1].cost) {
                temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    printf("\nEdges in the Minimum Cost Spanning Tree:\n");

    for (i = 0; i < e && count < n - 1; i++) {
        if (findParent(edges[i].u) != findParent(edges[i].v)) {
            unionSet(edges[i].u, edges[i].v);
            printf("Edge (%d, %d) with cost %d\n", edges[i].u, edges[i].v, edges[i].cost);
            minCost += edges[i].cost;
            count++;
        }
    }

    if (count != n - 1) {
        printf("No spanning tree exists.\n");
    } else {
        printf("Total Minimum Cost: %d\n", minCost);
    }

    return 0;
}

