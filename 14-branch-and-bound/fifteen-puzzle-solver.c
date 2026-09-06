/*
15-Puzzle Solver (Branch and Bound / IDA*)
------------------------------------------
What this program solves:
Solves the classic sliding puzzle: given a 3x3 (8-puzzle) board,
find the minimum number of moves to reach the goal arrangement
using iterative-deepening A* with the Manhattan-distance heuristic.

Real life example:
The physical sliding tile toy. The same "search with an admissible
heuristic" engine drives route planners, game AI, and robot arm
movement planning.

DIAGRAM (best-first search ordered by f = g + heuristic):
  g = moves made so far, h = misplaced tiles / Manhattan distance
  open set: expand the SMALLEST f first (priority queue)
  dead branches die because their f already exceeds the optimum.

How the process works:
1. State = 3x3 board (0 = blank). Moves slide a tile into the blank.
2. Heuristic h = sum over tiles of Manhattan distance to the goal
   position (admissible: each move changes h by at most 1).
3. IDA*: DFS bounded by f = g + h; deepen the bound to the smallest
   f that exceeded the previous bound.
4. First iteration to reach the goal yields an OPTIMAL solution.

Pseudo code:
START
SEARCH(node, g, bound):
    f = g + manhattan(node)
    IF f > bound: RETURN f (new minimum bound)
    IF node == goal: RETURN found
    FOR each neighbour move:
        nextBound = min(nextBound, SEARCH(child, g+1, bound))
    RETURN nextBound
BOUND = manhattan(root)
WHILE not found: BOUND = SEARCH(root, 0, BOUND)
END

Sample input and dry run:
Enter the initial 3x3 board (0 = blank), e.g.
1 2 3
4 0 6
7 5 8
Solved in 2 moves: slide 5 down, slide 8 left.

Main logic to understand:
A* logic inside an IDDFS loop: memory-friendly (no priority queue)
and optimal, because Manhattan distance never overestimates. The
8-puzzle state space (9!/2) is tiny; 15-puzzle needs the same
code with more patience.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 3

int goal[SIZE][SIZE] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

int board[SIZE][SIZE];
int blankR;
int blankC;
int nodesExpanded = 0;

/*
 * manhattan - Sum of tile distances to goal (0 tile ignored).
 */
int manhattan(void) {
    int sum = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int v = board[r][c];
            if (v != 0) {
                int gr = (v - 1) / SIZE;
                int gc = (v - 1) % SIZE;
                int dr = r - gr;
                int dc = c - gc;
                if (dr < 0) {
                    dr = -dr;
                }
                if (dc < 0) {
                    dc = -dc;
                }
                sum = sum + dr + dc;
            }
        }
    }
    return sum;
}

/*
 * isGoal - Board equals the target arrangement.
 */
int isGoal(void) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] != goal[r][c]) {
                return 0;
            }
        }
    }
    return 1;
}

/*
 * search - IDA* depth-first search with f-bound.
 */
int search(int g, int bound, int prevMove) {
    int h = manhattan();
    int f = g + h;

    if (f > bound) {
        return f;
    }
    if (isGoal()) {
        return -1;
    }

    nodesExpanded = nodesExpanded + 1;

    int minNext = 1000000;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int m = 0; m < 4; m++) {
        if (prevMove != -1 && (m ^ 1) == prevMove) {
            continue;
        }
        int nr = blankR + dr[m];
        int nc = blankC + dc[m];
        if (nr < 0 || nr >= SIZE || nc < 0 || nc >= SIZE) {
            continue;
        }

        board[blankR][blankC] = board[nr][nc];
        board[nr][nc] = 0;
        int oldBlankR = blankR;
        int oldBlankC = blankC;
        blankR = nr;
        blankC = nc;

        int t = search(g + 1, bound, m);
        if (t == -1) {
            return -1;
        }
        if (t < minNext) {
            minNext = t;
        }

        blankR = oldBlankR;
        blankC = oldBlankC;
        board[nr][nc] = board[blankR][blankC];
        board[blankR][blankC] = 0;
    }
    return minNext;
}

int main() {
    printf("8-Puzzle Solver (IDA* with Manhattan heuristic)\n\n");
    printf("Enter the 3x3 board row by row (0 = blank), e.g.:\n");
    printf("1 2 3\n4 0 6\n7 5 8\n\n");

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            scanf("%d", &board[r][c]);
            if (board[r][c] == 0) {
                blankR = r;
                blankC = c;
            }
        }
    }

    int seen[9] = {0};
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int v = board[r][c];
            if (v < 0 || v > 8 || seen[v]) {
                printf("Invalid board (values 0-8 each exactly once)\n");
                return 1;
            }
            seen[v] = 1;
        }
    }

    /* solvability: inversion count parity */
    int arr[9];
    int k = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] != 0) {
                arr[k] = board[r][c];
                k = k + 1;
            }
        }
    }
    int inversions = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            if (arr[i] > arr[j]) {
                inversions = inversions + 1;
            }
        }
    }
    if (inversions % 2 != 0) {
        printf("\nUnsolvable (odd inversion count %d)\n", inversions);
        return 0;
    }

    int bound = manhattan();
    printf("Initial Manhattan distance: %d\n", bound);

    while (1) {
        int t = search(0, bound, -1);
        if (t == -1) {
            printf("Solved optimally in %d moves\n", bound);
            printf("Nodes expanded: %d\n", nodesExpanded);
            break;
        }
        if (t >= 1000000) {
            printf("No solution found within bound\n");
            break;
        }
        bound = t;
    }

    return 0;
}
