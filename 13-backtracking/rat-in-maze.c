/*
Rat in a Maze (Backtracking)
----------------------------
What this program solves:
A rat starts at the top-left of an n x n grid and must reach the
bottom-right, moving only through open cells (1) - not walls (0) -
printing one successful path.

Real life example:
Robot navigation in a warehouse: some aisles are blocked; find any
route from the loading dock to the exit. Also maze generation
testing and network routing demonstrations.

DIAGRAM (1 = open, follow a path, backtrack at walls):
  1 0 1        rat moves R/D/L/U, marks visited cells;
  1 1 1        path printed when it reaches bottom-right.
  0 1 1        dead end -> unmark and try the next direction.

How the process works:
1. sol[i][j] = 0 everywhere; rat starts at (0, 0).
2. At (x, y): if it is the goal, mark it and report success.
3. A cell is safe if it is inside the grid, maze[x][y] == 1, and
   not already on the current path.
4. Mark, recurse right then down (order matters little), and if
   both fail, unmark (backtrack) and return failure.

Pseudo code:
START
SOLVE(x, y):
    IF (x, y) == (n-1, n-1): sol[x][y]=1; RETURN true
    IF safe(x, y):
        sol[x][y] = 1
        IF SOLVE(x+1, y): return true
        IF SOLVE(x, y+1): return true
        sol[x][y] = 0   (backtrack)
    RETURN false
END

Sample input and dry run:
maze = 1 0 0 0
       1 1 0 1
       0 1 0 1
       1 1 1 1
path: (0,0) (1,0) (1,1) (2,1) (3,1) (3,2) (3,3)

Main logic to understand:
DFS with path-marking and undo: marking prevents cycles, unmarking
lets the cell be reused by a different route. O(2^(n^2)) worst,
fast in practice. Easy to extend to all four directions or
weighted mazes (that becomes shortest path, a BFS/Dijkstra job).
*/

#include <stdio.h>

#define MAX_N 20

int maze[MAX_N][MAX_N];
int sol[MAX_N][MAX_N];
int n;

/*
 * printSolution - The winning path.
 */
void printSolution(void) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%2d ", sol[r][c]);
        }
        printf("\n");
    }
}

/*
 * isSafe - Inside grid, open cell, not already on path.
 */
int isSafe(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < n &&
           maze[x][y] == 1 && sol[x][y] == 0;
}

/*
 * solveMaze - Tries right then down, backtracking on failure.
 */
int solveMaze(int x, int y) {
    if (x == n - 1 && y == n - 1) {
        sol[x][y] = 1;
        return 1;
    }

    if (isSafe(x, y)) {
        sol[x][y] = 1;

        if (solveMaze(x, y + 1)) {
            return 1;
        }
        if (solveMaze(x + 1, y)) {
            return 1;
        }

        sol[x][y] = 0;
    }
    return 0;
}

int main() {
    printf("Rat in a Maze (Backtracking)\n\n");

    printf("Maze size n (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid size\n");
        return 1;
    }

    printf("Enter the maze row by row (%d values each, 1 = open, 0 = wall):\n", n);
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            scanf("%d", &maze[r][c]);
            sol[r][c] = 0;
        }
    }

    if (maze[0][0] == 0 || maze[n - 1][n - 1] == 0) {
        printf("\nStart or destination is blocked - no path\n");
        return 0;
    }

    if (solveMaze(0, 0)) {
        printf("\nPath found (1 = route taken):\n");
        printSolution();
    } else {
        printf("\nNo path exists from top-left to bottom-right\n");
    }

    return 0;
}
