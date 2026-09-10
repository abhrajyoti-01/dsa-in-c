/*
2D Prefix Sums - Submatrix Sums in O(1) (Basic -> Intermediate)
--------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 304  - Range Sum Query 2D - Immutable
  LeetCode 1314 - Matrix Block Sum (same inclusion-exclusion)
  HackerRank "2D Array - DS" - maximum hourglass sum

PROBLEM DEFINITION:
Preprocess a static matrix so any submatrix sum returns in constant
time. Scanning the rectangle costs O(rows*cols) per query; a 2D prefix
table answers every query in O(1).

REAL-LIFE EXAMPLE:
Heat maps and satellite tiles: "how much rainfall between these two
map corners?" The image never changes, but the requested rectangle
changes with every click.

DIAGRAM:
  grid:                prefix table P with an extra zero row/column:
   1  2  3               0   0   0   0
   4  5  6               0   1   3   6
   7  8  9               0   5  12  21
                         0  12  27  45
  sum of rows 1..2, cols 1..2:
    P[3][3] - P[1][3] - P[3][1] + P[1][1] = 45 - 6 - 12 + 1 = 28
    check: 5 + 6 + 8 + 9 = 28

  hourglass (HackerRank): 3x3 window minus the two middle side cells
    a b c
      d          = sum(3x3 block) - (b + h)
    e f g

PSEUDO CODE:
  P[r+1][c+1] = grid[r][c] + P[r][c+1] + P[r+1][c] - P[r][c]
  Submatrix(r1,c1,r2,c2)
      = P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1]

SAMPLE INPUT & DRY RUN:
  3 3
  1 2 3
  4 5 6
  7 8 9
  2
  1 1 2 2
  0 0 0 0
  Best hourglass uses rows 0..2, cols 0..2: 1+2+3+5+7+8+9 = 35.
  Query [1..2]x[1..2] = 28; query [0..0]x[0..0] = 1.

SPECIAL CASES:
  - The extra row/column of zeros removes every boundary check.
  - Negative entries are fine: inclusion-exclusion is sign-agnostic.
  - One-cell queries return the element itself.
  - Hourglasses need a 3x3 window; smaller grids report "not applicable".
  - rows, cols <= 200, values within +/-10^9, queries <= 1000.
    Worst case 4*10^4 cells * 10^9 = 4*10^13 stays inside long long.

COMPLEXITY:
  Build O(rows*cols) time and space; each submatrix sum O(1).

TIPS:
  Subtraction order matters: one sign error flips a whole region.
  A 1D difference array handles batch UPDATES; a 2D prefix sum handles
  batch QUERIES. Pick by which operation repeats.
*/

#include <stdio.h>

#define MAX_DIM 200
#define MAX_QUERIES 1000
#define VALUE_LIMIT 1000000000LL

long long grid[MAX_DIM][MAX_DIM];
long long prefix[MAX_DIM + 1][MAX_DIM + 1];

long long submatrixSum(int r1, int c1, int r2, int c2) {
    return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] -
           prefix[r2 + 1][c1] + prefix[r1][c1];
}

int main(void) {
    int rows;
    int cols;
    int queries;
    long long bestHourglass = 0;
    int foundHourglass = 0;

    printf("2D Prefix Sums - Submatrix Sum Queries\n\n");
    printf("How many rows and columns (1-%d)? ", MAX_DIM);
    if (scanf("%d %d", &rows, &cols) != 2 ||
        rows < 1 || rows > MAX_DIM || cols < 1 || cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    printf("Enter %d rows of %d integers (within +/-10^9):\n", rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            long long value;
            if (scanf("%lld", &value) != 1 ||
                value < -VALUE_LIMIT || value > VALUE_LIMIT) {
                printf("Invalid value\n");
                return 1;
            }
            grid[r][c] = value;
            prefix[r + 1][c + 1] = value + prefix[r][c + 1] +
                                   prefix[r + 1][c] - prefix[r][c];
        }
    }

    for (int r = 0; r + 2 < rows; r++) {
        for (int c = 0; c + 2 < cols; c++) {
            long long hourglass = submatrixSum(r, c, r + 2, c + 2) -
                                  grid[r + 1][c] - grid[r + 1][c + 2];
            if (!foundHourglass || hourglass > bestHourglass) {
                bestHourglass = hourglass;
                foundHourglass = 1;
            }
        }
    }
    printf("\nMaximum hourglass sum: ");
    if (foundHourglass) {
        printf("%lld\n", bestHourglass);
    } else {
        printf("not applicable (grid smaller than 3x3)\n");
    }
    printf("Total matrix sum: %lld\n", submatrixSum(0, 0, rows - 1, cols - 1));

    printf("How many queries (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &queries) != 1 || queries < 0 || queries > MAX_QUERIES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < queries; i++) {
        int r1;
        int c1;
        int r2;
        int c2;
        printf("Query %d: r1 c1 r2 c2 (zero-based, inclusive): ", i + 1);
        if (scanf("%d %d %d %d", &r1, &c1, &r2, &c2) != 4 ||
            r1 < 0 || r2 >= rows || c1 < 0 || c2 >= cols ||
            r1 > r2 || c1 > c2) {
            printf("Invalid query\n");
            return 1;
        }
        printf("sum(%d,%d,%d,%d) = %lld\n", r1, c1, r2, c2,
               submatrixSum(r1, c1, r2, c2));
    }
    return 0;
}
