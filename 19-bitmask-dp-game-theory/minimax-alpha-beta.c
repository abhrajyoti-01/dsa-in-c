/*
Minimax with Alpha-Beta Pruning - Perfect Tic-Tac-Toe (Advanced)
---------------------------------------------------------------
PROBLEM DEFINITION:
Given a tic-tac-toe position, decide its value with perfect play (X
wins, O wins, or draw) and the best move. Minimax searches the whole
game tree: X picks the child with the MAXIMUM value, O the MINIMUM.
Alpha-beta pruning returns exactly the same value but skips branches
that cannot change the decision. The program runs both and counts the
positions each one visits.

REAL-LIFE EXAMPLE:
Chess and checkers engines, and AI opponents in board games, are
minimax with alpha-beta at heart (plus a depth limit and an evaluation
function). Adversarial planning - "what is my best move if my opponent
answers perfectly?" - is the same reasoning.

DIAGRAM (alpha-beta cut, X to move at the root):
            X: max
          /       \
       O: min      O: min
       /  \        /    \
      3    5      2      ?        first child is worth min(3,5) = 3,
                                  so alpha = 3 at the root. In the
  second child O already has a reply worth 2 <= alpha: X will never
  choose this branch, whatever "?" is - so "?" is never searched.

PSEUDO CODE:
  Score: X win = 10 - depth, O win = depth - 10, draw = 0
         (prefer fast wins and slow losses)
  AlphaBeta(position, depth, alpha, beta):
      if game over: return score
      if X to move:
          for each move: v = AlphaBeta(child, depth+1, alpha, beta)
                         alpha = max(alpha, v);  if alpha >= beta: stop
          return alpha
      else:
          for each move: v = AlphaBeta(child, depth+1, alpha, beta)
                         beta = min(beta, v);    if alpha >= beta: stop
          return beta
  Plain minimax is the same without alpha, beta and the "stop".

SAMPLE INPUT & DRY RUN:
  2
  ...
  ...
  ...
  X..
  .O.
  ..X
  Empty board, X to move: every first move is worth 0 (a draw), so
  the first cell, row 1 column 1, is reported.
      Positions visited: minimax 549946, alpha-beta 20866
  Second board, O to move - the famous trap:
          X   0   6          value of each empty cell for O's move:
          0   O   0          a CORNER loses (+6: X forks and wins in
          6   0   X          4 plies), an EDGE holds the draw (0)
      Value 0: best move row 1, column 2 (the first edge)
      Positions visited: minimax 1053, alpha-beta 356

SPECIAL CASES:
  - Whose turn it is comes from the piece counts: X moves first, so
    X count = O count means X to move, X = O + 1 means O to move.
  - Positions that are already won or full are reported, not searched.
  - Impossible boards (bad counts, two winners) are rejected.
  - Ties between equally good moves go to the first cell in reading
    order, for both methods, so they always agree on the move.
  - Up to 10 positions; rows use 'X', 'O' and '.'.

COMPLEXITY:
  Minimax: O(b^d) nodes (549,946 from the empty board).
  Alpha-beta: O(b^(d/2)) with perfect move ordering, O(b^d) worst case.
  Space O(d) for the recursion.

TIPS:
  Alpha = the best value X is already guaranteed; beta = the best O is
  guaranteed. Move ordering decides how much alpha-beta prunes: try
  the most promising moves first. Real engines add memoization of
  positions (transposition tables) - tic-tac-toe has only 5,478 legal
  positions. Compare nim-game (games solved by a formula instead of
  search).
*/

#include <stdio.h>
#include <string.h>

#define MAX_POSITIONS 10
#define WIN_SCORE 10

char board[9];
long long nodes;

static const int lines[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

/* Counts completed lines for player p. */
int linesOf(char p) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (board[lines[i][0]] == p && board[lines[i][1]] == p &&
            board[lines[i][2]] == p) {
            count++;
        }
    }
    return count;
}

int isFull(void) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == '.') return 0;
    }
    return 1;
}

/* Returns 1 and sets *score if the game is over. */
int terminal(int depth, int *score) {
    if (linesOf('X') > 0) {
        *score = WIN_SCORE - depth;
        return 1;
    }
    if (linesOf('O') > 0) {
        *score = depth - WIN_SCORE;
        return 1;
    }
    if (isFull()) {
        *score = 0;
        return 1;
    }
    return 0;
}

