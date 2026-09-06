/*
Backtracking Problems Ladder - Subsets, Combinations, Word Search (Basic -> Hard)
----------------------------------------------------------------------------------
Real life example:
Feature bundles (all subsets within budget), lottery ticket
combinations and Boggle/word-game engines all run these
choose-explore-unchoose searches.

DIAGRAM (subset tree for [1,2] - include or exclude each item):
              {}
            /     \
        {1}         {}
       /   \       /   \
    {1,2}  {1}  {2}    {}
  word search: mark cell visited, recurse 4 ways, UNMARK on return.

LADDER (easy -> hard inside this file):
  1. All subsets (power set)      (2^n, choose/include)
  2. Combinations of size k       (prune by remaining slots)
  3. Word search in a grid        (2D backtracking with visited marking)

BEGINNER EXPLANATION:
Backtracking = enumerate choices, recurse, UNDO.
- Subsets: for each element decide IN or OUT - a binary tree of
  decisions, 2^n leaves.
- Combinations: same tree but with a RULE: only look forward
  (start index increases) - kills permutations-as-subsets and
  prunes branches that cannot reach size k.
- Word search: at each cell, try the 4 directions, mark visited,
  recurse, UNMARK. The mark/unmark pair is the grid equivalent of
  swap-back.

SUPER SIMPLE EXAMPLE (subsets of [1,2]):
decide 1: in -> [1], decide 2: in [1,2], out [1]
          out -> [], 2 in [2], out []
Power set: {[], [1], [2], [1,2]} = 2^2 = 4.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Duplicates in input for subsets: [1,2,2] gives duplicate subsets
  unless you sort and skip equal siblings - noted, not implemented
  here (same dedup idea as string_permutations.c).
- k > n or k <= 0: no combinations exist - the prune catches it
  before any output.
- Word search single letter: found if any cell matches.
- Word search reusing a cell: forbidden (visited) - that is what
  the mark prevents; a snake path through the same letter twice
  must fail, and the unmark lets OTHER paths try it later.
- Word longer than total cells: impossible - quick reject.

DRY RUN (word search, grid AB/CD, word "ABDC"):
(0,0)=A matches. Mark A.
 try right (0,1)=B matches. Mark B.
  try down (1,1)=D matches. Mark D.
   try up (0,1) visited. try left (1,0)=C matches. Mark C.
    all consumed -> SUCCESS.
Unwind all unmarks (even on success they run - harmless).

TIPS:
- The three rungs share one skeleton: loop choices -> recurse ->
  undo. Master it here and Sudoku/N-Queens (folder 13 originals)
  become pattern applications.
- The start-index rule (combinations) vs no-rule (permutations)
  vs 2D-visited (grid) are the three canonical constraint types -
  every backtracking problem mixes them.
- Complexity: subsets 2^n, combinations C(n,k), word search
  O(rows*cols*4^len) worst - pruning is what makes them usable.
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 15
#define MAX_GRID 8

int chosen[MAX_N];

/*
 * printSubset - Current selection.
 */
void printSubset(int depth) {
    printf("[");
    for (int i = 0; i < depth; i++) {
        if (chosen[i]) {
            printf("%d ", chosen[i]);
        }
    }
    printf("]\n");
}

/*
 * subsets - Include/exclude recursion.
 */
void subsets(const int arr[], int idx, int n, int depth) {
    if (idx == n) {
        printf("[");
        for (int i = 0; i < n; i++) {
            if (chosen[i]) {
                printf("%d ", arr[i]);
            }
        }
        printf("]\n");
        return;
    }
    chosen[idx] = 0;
    subsets(arr, idx + 1, n, depth);
    chosen[idx] = 1;
    subsets(arr, idx + 1, n, depth);
    chosen[idx] = 0;
}

/*
 * combinations - Only forward picks (start index), size k.
 */
void combinations(const int arr[], int n, int start, int k,
                  int result[], int depth) {
    if (depth == k) {
        printf("[");
        for (int i = 0; i < k; i++) {
            printf("%d ", result[i]);
        }
        printf("]\n");
        return;
    }
    for (int i = start; i < n; i++) {
        /* prune: not enough elements left to fill k slots */
        if (n - i < k - depth) {
            break;
        }
        result[depth] = arr[i];
        combinations(arr, n, i + 1, k, result, depth + 1);
    }
}

/*
 * existWord - Grid word search with mark/unmark.
 */
int existWord(char grid[][MAX_GRID], int rows, int cols,
              const char* word, int r, int c, int pos) {
    if (word[pos] == '\0') {
        return 1;
    }
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return 0;
    }
    if (grid[r][c] != word[pos]) {
        return 0;
    }

    char saved = grid[r][c];
    grid[r][c] = '#';

    int found = existWord(grid, rows, cols, word, r + 1, c, pos + 1) ||
                existWord(grid, rows, cols, word, r - 1, c, pos + 1) ||
                existWord(grid, rows, cols, word, r, c + 1, pos + 1) ||
                existWord(grid, rows, cols, word, r, c - 1, pos + 1);

    grid[r][c] = saved;
    return found;
}

int main() {
    int arr[MAX_N];
    int n;
    int choice;

    printf("Backtracking Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. All subsets of n numbers\n");
        printf("2. Combinations of size k\n");
        printf("3. Word search in a grid\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("How many numbers (1-%d)? ", 12);
            scanf("%d", &n);
            if (n < 1 || n > 12) {
                printf("2^12 = 4096 lines max\n");
                continue;
            }
            printf("Enter %d numbers: ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            printf("Power set (%d subsets):\n", 1 << n);
            subsets(arr, 0, n, 0);
        } else if (choice == 2) {
            printf("How many numbers (1-%d)? ", 12);
            scanf("%d", &n);
            printf("Enter %d numbers: ", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &arr[i]);
            }
            int k;
            printf("k: ");
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("k must be 1..%d\n", n);
                continue;
            }
            int result[MAX_N];
            printf("Combinations of size %d:\n", k);
            combinations(arr, n, 0, k, result, 0);
        } else if (choice == 3) {
            int rows;
            int cols;
            char grid[MAX_GRID][MAX_GRID];
            char word[30];

            printf("Rows (1-8) and cols (1-8): ");
            scanf("%d %d", &rows, &cols);
            printf("Enter %d rows of %d letters (no spaces):\n", rows, cols);
            for (int i = 0; i < rows; i++) {
                printf("  Row %d: ", i + 1);
                char rowStr[50];
                scanf("%49s", rowStr);
                for (int j = 0; j < cols; j++) {
                    grid[i][j] = rowStr[j];
                }
            }
            printf("Word to search: ");
            scanf("%29s", word);

            int found = 0;
            for (int r = 0; r < rows && !found; r++) {
                for (int c = 0; c < cols && !found; c++) {
                    if (existWord(grid, rows, cols, word, r, c, 0)) {
                        found = 1;
                    }
                }
            }
            printf("Word \"%s\": %s\n", word, found ? "FOUND" : "not found");
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
