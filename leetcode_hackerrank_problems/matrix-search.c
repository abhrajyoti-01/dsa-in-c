/*
Matrix Search - Staircase Search and Sorted-Matrix Kth (Intermediate -> Advanced)
---------------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 74  - Search a 2D Matrix (fully sorted rows and columns)
  LeetCode 240 - Search a 2D Matrix II (staircase search, the star trick)
  LeetCode 378 - Kth Smallest Element in a Sorted Matrix
  HackerRank "Matrix" drill: count elements below a threshold

PROBLEM DEFINITION:
A matrix whose rows and columns are each sorted in non-decreasing
order hides an O(rows + cols) search: start at the top-right corner
and walk. The same monotone count("how many values <= x") answers the
kth-smallest query by binary search on the VALUE.

REAL-LIFE EXAMPLE:
A price/quantity table: rows sorted by product tier and columns by
region, so both directions increase. "Which tier does this budget
reach?" is a staircase walk; "what is the median price?" is a
value-space binary search.

DIAGRAM (staircase search for 5):
   1  4  7 11        start top-right (11 > 5) -> move LEFT
  2  5  8 12        now (7 > 5) -> move LEFT
  3  6  9 16        now (4 < 5) -> move DOWN
  10 13 14 17       now (5 == 5) -> FOUND at row 1, col 1
  Every step deletes one row or one column: at most r + c steps.

PSEUDO CODE:
  Staircase(a, target):
      r = 0; c = cols - 1
      while r < rows and c >= 0:
          if a[r][c] == target: return (r, c)
          if a[r][c] > target:  c--      // column too large
          else:                 r++      // row too small
      return not found
  CountLE(x) = sum over rows of (number of entries <= x)   // O(rows log cols)
  KthSmallest(k): binary search x in [min, max] for the smallest x
                  whose CountLE(x) >= k

SAMPLE INPUT & DRY RUN:
  4 4
   1  4  7 11
   2  5  8 12
   3  6  9 16
  10 13 14 17
  2                       <- how many search targets
  5                       <- target 1
  7                       <- target 2
  6                       <- rank k for the kth-smallest query
  Staircase for 5 starts at the top-right (0,3) = 11:
  11 > 5 -> go left, 7 > 5 -> go left, 4 < 5 -> go down, 5 == 5 found.
  So 5 is found at (1,1) after 4 steps.
  7 is found at (0,2) after 2 steps.
  The full sorted order is 1 2 3 4 5 6 7 8 9 10 11 12 13 14 16 17, so
  the 6th smallest is 6 (count of values <= 6 is 6).

SPECIAL CASES:
  - Only the ROW-wise and COLUMN-wise sorted property is required;
    the whole matrix need not be sorted row-major. The program
    validates this weaker property instead of rejecting valid inputs.
  - Equal values in many cells: the staircase still terminates, and
    CountLE uses upper_bound so duplicates are counted consistently.
  - k out of [1, rows*cols] is rejected.
  - kth = smallest x with CountLE(x) >= k (the "first true" boundary).
  - rows, cols <= 200, values within +/-10^9, k <= 40000.

COMPLEXITY:
  Staircase search O(rows + cols); kth-smallest O((rows+cols) * log
  (value range)) = about 31 staircase-equivalent counts; O(rows*cols)
  storage for the matrix itself.

TIPS:
  The top-right (or bottom-left) corner is the ONLY position where one
  neighbour is smaller and the other larger - that is why the walk
  works. Top-left cannot move: both neighbours are larger. Say this
  out loud in interviews; it shows the invariant.
*/

#include <stdio.h>

#define MAX_DIM 200
#define VALUE_LIMIT 1000000000LL

long long matrix[MAX_DIM][MAX_DIM];

int searchStaircase(int rows, int cols, long long target, int* foundRow,
                    int* foundCol, int* steps) {
    int r = 0;
    int c = cols - 1;
    int taken = 0;
    while (r < rows && c >= 0) {
        taken++;
        if (matrix[r][c] == target) {
            *foundRow = r;
            *foundCol = c;
            *steps = taken;
            return 1;
        }
        if (matrix[r][c] > target) {
            c--;
        } else {
            r++;
        }
    }
    *steps = taken;
    return 0;
}

long long countLessEqual(int rows, int cols, long long value) {
    long long total = 0;
    for (int r = 0; r < rows; r++) {
        int lo = 0;
        int hi = cols;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (matrix[r][mid] <= value) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        total += lo;
    }
    return total;
}

int main(void) {
    int rows;
    int cols;

    printf("Matrix Search - Staircase Walk and Kth Smallest\n\n");
    printf("How many rows and columns (1-%d)? ", MAX_DIM);
    if (scanf("%d %d", &rows, &cols) != 2 ||
        rows < 1 || rows > MAX_DIM || cols < 1 || cols > MAX_DIM) {
        printf("Invalid dimensions\n");
        return 1;
    }
    printf("Enter %d rows of %d sorted integers (within +/-10^9):\n",
           rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (scanf("%lld", &matrix[r][c]) != 1 ||
                matrix[r][c] < -VALUE_LIMIT || matrix[r][c] > VALUE_LIMIT) {
                printf("Invalid value\n");
                return 1;
            }
            if (c > 0 && matrix[r][c] < matrix[r][c - 1]) {
                printf("Invalid row %d: not sorted left to right\n", r);
                return 1;
            }
            if (r > 0 && matrix[r][c] < matrix[r - 1][c]) {
                printf("Invalid column %d: not sorted top to bottom\n", c);
                return 1;
            }
        }
    }

    long long target;
    printf("How many search targets (0-%d)? ", MAX_DIM);
    int tests;
    if (scanf("%d", &tests) != 1 || tests < 0 || tests > MAX_DIM) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < tests; i++) {
        printf("Target %d (within +/-10^9): ", i + 1);
        if (scanf("%lld", &target) != 1 ||
            target < -VALUE_LIMIT || target > VALUE_LIMIT) {
            printf("Invalid target\n");
            return 1;
        }
        int foundRow = -1;
        int foundCol = -1;
        int steps = 0;
        if (searchStaircase(rows, cols, target, &foundRow, &foundCol, &steps)) {
            printf("Found %lld at row %d, column %d (%d steps)\n", target,
                   foundRow, foundCol, steps);
        } else {
            printf("%lld not found (%d steps)\n", target, steps);
        }
        printf("   count <= %lld is %lld\n", target,
               countLessEqual(rows, cols, target));
    }

    long long low = matrix[0][0];
    long long high = matrix[0][0];
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (matrix[r][c] < low) low = matrix[r][c];
            if (matrix[r][c] > high) high = matrix[r][c];
        }
    }
    long long k;
    printf("\nWhich rank k to find (1..%d)? ", rows * cols);
    if (scanf("%lld", &k) != 1 || k < 1 || k > (long long)rows * cols) {
        printf("Invalid rank\n");
        return 1;
    }
    while (low < high) {
        long long mid = low + (high - low) / 2;
        if (countLessEqual(rows, cols, mid) >= k) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    printf("%lld-th smallest element = %lld (count <= it is %lld)\n", k, low,
           countLessEqual(rows, cols, low));
    return 0;
}