int minimax(int depth, int xToMove) {
    int score;
    nodes++;
    if (terminal(depth, &score)) return score;
    int best = xToMove ? -100 : 100;
    for (int i = 0; i < 9; i++) {
        if (board[i] != '.') continue;
        board[i] = xToMove ? 'X' : 'O';
        int v = minimax(depth + 1, !xToMove);
        board[i] = '.';
        if (xToMove ? v > best : v < best) best = v;
    }
    return best;
}

int alphaBeta(int depth, int xToMove, int alpha, int beta) {
    int score;
    nodes++;
    if (terminal(depth, &score)) return score;
    for (int i = 0; i < 9; i++) {
        if (board[i] != '.') continue;
        board[i] = xToMove ? 'X' : 'O';
        int v = alphaBeta(depth + 1, !xToMove, alpha, beta);
        board[i] = '.';
        if (xToMove && v > alpha) alpha = v;
        if (!xToMove && v < beta) beta = v;
        if (alpha >= beta) break;
    }
    return xToMove ? alpha : beta;
}

void describe(int value) {
    int plies = value > 0 ? WIN_SCORE - value : WIN_SCORE + value;
    if (value == 0) printf("draw with perfect play");
    else printf("%c wins in %d pl%s", value > 0 ? 'X' : 'O', plies,
                plies == 1 ? "y" : "ies");
}

int main(void) {
    int count;

    printf("Minimax and Alpha-Beta - Tic-Tac-Toe\n\n");
    printf("How many positions (1-%d)? ", MAX_POSITIONS);
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_POSITIONS) {
        printf("Invalid count\n");
        return 1;
    }
    for (int p = 0; p < count; p++) {
        char row[3][8];
        int xs = 0;
        int os = 0;
        printf("Position %d: three rows of X, O, '.': ", p + 1);
        for (int r = 0; r < 3; r++) {
            if (scanf("%7s", row[r]) != 1 || strlen(row[r]) != 3) {
                printf("Invalid row\n");
                return 1;
            }
            for (int c = 0; c < 3; c++) {
                char ch = row[r][c];
                if (ch != 'X' && ch != 'O' && ch != '.') {
                    printf("Invalid cell '%c'\n", ch);
                    return 1;
                }
                board[3 * r + c] = ch;
                xs += ch == 'X';
                os += ch == 'O';
            }
        }
        if ((xs != os && xs != os + 1) ||
            (linesOf('X') > 0 && linesOf('O') > 0)) {
            printf("\nImpossible position\n");
            return 1;
        }
        int xToMove = xs == os;
        int score;
        printf("\n%s\n%s\n%s\n", row[0], row[1], row[2]);
        if (terminal(0, &score)) {
            printf("Game already over: %s\n\n",
                   score > 0 ? "X has won" : (score < 0 ? "O has won" : "draw"));
            continue;
        }
        printf("%c to move. Value of each empty cell "
               "(+ X wins, - O wins, 0 draw):\n", xToMove ? 'X' : 'O');

        nodes = 1;
        int best = xToMove ? -100 : 100;
        int bestMove = -1;
        int value[9];
        for (int i = 0; i < 9; i++) {
            if (board[i] != '.') continue;
            board[i] = xToMove ? 'X' : 'O';
            value[i] = minimax(1, !xToMove);
            board[i] = '.';
            if (xToMove ? value[i] > best : value[i] < best) {
                best = value[i];
                bestMove = i;
            }
        }
        long long minimaxNodes = nodes;
        for (int r = 0; r < 3; r++) {
            printf("   ");
            for (int c = 0; c < 3; c++) {
                int i = 3 * r + c;
                if (board[i] == '.') printf("%4d", value[i]);
                else printf("%4c", board[i]);
            }
            printf("\n");
        }

        nodes = 1;
        int alpha = -100;
        int beta = 100;
        int abMove = -1;
        for (int i = 0; i < 9; i++) {
            if (board[i] != '.') continue;
            board[i] = xToMove ? 'X' : 'O';
            int v = alphaBeta(1, !xToMove, alpha, beta);
            board[i] = '.';
            if (xToMove && v > alpha) {
                alpha = v;
                abMove = i;
            }
            if (!xToMove && v < beta) {
                beta = v;
                abMove = i;
            }
        }
        int abValue = xToMove ? alpha : beta;

        printf("Value %d: ", best);
        describe(best);
        printf("\nBest move: row %d, column %d\n", bestMove / 3 + 1,
               bestMove % 3 + 1);
        printf("Positions visited: minimax %lld, alpha-beta %lld "
               "(value %d, same move: %s)\n\n", minimaxNodes, nodes, abValue,
               abMove == bestMove ? "yes" : "NO");
    }
    return 0;
}
