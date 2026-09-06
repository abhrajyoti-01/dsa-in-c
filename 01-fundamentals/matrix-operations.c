/*
Matrix Operations - Add, Subtract, Multiply, Transpose, Determinant, Inverse
----------------------------------------------------------------------------
What this program solves:
Menu-driven program that reads matrices from the user and performs:
addition, subtraction, multiplication, transpose, determinant
(recursion + cofactor expansion), scalar multiplication, and
inverse. "Division" of matrices is shown as A / B = A x B_inverse.

Real life example:
A shop sells three products in two cities. A sales matrix holds
units sold, a price matrix holds unit prices - multiplying them
gives total revenue per city. Image processing rotates images with
transposes; engineering solves linear systems with inverses;
graphics engines multiply matrices millions of times per frame.

DIAGRAM (addition is element-wise; transpose flips rows/columns):
  A   +  B   =  A+B          transpose(A):
  1 2    5 6     6 8         1 2 3        1 4
  3 4    7 8    10 12        4 5 6   ->   2 5
                                          3 6
  row i of A becomes column i of A^T.

How the process works:
1. ADD / SUBTRACT: C[i][j] = A[i][j] +/- B[i][j].
   Requires same rows and columns - done element by element.
2. MULTIPLY: C[i][j] = sum over k of A[i][k] * B[k][j].
   Requires columns(A) == rows(B). O(n^3) with triple loop.
3. TRANSPOSE: T[i][j] = A[j][i] - rows become columns.
4. DETERMINANT: recursive cofactor expansion along row 0.
   det(A) = sum of (-1)^col * A[0][col] * det(minor).
   Base cases: 1x1 -> element, 2x2 -> ad - bc.
5. INVERSE: exists only if det != 0.
   Inverse = adjugate / determinant; adjugate = transpose of the
   cofactor matrix. For 1x1: inverse = 1/element.
6. DIVISION: A / B = A x B_inverse (B must be square, det != 0).

Pseudo code:
START
READ rowsA, colsA, matrix A
READ rowsB, colsB, matrix B
ADD/SUBTRACT: FOR each cell: C[i][j] = A[i][j] +/- B[i][j]
MULTIPLY:     FOR i,j,k: C[i][j] += A[i][k] * B[k][j]
DETERMINANT:  IF n == 1 -> A[0][0]; n == 2 -> ad - bc;
              ELSE cofactor expansion along row 0
INVERSE:      adj(A) / det(A)
DIVISION:     A x inverse(B)
END

Sample input and dry run:
A = | 2 1 |     B = | 3 0 |
    | 1 4 |         | 5 2 |
A + B = | 5 1 |    A - B = | -1 1 |
        | 6 6 |            | -4 2 |
A x B = | 11 2 |   det(A) = 2*4 - 1*1 = 7
        | 23 8 |
inverse(B): det = 6, adj = | 2 -0 |, inverse = | 0.33 0.00 |
                           | -5  3 |            | -0.83 0.50 |

Main logic to understand:
Addition/subtraction are element-wise O(rows x cols); multiplication
contracts rows against columns O(n^3); the determinant recursively
shrinks the matrix through minors; the inverse divides the adjugate
by the determinant; and matrix "division" is really multiplication
by the inverse.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 10

/*
 * readMatrix - Reads an r x c matrix from the user.
 */
void readMatrix(int m[MAX_DIM][MAX_DIM], int r, int c, const char* name) {
    printf("Enter %d rows x %d cols for matrix %s:\n", r, c, name);
    for (int i = 0; i < r; i++) {
        printf("  Row %d (%d values separated by space): ", i + 1, c);
        for (int j = 0; j < c; j++) {
            scanf("%d", &m[i][j]);
        }
    }
}

/*
 * printMatrixInt - Displays an integer matrix.
 */
void printMatrixInt(int m[MAX_DIM][MAX_DIM], int r, int c, const char* title) {
    printf("%s\n", title);
    for (int i = 0; i < r; i++) {
        printf("  |");
        for (int j = 0; j < c; j++) {
            printf(" %6d", m[i][j]);
        }
        printf(" |\n");
    }
}

/*
 * printMatrixFloat - Displays a floating-point matrix.
 */
void printMatrixFloat(double m[MAX_DIM][MAX_DIM], int r, int c, const char* title) {
    printf("%s\n", title);
    for (int i = 0; i < r; i++) {
        printf("  |");
        for (int j = 0; j < c; j++) {
            printf(" %8.3lf", m[i][j]);
        }
        printf(" |\n");
    }
}

/*
 * addMatrices - C = A + B (element-wise).
 */
