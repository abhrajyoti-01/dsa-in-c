/*
Graph Coloring (m-Coloring Backtracking)
----------------------------------------
What this program solves:
Decides whether an undirected graph can be colored with at most m
colors so that no two adjacent vertices share a color, and prints
one valid coloring found by backtracking.

Real life example:
Timetabling: exams are vertices, shared students are edges, colors
are exam slots. m-coloring asks: can the schedule fit in m slots?
Register allocation in compilers is graph coloring on variables.

DIAGRAM (color a 3-node cycle with 2 colors?):
  A-1, B-2, C-? : C touches A(1) and B(2) -> FAIL with 2
  backtrack, retry with 3 colors -> A1 B2 C3 works.

How the process works:
1. Color vertices 0..v-1 one at a time.
2. For vertex v, try colors 1..m; a color is SAFE if no already-
   colored neighbour has it.
3. If no color is safe, backtrack to the previous vertex and try
   its next color.
4. Coloring all vertices = solution found.

Pseudo code:
START
SAFE(v, c): FOR u < v adjacent to v: color[u] != c
SOLVE(v):
    IF v == numVertices: solution found
    FOR c = 1..m
        IF safe(v, c): color[v] = c; recurse; if success return
    RETURN failure (backtrack)
END

Sample input and dry run:
4-cycle 0-1-1-2-2-3-3-0 with m=2
colors [1,2,1,2] - valid!
Complete graph K3 with m=2 -> impossible.

Main logic to understand:
Exponential worst case O(m^V), but pruning by the safety check
cuts hopeless branches early. Chromatic-number questions are NP-
hard in general; this fixed-m decision version is the classic
backtracking exercise.
*/

#include <stdio.h>

#define MAX_V 20

int adj[MAX_V][MAX_V];
int colors[MAX_V];
int vCount;
int m;

/*
 * isSafe - Color c conflicts with no colored neighbour.
 */
int isSafe(int v, int c) {
    for (int u = 0; u < vCount; u++) {
        if (adj[v][u] && colors[u] == c) {
            return 0;
        }
    }
    return 1;
}

/*
 * colorGraph - Backtracking over vertices.
 */
int colorGraph(int v) {
    if (v == vCount) {
        return 1;
    }
    for (int c = 1; c <= m; c++) {
        if (isSafe(v, c)) {
            colors[v] = c;
            if (colorGraph(v + 1)) {
                return 1;
            }
            colors[v] = 0;
        }
    }
    return 0;
}

int main() {
    int e;
    int u;
    int v;

    printf("Graph m-Coloring (Backtracking)\n\n");

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

    printf("Number of colors m: ");
    scanf("%d", &m);

    if (m < 1) {
        printf("Invalid m\n");
        return 1;
    }

    for (int i = 0; i < vCount; i++) {
        colors[i] = 0;
    }

    if (colorGraph(0)) {
        printf("\nColoring EXISTS with %d colors:\n", m);
        for (int i = 0; i < vCount; i++) {
            printf("Vertex %d -> color %d\n", i, colors[i]);
        }
    } else {
        printf("\nNo valid coloring with %d colors\n", m);
    }

    return 0;
}
