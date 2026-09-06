/*
Graph Problems Ladder 2 - Bipartite, Islands, Flood Fill (Basic -> Hard)
-------------------------------------------------------------------------
Real life example:
Photo editors flood-fill regions (paint bucket); satellite
software counts islands/forests; maze games are grid graphs.

DIAGRAM (flood fill / island count on a grid):
  1 1 0          4-direction spread from each unvisited land cell
  1 0 0          each spread = one island; here: 2 islands
  0 0 1

LADDER (easy -> hard inside this file):
  1. Flood fill (BFS recolor)         (paint-bucket basics)
  2. Number of islands                (connected components on a grid)
  3. Max area island                  (DFS size counting)
  4. Bipartite check (2-coloring)     (odd cycle detector)

BEGINNER EXPLANATION:
A GRID IS A GRAPH: each cell is a node, adjacent same-value cells
are edges. Islands = connected components of '1' cells (4-directional).
- Flood fill: BFS from a cell recoloring the component.
- Max area: same component walk, count cells.
- Bipartite on a grid-graph or general graph: 2-color via BFS -
  neighbors must alternate colors; conflict = odd cycle.

SUPER SIMPLE EXAMPLE (islands):
Grid:
1 1 0
0 1 0
0 0 1
The three 1s at (0,0),(0,1),(1,1) form one island; (2,2) is
another. Answer: 2 islands.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- All water: 0 islands, flood fill changes nothing.
- All land (single component): 1 island covering everything -
  max area = rows*cols.
- Flood fill with SAME new color: infinite loop risk if you don't
  check "already this color" - the visited guard IS the fix.
- Bipartite with a self-loop: a vertex adjacent to itself is an
  immediate odd cycle -> not bipartite.
- Diagonal adjacency: this file uses 4-directional; switching to
  8-directional changes island counts (state the rule first).

DRY RUN (max area, [[1,1],[1,0]]):
DFS from (0,0): count 1 -> (0,1) count 2 -> (1,0) count 3.
(1,1) is 0. Area = 3. Other cell (1,1)=0 gives nothing.
Best = 3.

TIPS:
- "Grid as graph" removes the need to build adjacency lists -
  the neighbor deltas {{1,0},{-1,0},{0,1},{0,-1}} ARE the edges.
- Flood fill = BFS shortest-path minus the distance tracking;
  add a dist[] array and it becomes maze shortest path.
- Bipartite via 2-coloring pairs with Kuhn's matching (folder 16)
  - bipartite-ness is the precondition for assignment problems.
*/

#include <stdio.h>
#include <string.h>

#define MAX_RC 20

int grid[MAX_RC][MAX_RC];
int visited[MAX_RC][MAX_RC];
int rows;
int cols;

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

/*
 * floodFill - BFS recolor from (r,c).
 */
void floodFill(int r, int c, int newColor) {
    int oldColor = grid[r][c];
    if (oldColor == newColor) {
        printf("Cell already the target color - nothing to do\n");
        return;
    }
    int qr[400];
    int qc[400];
    int front = 0;
    int rear = 0;
    qr[rear] = r;
    qc[rear] = c;
    rear = rear + 1;
    grid[r][c] = newColor;

    while (front < rear) {
        int cr = qr[front];
        int cc = qc[front];
        front = front + 1;
        for (int d = 0; d < 4; d++) {
            int nr = cr + dr[d];
            int nc = cc + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] == oldColor) {
                grid[nr][nc] = newColor;
                qr[rear] = nr;
                qc[rear] = nc;
                rear = rear + 1;
            }
        }
    }
}

/*
 * areaDFS - Counts component size (iterative BFS).
 */
int areaBFS(int r, int c) {
    int qr[400];
    int qc[400];
    int front = 0;
    int rear = 0;
    qr[rear] = r;
    qc[rear] = c;
    rear = rear + 1;
    visited[r][c] = 1;
    int count = 0;

    while (front < rear) {
        int cr = qr[front];
        int cc = qc[front];
        front = front + 1;
        count = count + 1;
        for (int d = 0; d < 4; d++) {
            int nr = cr + dr[d];
            int nc = cc + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && grid[nr][nc] == 1) {
                visited[nr][nc] = 1;
                qr[rear] = nr;
                qc[rear] = nc;
                rear = rear + 1;
            }
        }
    }
    return count;
}

/*
 * isBipartiteGrid - 2-color the cell graph.
 */
int isBipartiteGrid(void) {
    int colorGrid[MAX_RC][MAX_RC];
    memset(colorGrid, -1, sizeof(colorGrid));

    for (int sr = 0; sr < rows; sr++) {
        for (int sc = 0; sc < cols; sc++) {
            if (colorGrid[sr][sc] != -1) {
                continue;
            }
            int qr[400];
            int qc[400];
            int front = 0;
            int rear = 0;
            qr[rear] = sr;
            qc[rear] = sc;
            rear = rear + 1;
            colorGrid[sr][sc] = 0;

            while (front < rear) {
                int cr = qr[front];
                int cc = qc[front];
                front = front + 1;
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (colorGrid[nr][nc] == -1) {
                            colorGrid[nr][nc] = 1 - colorGrid[cr][cc];
                            qr[rear] = nr;
                            qc[rear] = nc;
                            rear = rear + 1;
                        } else if (colorGrid[nr][nc] ==
                                   colorGrid[cr][cc]) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

void printGrid(void) {
    for (int i = 0; i < rows; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int choice;

    printf("Graph Problems Ladder 2 (easy -> hard)\n\n");

    printf("Rows (1-%d)? ", MAX_RC);
    scanf("%d", &rows);
    printf("Cols (1-%d)? ", MAX_RC);
    scanf("%d", &cols);

    if (rows < 1 || rows > MAX_RC || cols < 1 || cols > MAX_RC) {
        printf("Invalid size\n");
        return 1;
    }

    printf("Enter the grid (%d values per row):\n", cols);
    for (int i = 0; i < rows; i++) {
        printf("  Row %d: ", i + 1);
        for (int j = 0; j < cols; j++) {
            scanf("%d", &grid[i][j]);
        }
    }

    while (1) {
        printf("\n1. Flood fill (recolor a component)\n");
        printf("2. Number of islands (1 = land)\n");
        printf("3. Max island area\n");
        printf("4. Bipartite check (cell graph)\n");
        printf("5. Show grid\n");
        printf("6. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int r;
            int c;
            int color;
            printf("Cell row col (1-based): ");
            scanf("%d %d", &r, &c);
            printf("New color: ");
            scanf("%d", &color);
            if (r < 1 || r > rows || c < 1 || c > cols) {
                printf("Invalid cell\n");
                continue;
            }
            floodFill(r - 1, c - 1, color);
            printGrid();
        } else if (choice == 2) {
            memset(visited, 0, sizeof(visited));
            int count = 0;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        count = count + 1;
                        areaBFS(i, j);
                    }
                }
            }
            printf("Islands: %d\n", count);
        } else if (choice == 3) {
            memset(visited, 0, sizeof(visited));
            int best = 0;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        int area = areaBFS(i, j);
                        printf("  island at (%d,%d): area %d\n",
                               i + 1, j + 1, area);
                        if (area > best) {
                            best = area;
                        }
                    }
                }
            }
            printf("Max area: %d\n", best);
        } else if (choice == 4) {
            printf("Bipartite: %s\n",
                   isBipartiteGrid() ? "YES" : "NO (odd cycle)");
        } else if (choice == 5) {
            printGrid();
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
