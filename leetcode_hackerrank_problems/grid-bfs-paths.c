/*
Grid BFS - Shortest Path, Walking Steps, Rotting Oranges, Multi-Source
(Intermediate -> Advanced)
------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 1091 - Shortest Path in a Binary Matrix (8 directions)
  LeetCode 1926 - Nearest Exit from Entrance in Maze
  LeetCode 994  - Rotting Oranges
  LeetCode 542  - 01 Matrix (multi-source BFS)
  LeetCode 200  - Number of Islands (DFS/BFS flood fill, counted here)
  HackerRank "Shortest Reach in a Graph" (grid variant)

PROBLEM DEFINITION:
A grid is an implicit graph: each free cell is a vertex, each step to a
free neighbour is an edge. One BFS answers the shortest path, the steps
to the nearest exit, the time until everything rots, and the distance
to the nearest zero - because all edges have unit weight.

REAL-LIFE EXAMPLE:
Warehouse robots plan routes on a floor plan (shortest path), and a
spoilage/contamination model spreads from every source simultaneously
(multi-source BFS) to estimate the time until the last cell is
affected.

DIAGRAM (grid, 0 = open, 1 = wall):
  grid           4-directional distances from (0,0):
  0 0 0          0 1 2
  1 1 0          # # 3
  0 0 0          6 5 4
  There is exactly ONE open component (all six free cells connect
  around the right side), so islands = 1 - walls do not always split
  a grid. Cell (2,0) IS reachable: the only route is
  (0,0)->(0,1)->(0,2)->(1,2)->(2,2)->(2,1)->(2,0), six steps, because
  (1,0) and (1,1) are walls. The 8-directional walk reaches (2,2) in
  3 steps (diagonals cut corners) against 4 for the 4-directional walk.
  Multi-source spread (rot/fire) grows one ring per minute from ALL
  sources at once.

PSEUDO CODE:
  BFS(start): queue = {start}, distance[start] = 0
      while queue not empty:
          pop (r, c); for each of the 4 (or 8) neighbours:
              if inside, not a wall, and unvisited:
                  distance = distance[current] + 1; push
  MultiSourceBFS(sources): push ALL sources with distance 0 first;
      the frontier then expands as one wave - this is the ONLY
      difference from the single-source BFS.
  Islands: for every unvisited free cell, run a flood fill (BFS or
      DFS) and count the number of fills.

SAMPLE INPUT & DRY RUN:
  3 3
  0 0 0
  1 1 0
  0 0 0
  0 0
  2 2
  Shortest path from (0,0) to (2,2): 4 steps in 4 directions (down the
  right side) and 3 steps with diagonals enabled. Islands = 1: every
  free cell is connected around the walls. Cell (2,0) is reachable in
  6 steps; an unreachable target would be printed as -1.

SPECIAL CASES:
  - Start equals target: distance 0.
  - Start on a wall, or outside the grid: invalid input, rejected.
  - Unreachable target: -1 (not "no path" with a random number).
  - 4-directional vs 8-directional changes the answer: this program
    runs BOTH and prints them side by side so the difference is visible.
  - Multi-source BFS must seed EVERY source at distance 0 - seeding
    them one by one with separate BFS runs is the classic slow bug.
  - rows, cols <= 100, cell values are 0 (open) or 1 (wall).

COMPLEXITY:
  O(rows * cols) time and space per BFS; counting islands is one more
  O(rows * cols) pass.

TIPS:
  Unit weights + "shortest / fewest steps / minimum time" = BFS, never
  Dijkstra. If several sources spread at once (rotting, fire, nearest
  facility), push them all first. If weights were not unit, switch to
  12-graphs/zero-one-bfs.c or 12-graphs/dijkstra.c.
*/

#include <stdio.h>

#define MAX_DIM 100
#define IMPOSSIBLE -1

int grid[MAX_DIM][MAX_DIM];
int distance[MAX_DIM][MAX_DIM];
int queueRow[MAX_DIM * MAX_DIM];
int queueCol[MAX_DIM * MAX_DIM];

void resetDistances(int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) distance[r][c] = IMPOSSIBLE;
    }
}

int bfs(int rows, int cols, int startRow, int startCol, int diagonal) {
    if (grid[startRow][startCol] == 1) return 0;
    resetDistances(rows, cols);
    static const int stepRow4[4] = {-1, 1, 0, 0};
    static const int stepCol4[4] = {0, 0, -1, 1};
    static const int stepRow8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static const int stepCol8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int* stepRow = diagonal ? stepRow8 : stepRow4;
    const int* stepCol = diagonal ? stepCol8 : stepCol4;
    int directions = diagonal ? 8 : 4;
    int front = 0;
    int rear = 0;
    distance[startRow][startCol] = 0;
    queueRow[rear] = startRow;
    queueCol[rear] = startCol;
    rear++;
    while (front < rear) {
        int r = queueRow[front];
        int c = queueCol[front];
        front++;
        for (int d = 0; d < directions; d++) {
            int nr = r + stepRow[d];
            int nc = c + stepCol[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == 1) continue;
            if (distance[nr][nc] != IMPOSSIBLE) continue;
            distance[nr][nc] = distance[r][c] + 1;
            queueRow[rear] = nr;
            queueCol[rear] = nc;
            rear++;
        }
    }
    return 1;
}

