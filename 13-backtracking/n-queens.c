/*
N Queens Problem
----------------
What this program solves:
This program places n queens on an n x n chessboard in such a way that no two
queens attack each other.

Real life example:
Think of assigning security towers in a square city grid. Each tower can monitor
its entire row, column, and diagonal. If two towers are placed in the same attack
line, they interfere with each other. So we must place them carefully so that every
tower is safe from all others. The N Queens problem follows the same logic.

DIAGRAM (4-queens: place row by row, check columns/diagonals):
  . Q . .
  . . . Q     row2 col3 ok
  Q . . .     row3 col0 ok? attacks? -> backtrack if conflicts
  . . Q .     row4 col2 -> solution found

How the process works:
1. Start from the first row.
2. Try placing a queen in each column of that row.
3. Check whether that position is safe from previously placed queens.
4. If safe, place the queen and move to the next row.
5. If the next row fails, come back and try another column in the previous row.
6. Continue until all rows are filled.

Pseudo code:
START
Read n
Try to place queens row by row recursively
For each column in current row
    If placing queen is safe
        Place the queen
        If last row reached
            Print solution
        Else
            Recurse for next row
If no column works
    Backtrack to previous row
If no solution found at all
    Print no solution
END

Sample input and dry run:
For n = 4, place the first queen in row 1.
Then try safe positions in row 2, row 3, and row 4.
If a row gets no safe column, return to the previous row and change the position.
One valid arrangement is:
row 1 -> column 2, row 2 -> column 4, row 3 -> column 1, row 4 -> column 3
This places all 4 queens without row, column, or diagonal conflict.

Main logic to understand:
This is a backtracking method. It moves forward when a choice is valid and steps
back when that choice later leads to failure.
*/
#include <stdio.h>
#include <stdlib.h>

int x[20];
int n;
int solutionCount = 0;

int place(int k, int i) {
    int j;

    for (j = 1; j < k; j++) {
        if (x[j] == i || abs(x[j] - i) == abs(j - k)) {
            return 0;
        }
    }

    return 1;
}

void nQueens(int k) {
    int i, j;

    for (i = 1; i <= n; i++) {
        if (place(k, i)) {
            x[k] = i;

            if (k == n) {
                solutionCount++;
                printf("\nSolution %d:\n", solutionCount);

                for (i = 1; i <= n; i++) {
                    for (j = 1; j <= n; j++) {
                        if (x[i] == j) {
                            printf("Q ");
                        } else {
                            printf(". ");
                        }
                    }
                    printf("\n");
                }
            } else {
                nQueens(k + 1);
            }
        }
    }
}

int main() {
    printf("N Queens Problem\n");
    printf("Enter the number of queens: ");
    scanf("%d", &n);

    nQueens(1);

    if (solutionCount == 0) {
        printf("No solution exists.\n");
    }

    return 0;
}

