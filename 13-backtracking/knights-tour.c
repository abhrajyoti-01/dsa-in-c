/*
Knight's Tour (Backtracking)
----------------------------
What this program solves:
Places a knight on an n x n chessboard and finds a sequence of
moves visiting EVERY square exactly once (an open knight's tour),
printing the step numbers on the board.

Real life example:
Path planning with a restricted mover - the same backtracking
pattern solves robot motion where only certain moves are legal.
Warnsdorff heuristics for this problem inspire modern heuristic
search.

DIAGRAM (knight jumps; mark square, recurse, unmark on dead end):
  8 candidate moves from each square (2+1 shaped):
  . K . .      try moves in order; a full 64-square path = tour
  . . . .
  K . . .      (X moves L-shapes around the board)

How the process works:
1. board[x][y] = 0 everywhere (unvisited); place the knight at
   (0, 0) as step 1.
2. From (x, y) try all 8 knight moves in order.
3. A move is valid if inside the board and the square is 0.
4. Mark the square with the step number; recurse.
5. If step count reaches n*n, a full tour exists; otherwise
   undo (set back to 0) and try the next move.

Pseudo code:
START
TOUR(x, y, move):
    IF move == n*n: RETURN true
    FOR each of 8 offsets (dx, dy)
        nx = x+dx, ny = y+dy
        IF inside AND board[nx][ny] == 0:
            board[nx][ny] = move+1
            IF TOUR(nx, ny, move+1): return true
            board[nx][ny] = 0
    RETURN false
END

Sample input and dry run:
5x5 board, start (0,0):
1st column of the printed board shows step numbers like
1, 14, 9, 20, 3 ... a complete 25-step tour.

Main logic to understand:
Depth-first search over move sequences with explicit undo - the
classic backtracking template. Worst case exponential, but the
move ordering + constraint (must be unvisited) prunes heavily;
5x5 solves in milliseconds. Board sizes >= 5 work well.
*/

#include <stdio.h>

#define MAX_N 8

int board[MAX_N][MAX_N];
int n;

int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

/*
 * printBoard - Step numbers on the board.
 */
void printBoard(void) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%3d ", board[r][c]);
        }
        printf("\n");
    }
}

/*
 * knightsTour - Recursive backtracking; move = next step number.
 */
int knightsTour(int x, int y, int move) {
    if (move == n * n) {
        return 1;
    }
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == 0) {
            board[nx][ny] = move + 1;
            if (knightsTour(nx, ny, move + 1)) {
                return 1;
            }
            board[nx][ny] = 0;
        }
    }
    return 0;
}

int main() {
    printf("Knight's Tour (Backtracking)\n\n");

    printf("Board size n (5-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 5 || n > MAX_N) {
        printf("Use size between 5 and %d (smaller boards may have no tour)\n",
               MAX_N);
        return 1;
    }

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            board[r][c] = 0;
        }
    }

    board[0][0] = 1;

    if (knightsTour(0, 0, 1)) {
        printf("\nKnight's tour found (numbers = visit order):\n");
        printBoard();
    } else {
        printf("\nNo tour exists for %dx%d from (0,0)\n", n, n);
    }

    return 0;
}
