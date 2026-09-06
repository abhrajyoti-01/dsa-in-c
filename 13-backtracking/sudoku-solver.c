/*
Sudoku Solver (Backtracking)
----------------------------
What this program solves:
Fills a partially-filled 9x9 Sudoku grid using backtracking:
each empty cell tries digits 1-9, checking row/column/box
constraints, undoing on conflict.

Real life example:
Newspaper Sudoku puzzles. The same backtracking-with-pruning
pattern also solves constraint problems in timetabling and
chip layout.

DIAGRAM (place 1-9 in the first empty cell; undo on conflict):
  row check + column check + 3x3 box check:
  . . 3 | . 2 . | . . .     cell (0,0): try 1..9, first value with
  4 . . | . . 9 | . 1 .     no clash in row/col/box is placed;
  ...                       failure later -> backtrack here.

How the process works:
1. Find the first empty cell (value 0).
2. Try digits 1-9: valid if the digit is absent from that row,
   that column, and the 3x3 box.
3. Place the digit and recurse to the next empty cell.
4. If a conflict arises later, reset the cell to 0 and try the
   next digit (backtrack).
5. No empty cells left -> solved.

Pseudo code:
START
SOLVE():
    find empty cell (r, c)
    IF none: RETURN true
    FOR d = 1..9
        IF rowOK AND colOK AND boxOK:
            grid[r][c] = d
            IF SOLVE(): return true
            grid[r][c] = 0
    RETURN false
END

Sample input and dry run:
Classic puzzle with ~30 given cells; the solver fills the rest,
verifying each placement against its row/column/box instantly.

Main logic to understand:
Constraint propagation by trial: each placement is validated
locally in O(1)-ish work (27 cells), pruning the exponential tree
drastically. Enter 0 for empty cells; 81 values total.
*/

#include <stdio.h>

#define N 9

int grid[N][N];

/*
 * printGrid - Board with box separators.
 */
void printGrid(void) {
    for (int r = 0; r < N; r++) {
        if (r % 3 == 0 && r != 0) {
            printf("------+-------+------\n");
        }
        for (int c = 0; c < N; c++) {
            if (c % 3 == 0 && c != 0) {
                printf("| ");
            }
            printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
}

/*
 * isSafe - Digit d violates no row, column, or box.
 */
int isSafe(int row, int col, int d) {
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == d || grid[i][col] == d) {
            return 0;
        }
    }
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int r = boxRow; r < boxRow + 3; r++) {
        for (int c = boxCol; c < boxCol + 3; c++) {
            if (grid[r][c] == d) {
                return 0;
            }
        }
    }
    return 1;
}

/*
 * solveSudoku - Backtracking over empty cells.
 */
int solveSudoku(void) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                for (int d = 1; d <= 9; d++) {
                    if (isSafe(row, col, d)) {
                        grid[row][col] = d;
                        if (solveSudoku()) {
                            return 1;
                        }
                        grid[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    printf("Sudoku Solver (Backtracking)\n\n");
    printf("Enter 81 values row by row (0 = empty cell).\n");
    printf("You can enter all 81 separated by spaces/newlines:\n\n");

    int ok = 1;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (scanf("%d", &grid[r][c]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (grid[r][c] < 0 || grid[r][c] > 9) {
                ok = 0;
            }
        }
    }

    if (!ok) {
        printf("Values must be 0-9\n");
        return 1;
    }

    printf("\nPuzzle:\n");
    printGrid();

    if (solveSudoku()) {
        printf("\nSolved:\n");
        printGrid();
    } else {
        printf("\nNo solution exists for this puzzle\n");
    }

    return 0;
}
