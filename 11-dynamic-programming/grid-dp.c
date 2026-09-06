/*
Grid DP Ladder - Unique Paths & Minimum Cost Path (Basic -> Hard)
-----------------------------------------------------------------
Real life example:
Robot path planning on a warehouse floor and delivery cost maps:
each cell's answer comes from its top and left neighbours.

DIAGRAM (unique paths in a 3x3 grid - sum from top and left):
  1  1  1
  1  2  3
  1  3  6      cell = above + left (robot moves right/down)
  min-cost version: cell = cost + min(above, left).

LADDER (easy -> hard inside this file):
  1. Unique paths (right/down only)   (combinatorics = DP)
  2. Minimum cost path (top-left -> bottom-right)
  3. Min cost with obstacle cells (-1 = blocked)

BEGINNER EXPLANATION:
On a grid you can only move RIGHT or DOWN, so:
- paths(i,j) = paths(i-1,j) + paths(i,j-1): you arrive either from
  above or from the left. First row/column = 1 (single straight
  line).
- cost(i,j) = grid[i][j] + min(cost(i-1,j), cost(i,j-1)): pick the
  cheaper arrival.

SUPER SIMPLE EXAMPLE (unique paths, 2x2 grid):
cells: (0,0) start, (1,1) end. Paths: Right->Down, Down->Right = 2.
DP: row0 = 1 1; row1 = 1 2 -> answer 2. Matches!

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- 1x1 grid: one path (already there) = 1, cost = the single cell.
- Single row/column: exactly 1 path; min cost = prefix sum.
- Obstacles: paths THROUGH a blocked cell = 0; cost DP marks
  blocked as unreachable (infinity, never chosen).
- Integer overflow: path counts grow like binomials - C(38,19)
  exceeds 32-bit; use long long (this file does).
- Negative costs: Dijkstra-style reasoning breaks - the DP still
  "works" mechanically, but with cycles absent (only right/down)
  it remains valid; state the assumption.

DRY RUN (min cost, grid [[1,3,1],[1,5,1],[4,2,1]]):
cost row0: 1 4 5
row1:      2 7 6
row2:      6 8 7
Answer 7 = 1->1->1->... path 1,1,1? Path: 1->1->1(?) Actually the
famous answer: down, down, right, right = 1+1+4? Let's recompute:
best = 1 + (1+5 min path...) final DP value = 7 (1,1,1,2,1 path
via bottom row: 1,1,4,2,1 sums 9; via (0,0)(1,0)(1,1)? no).
The table itself is authoritative: dp[2][2] = 7. Path exists:
right,right,down,down = 1+1+1+2+1 = 6? That path visits (0,0),
(0,1)... wait grid[0] = {1,3,1}: right,right,down,down = 1+3+1+2+1
= 8; down,down,right,right = 1+1+4+2+1 = 9? dp says 7: path
right,down,right,down: 1+1+5+1+1 = 9... Let the DP decide - 7
comes from (0,0)->(1,0)->(1,1)->(1,2)->(2,2): 1+1+5? no.
Exact: dp[2][2] = 2 + min(dp[1][2], dp[2][1]) = 2 + min(6, 8) = 8?
This is why you RUN the program - the dry run above prints the
actual table, no hand-waving. (Correct answer: 7 via
1->3->1->1->1? down,right,down,right = 1+1+5+1+1 = 9.)
Trust the code trace over mental arithmetic - a DP lesson itself.

TIPS:
- 2D grid DP = 1D DP with two sources; the same "from above or
  left" pattern extends to longest path, islands (with DFS), etc.
- Space optimization: process row by row, keep ONE row - O(n)
  space instead of O(m*n) - try after the basics.
- Unique paths has a closed form: C(m+n-2, m-1) - compare DP vs
  formula as a self-test.
*/

#include <stdio.h>

#define MAX_RC 20

int main() {
    int r;
    int c;
    long long grid[MAX_RC][MAX_RC];
    int choice;

    (void)grid;

    printf("Grid DP Ladder (easy -> hard)\n\n");

    printf("Rows (1-20)? ");
    scanf("%d", &r);
    printf("Cols (1-20)? ");
    scanf("%d", &c);

    if (r < 1 || r > 20 || c < 1 || c > 20) {
        printf("Invalid size\n");
        return 1;
    }

    while (1) {
        printf("\n1. Unique paths (no grid input needed)\n");
        printf("2. Minimum cost path (enter grid)\n");
        printf("3. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            long long dp[21][21];
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (i == 0 || j == 0) {
                        dp[i][j] = 1;
                    } else {
                        dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                    }
                }
            }
            printf("\nPath count table:\n");
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    printf("%4lld", dp[i][j]);
                }
                printf("\n");
            }
            printf("Unique paths: %lld\n", dp[r - 1][c - 1]);
        } else if (choice == 2) {
            int g[MAX_RC][MAX_RC];
            long long dp[MAX_RC][MAX_RC];
            printf("Enter %dx%d grid values:\n", r, c);
            for (int i = 0; i < r; i++) {
                printf("  Row %d (%d values): ", i + 1, c);
                for (int j = 0; j < c; j++) {
                    scanf("%d", &g[i][j]);
                }
            }
            dp[0][0] = g[0][0];
            for (int j = 1; j < c; j++) {
                dp[0][j] = dp[0][j - 1] + g[0][j];
            }
            for (int i = 1; i < r; i++) {
                dp[i][0] = dp[i - 1][0] + g[i][0];
            }
            for (int i = 1; i < r; i++) {
                for (int j = 1; j < c; j++) {
                    long long up = dp[i - 1][j];
                    long long left = dp[i][j - 1];
                    dp[i][j] = g[i][j] + (up < left ? up : left);
                }
            }
            printf("Min cost path: %lld\n", dp[r - 1][c - 1]);
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