void addMatrices(int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM],
                 int out[MAX_DIM][MAX_DIM], int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            out[i][j] = a[i][j] + b[i][j];
        }
    }
}

/*
 * subtractMatrices - C = A - B (element-wise).
 */
void subtractMatrices(int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM],
                      int out[MAX_DIM][MAX_DIM], int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            out[i][j] = a[i][j] - b[i][j];
        }
    }
}

/*
 * multiplyMatrices - C = A x B using the triple loop.
 */
void multiplyMatrices(int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM],
                      int out[MAX_DIM][MAX_DIM], int ra, int ca, int cb) {
    for (int i = 0; i < ra; i++) {
        for (int j = 0; j < cb; j++) {
            out[i][j] = 0;
            for (int k = 0; k < ca; k++) {
                out[i][j] = out[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

/*
 * transposeMatrix - T[i][j] = A[j][i].
 */
void transposeMatrix(int a[MAX_DIM][MAX_DIM], int out[MAX_DIM][MAX_DIM],
                     int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            out[j][i] = a[i][j];
        }
    }
}

/*
 * scalarMultiply - out = s * A.
 */
void scalarMultiply(int a[MAX_DIM][MAX_DIM], int out[MAX_DIM][MAX_DIM],
                    int s, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            out[i][j] = s * a[i][j];
        }
    }
}

/*
 * determinant - Recursive cofactor expansion along row 0.
 */
double determinant(int m[MAX_DIM][MAX_DIM], int n) {
    if (n == 1) {
        return (double)m[0][0];
    }
    if (n == 2) {
        return (double)m[0][0] * m[1][1] - (double)m[0][1] * m[1][0];
    }

    double det = 0;
    int minor[MAX_DIM][MAX_DIM];

    for (int col = 0; col < n; col++) {
        int mi = 0;
        for (int i = 1; i < n; i++) {
            int mj = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) {
                    continue;
                }
                minor[mi][mj] = m[i][j];
                mj = mj + 1;
            }
            mi = mi + 1;
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det = det + sign * m[0][col] * determinant(minor, n - 1);
    }
    return det;
}

/*
 * cofactorMatrix - Builds the matrix of cofactors of an n x n matrix.
 */
void cofactorMatrix(int m[MAX_DIM][MAX_DIM], double out[MAX_DIM][MAX_DIM], int n) {
    int minor[MAX_DIM][MAX_DIM];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int mi = 0;
            for (int r = 0; r < n; r++) {
                if (r == i) {
                    continue;
                }
                int mj = 0;
                for (int c = 0; c < n; c++) {
                    if (c == j) {
                        continue;
                    }
                    minor[mi][mj] = m[r][c];
                    mj = mj + 1;
                }
                mi = mi + 1;
            }
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            out[i][j] = sign * determinant(minor, n - 1);
        }
    }
}

/*
 * inverseMatrix - Computes inverse = adjugate / determinant.
 * Returns 1 on success, 0 when the matrix is singular.
 */
int inverseMatrix(int m[MAX_DIM][MAX_DIM], double out[MAX_DIM][MAX_DIM], int n) {
    double det = determinant(m, n);
    if (det == 0) {
        return 0;
    }

    if (n == 1) {
        out[0][0] = 1.0 / (double)m[0][0];
        return 1;
    }

    double cof[MAX_DIM][MAX_DIM];
    cofactorMatrix(m, cof, n);

    /* adjugate = transpose of cofactor matrix; inverse = adj / det */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            out[i][j] = cof[j][i] / det;
        }
    }
    return 1;
}

/*
 * readDoubleMatrix - Reads an r x c double matrix from the user.
 */
void readDoubleMatrix(double m[MAX_DIM][MAX_DIM], int r, int c, const char* name) {
    printf("Enter %d rows x %d cols for matrix %s:\n", r, c, name);
    for (int i = 0; i < r; i++) {
        printf("  Row %d (%d values separated by space): ", i + 1, c);
        for (int j = 0; j < c; j++) {
            scanf("%lf", &m[i][j]);
        }
    }
}

