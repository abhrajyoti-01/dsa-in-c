/*
 PROBLEM DEFINITION:
 --------------------
 Standard matrix multiplication of two N x N matrices needs 8 recursive
 multiplications per block, giving O(N^3). Strassen's Divide & Conquer
 algorithm reduces the recursive multiplications from 8 to 7 by using
 clever additions/subtractions, yielding O(N^2.807) time.

 The trick: instead of computing C = A*B block-wise with 8 products,
 compute 7 special products (M1..M7) of sums/differences of blocks,
 then combine them with additions to get the four result blocks.

 REAL-LIFE EXAMPLE - Rendering & Scientific Simulation:
 -------------------------------------------------------
 Computer graphics pipelines multiply huge matrices for 3D transforms,
 and physics simulations (weather prediction, fluid dynamics) multiply
 matrices of size thousands. Shaving the exponent from 3.0 to 2.807
 means a 1024x1024 multiply drops from ~1 billion to ~268 million
 operations - hours saved on supercomputers. Strassen's is used in
 optimized libraries (like BLAS variants) for large matrices, and its
 "do fewer big operations by trading for cheap small ones" idea appears
 everywhere from FFT to fast integer multiplication.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Divide each matrix into four N/2 x N/2 quadrants:
     A = | A11 A12 |   B = | B11 B12 |   C = | C11 C12 |
         | A21 A22 |       | B21 B22 |       | C21 C22 |

 1. BASE CASE:
    - If N == 1: return the single product A*B.

 2. DIVIDE:
    - Split A and B into their four quadrants.

 3. COMPUTE THE 7 STRASSEN PRODUCTS (each is a smaller multiply):
    M1 = (A11 + A22) * (B11 + B22)
    M2 = (A21 + A22) * B11
    M3 = A11 * (B12 - B22)
    M4 = A22 * (B21 - B11)
    M5 = (A11 + A12) * B22
    M6 = (A21 - A22) * (B11 + B12)
    M7 = (A11 - A22) * (B21 + B22)

 4. COMBINE (only additions/subtractions):
    C11 = M1 + M4 - M5 + M7
    C12 = M3 + M5
    C21 = M2 + M4
    C22 = M1 - M2 + M3 + M6

 5. CONQUER:
    - Recursively compute each M (7 multiplications of size N/2).
    - 7 instead of 8 is the whole win: 7 multiplies of half size beats
      8 multiplies of half size asymptotically.

 WHY 7 < 8 IS MAGIC:
    Standard: 8 * (N/2)^3 = N^3 operations.
    Strassen: 7 * (N/2)^3 + O(N^2) = (7/8) * N^3 + O(N^2).
    Recurrence T(N) = 7T(N/2) + O(N^2) solves to O(N^log2(7)) = O(N^2.807).

 PSEUDO CODE:
 ------------
 START
   Function Strassen(A, B, N):
     If N == 1: Return A * B

     Divide A into A11, A12, A21, A22
     Divide B into B11, B12, B21, B22

     M1 = Strassen(A11 + A22, B11 + B22, N/2)
     M2 = Strassen(A21 + A22, B11,      N/2)
     M3 = Strassen(A11,      B12 - B22, N/2)
     M4 = Strassen(A22,      B21 - B11, N/2)
     M5 = Strassen(A11 + A12, B22,      N/2)
     M6 = Strassen(A21 - A22, B11 + B12, N/2)
     M7 = Strassen(A11 - A22, B21 + B22, N/2)

     C11 = M1 + M4 - M5 + M7
     C12 = M3 + M5
     C21 = M2 + M4
     C22 = M1 - M2 + M3 + M6

     Combine C11, C12, C21, C22 into C
     Return C
 END

 SAMPLE INPUT & DRY RUN (2x2 matrices):
 ----------------------------------------
 A = | 1 1 |   B = | 1 1 |
     | 1 1 |       | 1 1 |

 N=2, so quadrants are 1x1 (base case reached immediately):
   A11=1, A12=1, A21=1, A22=1
   B11=1, B12=1, B21=1, B22=1

 7 products:
   M1 = (1+1)*(1+1) = 4
   M2 = (1+1)*1     = 2
   M3 = 1*(1-1)     = 0
   M4 = 1*(1-1)     = 0
   M5 = (1+1)*1     = 2
   M6 = (1-1)*(1+1) = 0
   M7 = (1-1)*(1+1) = 0

 Combine:
   C11 = 4 + 0 - 2 + 0 = 2
   C12 = 0 + 2         = 2
   C21 = 2 + 0         = 2
   C22 = 4 - 2 + 0 + 0 = 2

 Result: C = | 2 2 |   (matches standard multiplication)
             | 2 2 |

 TIME COMPLEXITY:
 - Standard:  O(N^3)
 - Strassen:  O(N^2.807)  [T(N) = 7T(N/2) + O(N^2)]

 SPACE COMPLEXITY: O(N^2) for intermediate sub-matrices

 COMMON USE CASES:
 - Large-scale scientific computing and simulations
 - Graphics and geometry transformations
 - Machine learning tensor operations (inspiration for variants)
 - Teaching the power of Divide & Conquer to beat naive exponents
DIAGRAM (split each matrix into 4 quadrants):
  A = |A11 A12|   B = |B11 B12|
      |A21 A22|       |B21 B22|
  7 clever sums/products instead of 8:
  M1=(A11+A22)(B11+B22) ... M7 -> combine into C11..C22
  T(n) = 7T(n/2) -> O(n^2.81) vs O(n^3).

*/

#include <stdio.h>
#include <stdlib.h>

#define N 4

/*
 * addMatrix - Element-wise C = A + B.
 */
