/*
Pattern Printing Ladder (Basic -> Hard)
---------------------------------------
Real life example:
Receipt/label printers and LED-matrix renderers build every shape
row by row: for each row compute how many blanks and how many
marked cells - the same per-row formula hunting used here.

DIAGRAM (pyramid rows, n = 4 - find the formula per row):
  row 1:    *       spaces = n-1 = 3, stars = 2*1-1 = 1
  row 2:   ***      spaces = 2,    stars = 3
  row 3:  *****     spaces = 1,    stars = 5
  row 4: *******    spaces = 0,    stars = 7

LADDER (easy -> hard inside this file):
  1. Right triangle of stars      (rows == stars)
  2. Inverted right triangle      (counting down)
  3. Centered pyramid             (spaces + stars)
  4. Diamond                      (up pyramid + down pyramid)
  5. Hollow square                (border only)
  6. Number triangle              (1 / 12 / 123 ...)
  7. Pascal's triangle            (binomial coefficients)

BEGINNER EXPLANATION:
Every pattern = two nested loops: outer = rows, inner = what to
print in that row. The inner loop's COUNT is a FORMULA of the row
number: stars = i, spaces = n - i, Pascal entry = C(row, col).
Spot the formula and any pattern becomes easy.

SUPER SIMPLE EXAMPLE (pyramid, n = 3):
Row 1: 2 spaces + 1 star  -> "  *"
Row 2: 1 space  + 3 stars -> " ***"
Row 3: 0 spaces + 5 stars -> "*****"
Spaces = n - row, stars = 2*row - 1. Two formulas - done.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- n = 0 or 1: degenerate patterns (nothing / single star) - the
  loops handle them naturally, no special code needed.
- Hollow shapes: print star only when on the border (row == 1,
  row == n, col == 1, col == n) else space.
- Pascal triangle edges: C(n, 0) = C(n, n) = 1 always - the DP
  builds interior from the two cells above.
- Centering assumes monospace font - proportional fonts break the
  alignment visually (not logically).

DRY RUN (number triangle n = 3):
Row 1: print 1          (inner loop 1..1)
Row 2: print 1 2        (inner 1..2)
Row 3: print 1 2 3      (inner 1..3)
Pascal n = 4:
Row 0: 1
Row 1: 1 1
Row 2: 1 2 1          (each interior = sum of two above)
Row 3: 1 3 3 1

TIPS:
- Pascal's triangle IS the DP table for binomial coefficients -
  it returns in folder 11 as the basis of combinatorics DP.
- The formulas stars = 2*row-1, spaces = n-row appear in real UI
  code (progress bars, ASCII art) - not just labs.
- Pattern 7 (Pascal) is the exact same table used by catalan-numbers and
  knapsack counting - see 19-bitmask-dp-game-theory/catalan-numbers.c.
*/

#include <stdio.h>

void rightTriangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

void invertedTriangle(int n) {
    for (int i = n; i >= 1; i--) {
        for (int j = 0; j < i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

void pyramid(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < n - i; j++) {
            printf(" ");
        }
        for (int j = 0; j < 2 * i - 1; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void diamond(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < n - i; j++) {
            printf(" ");
        }
        for (int j = 0; j < 2 * i - 1; j++) {
            printf("*");
        }
        printf("\n");
    }
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 0; j < n - i; j++) {
            printf(" ");
        }
        for (int j = 0; j < 2 * i - 1; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void hollowSquare(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == 1 || i == n || j == 1 || j == n) {
                printf("* ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void numberTriangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("%d ", j);
        }
        printf("\n");
    }
}

void pascalTriangle(int n) {
    int row[50];
    row[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int s = 0; s < n - i - 1; s++) {
            printf("  ");
        }
        for (int j = i; j >= 1; j--) {
            row[j] = row[j] + row[j - 1];
        }
        for (int j = 0; j <= i; j++) {
            printf("%4d", row[j]);
        }
        printf("\n");
    }
}

int main() {
    int n;
    int choice;

    printf("Pattern Printing Ladder (easy -> hard)\n\n");

    printf("Enter size n (1-20): ");
    scanf("%d", &n);

    if (n < 1 || n > 20) {
        printf("Invalid size\n");
        return 1;
    }

    while (1) {
        printf("\n1. Right triangle\n");
        printf("2. Inverted triangle\n");
        printf("3. Pyramid\n");
        printf("4. Diamond\n");
        printf("5. Hollow square\n");
        printf("6. Number triangle\n");
        printf("7. Pascal's triangle\n");
        printf("8. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            rightTriangle(n);
        } else if (choice == 2) {
            invertedTriangle(n);
        } else if (choice == 3) {
            pyramid(n);
        } else if (choice == 4) {
            diamond(n);
        } else if (choice == 5) {
            hollowSquare(n);
        } else if (choice == 6) {
            numberTriangle(n);
        } else if (choice == 7) {
            pascalTriangle(n);
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
