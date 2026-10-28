/*
A* Search - Heuristic Shortest Paths on a Grid (Advanced)
--------------------------------------------------------
PROBLEM DEFINITION:
Find a shortest path from S to G on a grid with walls ('#'), moving
up/down/left/right at cost 1. Dijkstra explores outward in every
direction. A* orders the frontier by f = g + h, where g is the
distance travelled so far and h is an estimate of the distance still
to go - here the Manhattan distance to G. Because h never
overestimates, A* still returns an optimal path, but it expands far
fewer cells. The program runs both and compares the work done.

REAL-LIFE EXAMPLE:
Game characters walking around obstacles, robot vacuum route planning,
and map apps all use A*: "head roughly towards the goal, but go around
what is in the way."

DIAGRAM (f = g + h on a tiny open grid, S at the left, G at the right):
  S  .  .  G       h = 3 2 1 0       Dijkstra grows a diamond around S;
  .  .  .  .       h = 4 3 2 1       A* grows a thin band towards G,
                                     because cells pointing away from
                                     G have larger f and wait in line.

PSEUDO CODE:
  g[S] = 0; push S with f = h(S)
  while the heap is not empty:
      pop the cell u with the smallest (f, h)   (ties: smaller h first)
      if u is closed: continue;  close u;  expanded++
      if u == G: stop
      for each open neighbour v:
          if g[u] + 1 < g[v]:
              g[v] = g[u] + 1;  parent[v] = u
              push v with f = g[v] + h(v)
  Dijkstra is the same loop with h = 0.

SAMPLE INPUT & DRY RUN:
  5 7
  S...#..
  .##.#..
  ...#...
  .#...#.
  ...#..G
  A* first follows row 0 towards G (those cells tie on the smallest f
  and have smaller h) and runs into the wall at column 4. The path
  that works goes down the left side, through row 3, then along the
  bottom:
      S...#..
      *##.#..
      ***#...
      .#***#.
      ...#**G
  Shortest path length: 10 for both (= Manhattan distance, so optimal)
  Cells expanded: A* 15, Dijkstra 23 of the 27 open cells.

SPECIAL CASES:
  - No path: both searches exhaust the reachable region and say so.
  - S next to G: path length 1.
  - Ties in f are common on grids; breaking them by smaller h makes A*
    dive towards G instead of filling a whole "equal-f" band.
  - The Manhattan heuristic is admissible (never too large) and
    consistent here, so every cell is closed at most once.
  - Grid 1..50 x 1..50 of '.', '#', exactly one 'S' and one 'G'.

COMPLEXITY:
  O(V log V) with a binary heap, V = rows * cols; same worst case as
  Dijkstra, but usually a small fraction of V is expanded.

TIPS:
  h = 0 gives Dijkstra; an h that overestimates gives faster but
  possibly non-optimal paths ("weighted A*"). With diagonal moves use
  the Chebyshev or octile distance instead. Compare bfs (unit weights,
  no goal direction) and dijkstra.
*/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_CELLS (MAX_SIZE * MAX_SIZE)
#define INF 1000000000

typedef struct {
    int f;
    int h;
    int order;
    int cell;
} Entry;

char grid[MAX_SIZE][MAX_SIZE + 2];
int rows;
int cols;
int startCell;
int goalCell;
int g[MAX_CELLS];
int parent[MAX_CELLS];
unsigned char closed[MAX_CELLS];
Entry heap[4 * MAX_CELLS + 1];
int heapSize;
int pushCount;

int before(Entry a, Entry b) {
    if (a.f != b.f) return a.f < b.f;
    if (a.h != b.h) return a.h < b.h;
    return a.order < b.order;
}

void push(int f, int h, int cell) {
    Entry e = {f, h, pushCount++, cell};
    int i = heapSize++;
    while (i > 0 && before(e, heap[(i - 1) / 2])) {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i] = e;
}

Entry pop(void) {
    Entry top = heap[0];
    Entry last = heap[--heapSize];
    int i = 0;
    for (;;) {
        int child = 2 * i + 1;
        if (child >= heapSize) break;
        if (child + 1 < heapSize && before(heap[child + 1], heap[child])) child++;
        if (!before(heap[child], last)) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return top;
}

int manhattan(int cell) {
    int dr = cell / cols - goalCell / cols;
    int dc = cell % cols - goalCell % cols;
    return (dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc);
}

/* Returns the path length (or -1); *expanded counts closed cells. */
int search(int useHeuristic, int *expanded) {
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    for (int i = 0; i < rows * cols; i++) {
        g[i] = INF;
        parent[i] = -1;
        closed[i] = 0;
    }
    heapSize = 0;
    pushCount = 0;
    *expanded = 0;
    g[startCell] = 0;
    int h0 = useHeuristic ? manhattan(startCell) : 0;
    push(h0, h0, startCell);
    while (heapSize > 0) {
        Entry e = pop();
        int u = e.cell;
        if (closed[u]) continue;
        closed[u] = 1;
        (*expanded)++;
        if (u == goalCell) return g[u];
        int r = u / cols;
        int c = u % cols;
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == '#') continue;
            int v = nr * cols + nc;
            if (g[u] + 1 < g[v]) {
                g[v] = g[u] + 1;
                parent[v] = u;
                int h = useHeuristic ? manhattan(v) : 0;
                push(g[v] + h, h, v);
            }
        }
    }
    return -1;
}

int main(void) {
    int starts = 0;
    int goals = 0;
    int openCells = 0;

    printf("A* Search vs Dijkstra on a Grid\n\n");
    printf("Rows and columns (1-%d each): ", MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2 || rows < 1 || rows > MAX_SIZE ||
        cols < 1 || cols > MAX_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    printf("Enter %d rows of '.', '#', 'S', 'G':\n", rows);
    for (int r = 0; r < rows; r++) {
        if (scanf("%51s", grid[r]) != 1 || (int)strlen(grid[r]) != cols) {
            printf("Invalid row\n");
            return 1;
        }
        for (int c = 0; c < cols; c++) {
            char ch = grid[r][c];
            if (ch == 'S') {
                startCell = r * cols + c;
                starts++;
            } else if (ch == 'G') {
                goalCell = r * cols + c;
                goals++;
            } else if (ch != '.' && ch != '#') {
                printf("Invalid cell '%c'\n", ch);
                return 1;
            }
            if (ch != '#') openCells++;
        }
    }
    if (starts != 1 || goals != 1) {
        printf("Need exactly one S and one G\n");
        return 1;
    }

    int dijkstraExpanded;
    int aStarExpanded;
    int dijkstraLength = search(0, &dijkstraExpanded);
    int aStarLength = search(1, &aStarExpanded);
    if (aStarLength < 0) {
        printf("\nNo path from S to G (Dijkstra expanded %d cells, "
               "A* %d)\n", dijkstraExpanded, aStarExpanded);
        return 0;
    }

    for (int v = parent[goalCell]; v != startCell; v = parent[v]) {
        grid[v / cols][v % cols] = '*';
    }
    printf("\nA* path (marked *):\n");
    for (int r = 0; r < rows; r++) printf("  %s\n", grid[r]);
    printf("\nShortest path length: A* %d, Dijkstra %d\n", aStarLength,
           dijkstraLength);
    printf("Cells expanded:       A* %d, Dijkstra %d (of %d open cells)\n",
           aStarExpanded, dijkstraExpanded, openCells);
    return 0;
}
