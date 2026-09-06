/*
Hamiltonian Cycle
-----------------
What this program solves:
This program finds a Hamiltonian cycle in a graph, which means a cycle that
visits every vertex exactly once and returns to the starting vertex.

Real life example:
Imagine a tourist wants to visit every important place in a city exactly once
and then return to the hotel. The tourist is not asking for minimum cost here,
only whether such a complete round trip exists.

DIAGRAM (extend a path; abort when a dead end has no unvisited neighbour):
  0 -> 1 -> 3 -> 2 -> 0   (back to start = cycle)
  try each unvisited vertex as the next stop; undo on failure.

How the process works:
1. Start from vertex 1.
2. Try placing another valid vertex in the next position of the path.
3. A vertex is valid only if:
   it is connected to the previous vertex and it is not already in the path.
4. Continue until all vertices are placed.
5. Finally check whether the last vertex connects back to the starting vertex.

Pseudo code:
START
Read number of vertices and adjacency matrix
Initialize path with first vertex fixed
Try to place vertices one by one recursively
    A vertex is safe if it is connected to previous vertex
    and not already used in the path
If all vertices are placed
    Check if last vertex connects to first vertex
    If yes, Hamiltonian cycle found
If no valid choice works
    Backtrack
Print cycle if found, otherwise print no cycle
END

Dry run idea:
If a graph has edges 1-2, 2-3, 3-4, and 4-1,
then one Hamiltonian cycle is 1 -> 2 -> 3 -> 4 -> 1.
*/
#include <stdio.h>

#define MAX 20

int graph[MAX][MAX], path[MAX];
int n;

int isSafe(int vertex, int position) {
    int i;

    if (graph[path[position - 1]][vertex] == 0) {
        return 0;
    }

    for (i = 0; i < position; i++) {
        if (path[i] == vertex) {
            return 0;
        }
    }

    return 1;
}

int hamiltonianCycle(int position) {
    int vertex;

    if (position == n) {
        if (graph[path[position - 1]][path[0]] == 1) {
            return 1;
        }
        return 0;
    }

    for (vertex = 1; vertex <= n; vertex++) {
        if (isSafe(vertex, position)) {
            path[position] = vertex;

            if (hamiltonianCycle(position + 1)) {
                return 1;
            }

            path[position] = -1;
        }
    }

    return 0;
}

int main() {
    int i, j;

    printf("Hamiltonian Cycle Problem\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    for (i = 0; i < n; i++) {
        path[i] = -1;
    }

    path[0] = 0;

    if (hamiltonianCycle(1)) {
        printf("Hamiltonian cycle is:\n");
        for (i = 0; i < n; i++) {
            printf("%d ", path[i] + 1);
        }
        printf("%d\n", path[0] + 1);
    } else {
        printf("No Hamiltonian cycle exists.\n");
    }

    return 0;
}