void addMatrix(int A[][N], int B[][N], int C[][N], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/*
 * subMatrix - Element-wise C = A - B.
 */
void subMatrix(int A[][N], int B[][N], int C[][N], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

/*
 * multiplyNaive - Standard O(N^3) multiplication, used as the base case
 * for small blocks. Strassen only pays off for large matrices, so we
 * fall back to this for small sizes.
 */
void multiplyNaive(int A[][N], int B[][N], int C[][N], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/*
 * strassen - Recursive Strassen multiplication.
 *
 * Works on size x size blocks located at offsets (ai,aj) in A and
 * (bi,bj) in B, writing results at (ci,cj) in C. Using offsets avoids
 * copying sub-matrices, keeping the code compact and efficient.
 *
 * Analogy: Instead of 8 block multiplications, do 7 clever ones.
 */
void strassen(int A[][N], int B[][N], int C[][N],
              int ai, int aj, int bi, int bj, int ci, int cj, int size) {

    if (size <= 2) {
        int tempA[2][2], tempB[2][2], tempC[2][2];

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                tempA[i][j] = A[ai + i][aj + j];
                tempB[i][j] = B[bi + i][bj + j];
            }
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                tempC[i][j] = 0;
                for (int k = 0; k < size; k++) {
                    tempC[i][j] += tempA[i][k] * tempB[k][j];
                }
            }
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                C[ci + i][cj + j] = tempC[i][j];
            }
        }
        return;
    }

    int half = size / 2;

    int M1[N][N], M2[N][N], M3[N][N], M4[N][N];
    int M5[N][N], M6[N][N], M7[N][N];

    /* M1 = (A11 + A22) * (B11 + B22) */
    int X[N][N], Y[N][N];
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + i][aj + j] + A[ai + half + i][aj + half + j];
            Y[i][j] = B[bi + i][bj + j] + B[bi + half + i][bj + half + j];
        }
    }
    strassen(X, Y, M1, 0, 0, 0, 0, 0, 0, half);

    /* M2 = (A21 + A22) * B11 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + half + i][aj + j] + A[ai + half + i][aj + half + j];
            Y[i][j] = B[bi + i][bj + j];
        }
    }
    strassen(X, Y, M2, 0, 0, 0, 0, 0, 0, half);

    /* M3 = A11 * (B12 - B22) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + i][aj + j];
            Y[i][j] = B[bi + i][bj + half + j] - B[bi + half + i][bj + half + j];
        }
    }
    strassen(X, Y, M3, 0, 0, 0, 0, 0, 0, half);

    /* M4 = A22 * (B21 - B11) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + half + i][aj + half + j];
            Y[i][j] = B[bi + half + i][bj + j] - B[bi + i][bj + j];
        }
    }
    strassen(X, Y, M4, 0, 0, 0, 0, 0, 0, half);

    /* M5 = (A11 + A12) * B22 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + i][aj + j] + A[ai + i][aj + half + j];
            Y[i][j] = B[bi + half + i][bj + half + j];
        }
    }
    strassen(X, Y, M5, 0, 0, 0, 0, 0, 0, half);

    /* M6 = (A21 - A11) * (B11 + B12) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + half + i][aj + j] - A[ai + i][aj + j];
            Y[i][j] = B[bi + i][bj + j] + B[bi + i][bj + half + j];
        }
    }
    strassen(X, Y, M6, 0, 0, 0, 0, 0, 0, half);

    /* M7 = (A12 - A22) * (B21 + B22) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            X[i][j] = A[ai + i][aj + half + j] - A[ai + half + i][aj + half + j];
            Y[i][j] = B[bi + half + i][bj + j] + B[bi + half + i][bj + half + j];
        }
    }
    strassen(X, Y, M7, 0, 0, 0, 0, 0, 0, half);

    /* C11 = M1 + M4 - M5 + M7 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[ci + i][cj + j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
        }
    }
    /* C12 = M3 + M5 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[ci + i][cj + half + j] = M3[i][j] + M5[i][j];
        }
    }
    /* C21 = M2 + M4 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[ci + half + i][cj + j] = M2[i][j] + M4[i][j];
        }
    }
    /* C22 = M1 - M2 + M3 + M6 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[ci + half + i][cj + half + j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }
}

/*
 * printMatrix - Displays a size x size matrix with a label.
 */
void printMatrix(const char* label, int M[][N], int size) {
    printf("  %s\n", label);
    for (int i = 0; i < size; i++) {
        printf("    |");
        for (int j = 0; j < size; j++) {
            printf(" %3d", M[i][j]);
        }
        printf(" |\n");
    }
    printf("\n");
}

int main() {
    int A[N][N];
    int B[N][N];
    int C[N][N];
    int standard[N][N];

    printf("Strassen's Matrix Multiplication - Divide & Conquer\n\n");

    /* STEP: read both 4x4 matrices from the user */
    printf("Enter 16 values for matrix A row by row (4 per row):\n");
    for (int i = 0; i < N; i++) {
        printf("  Row %d: ", i + 1);
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter 16 values for matrix B row by row (4 per row):\n");
    for (int i = 0; i < N; i++) {
        printf("  Row %d: ", i + 1);
        for (int j = 0; j < N; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    printf("\n");
    printMatrix("Matrix A:", A, N);
    printMatrix("Matrix B:", B, N);

    strassen(A, B, C, 0, 0, 0, 0, 0, 0, N);
    multiplyNaive(A, B, standard, N);

    printf("Result via Strassen:\n");
    printMatrix("C = A x B:", C, N);

    printf("Verification (standard O(N^3) result):\n");
    printMatrix("C = A x B:", standard, N);

    int match = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (C[i][j] != standard[i][j]) {
                match = 0;
            }
        }
    }

    printf("Strassen matches standard multiplication: %s\n",
           match ? "YES" : "NO");

    return 0;
}