int main() {
    int a[MAX_DIM][MAX_DIM];
    int b[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    int ra = 0;
    int ca = 0;
    int rb = 0;
    int cb = 0;
    int haveA = 0;
    int haveB = 0;
    int choice;

    while (1) {
        printf("\nMatrix Operations\n");
        printf("1. Read matrix A\n");
        printf("2. Read matrix B\n");
        printf("3. Addition (A + B)\n");
        printf("4. Subtraction (A - B)\n");
        printf("5. Multiplication (A x B)\n");
        printf("6. Transpose of A\n");
        printf("7. Determinant of A (square)\n");
        printf("8. Scalar multiply (k x A)\n");
        printf("9. Inverse of A (square)\n");
        printf("10. Division (A / B = A x B_inverse)\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Rows of A: ");
            scanf("%d", &ra);
            printf("Cols of A: ");
            scanf("%d", &ca);
            if (ra < 1 || ra > MAX_DIM || ca < 1 || ca > MAX_DIM) {
                printf("Invalid dimensions (1-%d)\n", MAX_DIM);
                ra = 0;
                ca = 0;
                continue;
            }
            readMatrix(a, ra, ca, "A");
            haveA = 1;
        } else if (choice == 2) {
            printf("Rows of B: ");
            scanf("%d", &rb);
            printf("Cols of B: ");
            scanf("%d", &cb);
            if (rb < 1 || rb > MAX_DIM || cb < 1 || cb > MAX_DIM) {
                printf("Invalid dimensions (1-%d)\n", MAX_DIM);
                rb = 0;
                cb = 0;
                continue;
            }
            readMatrix(b, rb, cb, "B");
            haveB = 1;
        } else if (choice == 3) {
            if (!haveA || !haveB) {
                printf("Read both A and B first\n");
            } else if (ra != rb || ca != cb) {
                printf("Addition needs same dimensions (A is %dx%d, B is %dx%d)\n",
                       ra, ca, rb, cb);
            } else {
                addMatrices(a, b, result, ra, ca);
                printMatrixInt(result, ra, ca, "A + B:");
            }
        } else if (choice == 4) {
            if (!haveA || !haveB) {
                printf("Read both A and B first\n");
            } else if (ra != rb || ca != cb) {
                printf("Subtraction needs same dimensions (A is %dx%d, B is %dx%d)\n",
                       ra, ca, rb, cb);
            } else {
                subtractMatrices(a, b, result, ra, ca);
                printMatrixInt(result, ra, ca, "A - B:");
            }
        } else if (choice == 5) {
            if (!haveA || !haveB) {
                printf("Read both A and B first\n");
            } else if (ca != rb) {
                printf("Multiplication needs cols(A) == rows(B) (%d vs %d)\n",
                       ca, rb);
            } else {
                multiplyMatrices(a, b, result, ra, ca, cb);
                printMatrixInt(result, ra, cb, "A x B:");
            }
        } else if (choice == 6) {
            if (!haveA) {
                printf("Read A first\n");
            } else {
                int t[MAX_DIM][MAX_DIM];
                transposeMatrix(a, t, ra, ca);
                printMatrixInt(t, ca, ra, "Transpose of A:");
            }
        } else if (choice == 7) {
            if (!haveA) {
                printf("Read A first\n");
            } else if (ra != ca) {
                printf("Determinant needs a SQUARE matrix (A is %dx%d)\n", ra, ca);
            } else {
                printf("Determinant of A = %.0lf\n", determinant(a, ra));
            }
        } else if (choice == 8) {
            if (!haveA) {
                printf("Read A first\n");
            } else {
                int k;
                printf("Enter scalar k: ");
                scanf("%d", &k);
                scalarMultiply(a, result, k, ra, ca);
                printMatrixInt(result, ra, ca, "k x A:");
            }
        } else if (choice == 9) {
            if (!haveA) {
                printf("Read A first\n");
            } else if (ra != ca) {
                printf("Inverse needs a SQUARE matrix (A is %dx%d)\n", ra, ca);
            } else {
                double inv[MAX_DIM][MAX_DIM];
                if (inverseMatrix(a, inv, ra)) {
                    printMatrixFloat(inv, ra, ra, "Inverse of A:");
                } else {
                    printf("A is singular (determinant 0) - inverse does not exist\n");
                }
            }
        } else if (choice == 10) {
            if (!haveA || !haveB) {
                printf("Read both A and B first\n");
            } else if (rb != cb) {
                printf("B must be SQUARE for division (B is %dx%d)\n", rb, cb);
            } else if (ca != rb) {
                printf("A x B_inverse needs cols(A) == rows(B) (%d vs %d)\n",
                       ca, rb);
            } else {
                double binv[MAX_DIM][MAX_DIM];
                if (!inverseMatrix(b, binv, rb)) {
                    printf("B is singular (determinant 0) - cannot divide\n");
                } else {
                    double quotient[MAX_DIM][MAX_DIM];
                    for (int i = 0; i < ra; i++) {
                        for (int j = 0; j < rb; j++) {
                            double sum = 0;
                            for (int k = 0; k < ca; k++) {
                                sum = sum + (double)a[i][k] * binv[k][j];
                            }
                            quotient[i][j] = sum;
                        }
                    }
                    printMatrixFloat(quotient, ra, rb, "A / B (= A x B_inverse):");
                }
            }
        } else if (choice == 11) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
