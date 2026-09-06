/*
2D Matrix Problems Ladder (Basic -> Hard)
------------------------------------------
Real life example:
Plotters, CNC machines and image-rotation kernels fill or scan a
square area layer by layer from the outside in - a spiral walk.

DIAGRAM (spiral order of a 3x3 grid - peel boundaries):
  1  2  3       top row ->        1 2 3
  4  5  6       right col down    6 9 8 7 4
  7  8  9       bottom row <-
                left col up       then inner layer: 5
  order: 1 2 3 6 9 8 7 4 5

LADDER (easy -> hard inside this file):
  1. Row sums and column sums        (double loop warm-up)
  2. Transpose in place              (swap across diagonal)
  3. Rotate 90 degrees clockwise     (transpose + reverse rows)
  4. Spiral traversal                (4 shrinking boundaries)
  5. Search in row+column sorted matrix (start at top-right corner)
  6. Set zeroes                      (propagate zero rows/columns)

BEGINNER EXPLANATION:
A 2D matrix is an array of arrays: m[i][j] - i is the row, j is
the column. Every classic matrix problem is a different walk over
the same data:
- Transpose: m[i][j] swaps with m[j][i] (mirror on main diagonal).
- Rotate 90 CW: transpose, then reverse each row - two easy moves
  replace one confusing index formula.
- Spiral: keep 4 boundaries (top, bottom, left, right) and shrink
  them as you print rings.
- Sorted-matrix search: start at the TOP-RIGHT corner - moving
  LEFT decreases the value, moving DOWN increases it. One step per
  comparison, O(m + n).

SUPER SIMPLE EXAMPLE (rotate 3x3):
1 2 3      transpose      1 4 7      reverse rows     7 4 1
4 5 6   ------------->    2 5 8   --------------->    8 5 2
7 8 9                     3 6 9                      9 6 3

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Transpose in place requires a SQUARE matrix; for m x n you build
  the n x m result - the program handles both.
- Spiral on a single row / column: the "if top < bottom" and
  "if left < right" guards prevent printing the middle row/col
  twice - remove them and watch duplicates appear.
- Rotate a 1xN row: becomes an Nx1 column (result dims swap).
- Search matrix rows sorted left-to-right AND columns top-to-
  bottom: works only for THAT shape; for fully row-sorted matrices
  use binary search per row instead.
- Set zeroes with all zeros: the entire matrix becomes zero - the
  row/col marker arrays handle it without re-reading zeros as new
  markers (classic infinite-loop bug in the naive approach).

DRY RUN (spiral 3x3):
top row: 1 2 3 (top++)  right col: 6 9 (right--)
bottom row: 8 7 (bottom--) left col: 4 (left++)
inner: 5. Output: 1 2 3 6 9 8 7 4 5.

TIPS:
- "Transpose + reverse" beats index gymnastics for rotation -
  compose simple passes, avoid error-prone four-way swaps.
- Spiral's four boundaries is the template for "print boundary",
  "rotate layers", and image rotate puzzles.
- Top-right corner search returns in sorted-matrix interview
  variants (search a 2D matrix II) - O(m+n) beats O(m log n).
*/

#include <stdio.h>

#define MAX_RC 20

void printMatrix(int m[MAX_RC][MAX_RC], int r, int c, const char* title) {
    printf("%s\n", title);
    for (int i = 0; i < r; i++) {
        printf("  |");
        for (int j = 0; j < c; j++) {
            printf(" %4d", m[i][j]);
        }
        printf(" |\n");
    }
}

int main() {
    int a[MAX_RC][MAX_RC];
    int r;
    int c;
    int choice;

    printf("2D Matrix Problems Ladder (easy -> hard)\n\n");

    printf("Rows (1-%d)? ", MAX_RC);
    scanf("%d", &r);
    printf("Cols (1-%d)? ", MAX_RC);
    scanf("%d", &c);

    if (r < 1 || r > MAX_RC || c < 1 || c > MAX_RC) {
        printf("Invalid size\n");
        return 1;
    }

    printf("Enter %d values per row:\n", c);
    for (int i = 0; i < r; i++) {
        printf("  Row %d: ", i + 1);
        for (int j = 0; j < c; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    while (1) {
        printf("\n1. Row sums and column sums\n");
        printf("2. Transpose (in place if square)\n");
        printf("3. Rotate 90 degrees clockwise\n");
        printf("4. Spiral traversal\n");
        printf("5. Search in row+column sorted matrix\n");
        printf("6. Set zeroes\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Row sums: ");
            for (int i = 0; i < r; i++) {
                int s = 0;
                for (int j = 0; j < c; j++) {
                    s = s + a[i][j];
                }
                printf("%d ", s);
            }
            printf("\nColumn sums: ");
            for (int j = 0; j < c; j++) {
                int s = 0;
                for (int i = 0; i < r; i++) {
                    s = s + a[i][j];
                }
                printf("%d ", s);
            }
            printf("\n");
        } else if (choice == 2) {
            if (r == c) {
                for (int i = 0; i < r; i++) {
                    for (int j = i + 1; j < c; j++) {
                        int t = a[i][j];
                        a[i][j] = a[j][i];
                        a[j][i] = t;
                    }
                }
                printMatrix(a, r, c, "Transposed (in place):");
            } else {
                int t[MAX_RC][MAX_RC];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        t[j][i] = a[i][j];
                    }
                }
                printMatrix(t, c, r, "Transposed (m x n -> n x m):");
            }
        } else if (choice == 3) {
            int res[MAX_RC][MAX_RC];
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    res[j][r - 1 - i] = a[i][j];
                }
            }
            printMatrix(res, c, r, "Rotated 90 CW:");
        } else if (choice == 4) {
            printf("Spiral: ");
            int top = 0;
            int bottom = r - 1;
            int left = 0;
            int right = c - 1;
            while (top <= bottom && left <= right) {
                for (int j = left; j <= right; j++) {
                    printf("%d ", a[top][j]);
                }
                top = top + 1;
                for (int i = top; i <= bottom; i++) {
                    printf("%d ", a[i][right]);
                }
                right = right - 1;
                if (top <= bottom) {
                    for (int j = right; j >= left; j--) {
                        printf("%d ", a[bottom][j]);
                    }
                    bottom = bottom - 1;
                }
                if (left <= right) {
                    for (int i = bottom; i >= top; i--) {
                        printf("%d ", a[i][left]);
                    }
                    left = left + 1;
                }
            }
            printf("\n");
        } else if (choice == 5) {
            int target;
            printf("This needs rows sorted L->R and cols sorted T->B.\n");
            printf("Target: ");
            scanf("%d", &target);
            int i = 0;
            int j = c - 1;
            int found = -1;
            while (i < r && j >= 0) {
                if (a[i][j] == target) {
                    found = 1;
                    printf("Found %d at (%d, %d)\n", target, i, j);
                    break;
                }
                if (a[i][j] > target) {
                    j = j - 1;
                } else {
                    i = i + 1;
                }
            }
            if (!found) {
                printf("%d not found\n", target);
            }
        } else if (choice == 6) {
            int zeroRow[MAX_RC] = {0};
            int zeroCol[MAX_RC] = {0};
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (a[i][j] == 0) {
                        zeroRow[i] = 1;
                        zeroCol[j] = 1;
                    }
                }
            }
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (zeroRow[i] || zeroCol[j]) {
                        a[i][j] = 0;
                    }
                }
            }
            printMatrix(a, r, c, "After set-zeroes:");
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
