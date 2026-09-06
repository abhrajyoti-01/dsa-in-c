/*
Euler Path / Euler Circuit (Hierholzer's Algorithm)
---------------------------------------------------
What this program solves:
Finds a trail that uses EVERY EDGE exactly once (Euler path), or
a closed one (Euler circuit), via Hierholzer's algorithm.

Real life example:
The historic Konigsberg bridge problem: can you cross all seven
bridges exactly once? Roadsweeping, plowing, and DNA fragment
assembly all need "visit every edge once" routes.

DIAGRAM (Euler path exists when odd-degree vertices are 0 or 2):
  degrees: A=3 B=3 C=2 D=2   -> exactly 2 odd -> path A...A
  start at an ODD vertex, use every edge exactly once.

How the process works:
1. Existence: every vertex must have even degree (circuit), or
   exactly 0 or 2 vertices with odd degree (path starting at an
   odd vertex).
2. Hierholzer: start at the valid start vertex, greedily walk
   unused edges pushing vertices on a stack; when stuck, pop -
   each pop appends to the final tour. Untraversed side trips
   get spliced into the main trail.
3. Result is the Euler tour in O(V + E).

Pseudo code:
START
CHECK degrees (0 or 2 odd for path; all even for circuit)
start = odd vertex if any, else 0
STACK = [start]
WHILE stack:
    u = top
    IF unused edge u->v: push v, remove edge
    ELSE: pop u, append u to tour
REVERSE tour
END

Sample input and dry run:
4 vertices, edges 0-1 1-2 2-0 0-3 3-4 4-0
degrees: 0:4, 1:2, 2:2, 3:2, 4:2 -> circuit exists
tour: 0 3 4 0 2 1 0 (uses every edge once)

Main logic to understand:
Every visit enters AND leaves a vertex - hence even degree for
circuits. Hierholzer splices "detours" into the main route, so no
edge is used twice and none is missed. O(V + E).
*/

#include <stdio.h>
#include <string.h>

#define MAX_V 20

int adj[MAX_V][MAX_V];
int degree[MAX_V];

/*
 * findStart - Odd-degree vertex for a path, else 0.
 */
int findStart(int n) {
    for (int i = 0; i < n; i++) {
        if (degree[i] % 2 == 1) {
            return i;
        }
    }
    for (int i = 0; i < n; i++) {
        if (degree[i] > 0) {
            return i;
        }
    }
    return 0;
}

int main() {
    int n;
    int e;
    int u;
    int v;

    printf("Euler Path / Circuit (Hierholzer)\n\n");

    printf("How many vertices (1-%d)? ", MAX_V);
    scanf("%d", &n);

    if (n < 1 || n > MAX_V) {
        printf("Invalid count\n");
        return 1;
    }

    printf("How many undirected edges? ");
    scanf("%d", &e);

    printf("Enter %d edges as 'u v', vertices 0..%d:\n", e, n - 1);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        if (u < 0 || u >= n || v < 0 || v >= n) {
            printf("Invalid edge - skipped\n");
            continue;
        }
        if (adj[u][v]) {
            continue;
        }
        adj[u][v] = 1;
        adj[v][u] = 1;
        degree[u] = degree[u] + 1;
        degree[v] = degree[v] + 1;
    }

    /* STEP: Euler theorem check on degrees */
    int oddCount = 0;
    for (int i = 0; i < n; i++) {
        if (degree[i] % 2 == 1) {
            oddCount = oddCount + 1;
        }
    }

    if (oddCount != 0 && oddCount != 2) {
        printf("\n%d vertices have odd degree -> no Euler path/circuit exists\n",
               oddCount);
        return 0;
    }

    if (oddCount == 0) {
        printf("\nAll even degrees -> Euler CIRCUIT exists\n");
    } else {
        printf("\nExactly 2 odd degrees -> Euler PATH exists (start at odd vertex)\n");
    }

    /* STEP: Hierholzer walk */
    int stackArr[MAX_V * MAX_V];
    int tour[MAX_V * MAX_V];
    int top = 0;
    int tourLen = 0;

    int start = findStart(n);
    stackArr[top] = start;
    top = top + 1;

    while (top > 0) {
        int curr = stackArr[top - 1];
        int next = -1;
        for (int i = 0; i < n; i++) {
            if (adj[curr][i]) {
                next = i;
                break;
            }
        }
        if (next != -1) {
            adj[curr][next] = 0;
            adj[next][curr] = 0;
            stackArr[top] = next;
            top = top + 1;
        } else {
            top = top - 1;
            tour[tourLen] = curr;
            tourLen = tourLen + 1;
        }
    }

    printf("Euler tour (reverse order of discovery):\n  ");
    for (int i = tourLen - 1; i >= 0; i--) {
        printf("%d ", tour[i]);
    }
    printf("\n");

    return 0;
}