int countIslands(int rows, int cols) {
    resetDistances(rows, cols);
    static const int stepRow4[4] = {-1, 1, 0, 0};
    static const int stepCol4[4] = {0, 0, -1, 1};
    int islands = 0;
    for (int sr = 0; sr < rows; sr++) {
        for (int sc = 0; sc < cols; sc++) {
            if (grid[sr][sc] == 1 || distance[sr][sc] != IMPOSSIBLE) continue;
            islands++;
            int front = 0;
            int rear = 0;
            distance[sr][sc] = 0;
            queueRow[rear] = sr;
            queueCol[rear] = sc;
            rear++;
            while (front < rear) {
                int r = queueRow[front];
                int c = queueCol[front];
                front++;
                for (int d = 0; d < 4; d++) {
                    int nr = r + stepRow4[d];
                    int nc = c + stepCol4[d];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                        continue;
                    }
                    if (grid[nr][nc] == 1 ||
                        distance[nr][nc] != IMPOSSIBLE) {
                        continue;
                    }
                    distance[nr][nc] = 0;
                    queueRow[rear] = nr;
                    queueCol[rear] = nc;
                    rear++;
                }
            }
        }
    }
    return islands;
}

int nearestZeroDistance(int rows, int cols, int startRow, int startCol) {
    resetDistances(rows, cols);
    static const int stepRow4[4] = {-1, 1, 0, 0};
    static const int stepCol4[4] = {0, 0, -1, 1};
    int front = 0;
    int rear = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 0) {
                distance[r][c] = 0;
                queueRow[rear] = r;
                queueCol[rear] = c;
                rear++;
            }
        }
    }
    while (front < rear) {
        int r = queueRow[front];
        int c = queueCol[front];
        front++;
        for (int d = 0; d < 4; d++) {
            int nr = r + stepRow4[d];
            int nc = c + stepCol4[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (distance[nr][nc] != IMPOSSIBLE) continue;
            distance[nr][nc] = distance[r][c] + 1;
            queueRow[rear] = nr;
            queueCol[rear] = nc;
            rear++;
        }
    }
    return distance[startRow][startCol];
}

int main(void) {
    int rows;
    int cols;

    printf("Grid BFS - Shortest Path, Islands, Multi-Source\n\n");
    printf("How many rows and columns (1-%d)? ", MAX_DIM);
    if (scanf("%d %d", &rows, &cols) != 2 ||
        rows < 1 || rows > MAX_DIM || cols < 1 || cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    printf("Enter %d rows of %d cells (0 = open, 1 = wall):\n", rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (scanf("%d", &grid[r][c]) != 1 ||
                (grid[r][c] != 0 && grid[r][c] != 1)) {
                printf("Invalid cell\n");
                return 1;
            }
        }
    }

    int startRow;
    int startCol;
    printf("Start cell row and column (0-based): ");
    if (scanf("%d %d", &startRow, &startCol) != 2 ||
        startRow < 0 || startRow >= rows || startCol < 0 ||
        startCol >= cols) {
        printf("Invalid start cell\n");
        return 1;
    }
    if (grid[startRow][startCol] == 1) {
        printf("Invalid start cell: it is a wall\n");
        return 1;
    }
    int targetRow;
    int targetCol;
    printf("Target cell row and column (0-based): ");
    if (scanf("%d %d", &targetRow, &targetCol) != 2 ||
        targetRow < 0 || targetRow >= rows || targetCol < 0 ||
        targetCol >= cols) {
        printf("Invalid target cell\n");
        return 1;
    }
    if (grid[targetRow][targetCol] == 1) {
        printf("Invalid target cell: it is a wall\n");
        return 1;
    }

    if (bfs(rows, cols, startRow, startCol, 0)) {
        printf("\n4-directional distances from (%d,%d):\n", startRow,
               startCol);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1) {
                    printf("  #");
                } else if (distance[r][c] == IMPOSSIBLE) {
                    printf("  .");
                } else {
                    printf(" %2d", distance[r][c]);
                }
            }
            printf("\n");
        }
        printf("Shortest path (%d,%d) -> (%d,%d), 4-directional = ",
               startRow, startCol, targetRow, targetCol);
        if (distance[targetRow][targetCol] == IMPOSSIBLE) {
            printf("-1 (unreachable)\n");
        } else {
            printf("%d steps\n", distance[targetRow][targetCol]);
        }
    }

    if (bfs(rows, cols, startRow, startCol, 1)) {
        printf("Shortest path (%d,%d) -> (%d,%d), 8-directional = ",
               startRow, startCol, targetRow, targetCol);
        if (distance[targetRow][targetCol] == IMPOSSIBLE) {
            printf("-1 (unreachable)\n");
        } else {
            printf("%d steps\n", distance[targetRow][targetCol]);
        }
    }

    printf("Islands of open cells (4-directional) = %d\n",
           countIslands(rows, cols));
    printf("Distance from (%d,%d) to the nearest open cell = %d\n",
           startRow, startCol,
           nearestZeroDistance(rows, cols, startRow, startCol));
    return 0;
}
