/*
Matrix Chain Multiplication
---------------------------
What this program solves:
This program finds the minimum number of scalar multiplications needed to multiply
a sequence of matrices.

Real life example:
Suppose a graphics application must multiply several transformation matrices.
The final result is the same no matter how the brackets are placed, but the
number of calculations can change a lot. So the program tries to find the
cheapest multiplication order.

DIAGRAM (A1(10x20) A2(20x5) A3(5x30): which order multiplies fastest?):
  ((A1 A2) A3): 10*20*5 + 10*5*30  = 1000 + 1500 = 2500 ops
  (A1 (A2 A3)): 20*5*30 + 10*20*30 = 3000 + 6000 = 9000 ops
  dp[i][j] = min over split k of dp[i][k] + dp[k+1][j] + cost.

How the process works:
1. Read the dimensions of matrices.
2. Consider every chain length from small to large.
3. For each subchain, try every possible split point.
4. Calculate the total multiplication cost for that split.
5. Keep the minimum cost among all possible splits.

Pseudo code:
START
Read number of matrices and their dimensions
Set cost of single matrix multiplication as 0
For each chain length from 2 to n
    For each subchain i to j
        Set current minimum cost as INF
        Try every split point k between i and j
            Compute multiplication cost
            Update minimum if smaller
Print minimum cost for full chain
END

Sample input and dry run:
Suppose dimensions are 10 30 5 60
That means matrices are A(10x30), B(30x5), C(5x60)
If we compute (A x B) first, cost = 10x30x5 = 1500, then result x C costs 10x5x60 = 3000
Total = 4500
If we compute (B x C) first, cost = 30x5x60 = 9000, then A x result costs 10x30x60 = 18000
So the minimum cost is 4500.

Main logic to understand:
The result matrix is the same, but the order of multiplication changes the
amount of work, so dynamic programming is used to test all useful bracketings.
*/
#include <stdio.h>

#define MAX 20

int main() {
    int p[MAX], m[MAX][MAX];
    int n, i, j, k, length, q;

    printf("Matrix Chain Multiplication\n");
    printf("Enter the number of matrices: ");
    scanf("%d", &n);

    printf("Enter %d dimensions:\n", n + 1);
    for (i = 0; i <= n; i++) {
        scanf("%d", &p[i]);
    }

    for (i = 1; i <= n; i++) {
        m[i][i] = 0;
    }

    for (length = 2; length <= n; length++) {
        for (i = 1; i <= n - length + 1; i++) {
            j = i + length - 1;
            m[i][j] = 999999;

            for (k = i; k < j; k++) {
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                }
            }
        }
    }

    printf("Minimum number of multiplications: %d\n", m[1][n]);
    return 0;
}

