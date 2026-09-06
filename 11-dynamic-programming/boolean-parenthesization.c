/*
Boolean Parenthesization
------------------------
What this program solves:
Counts the number of ways to parenthesize a boolean expression
made of symbols T/F and operators |, &, ^ so that the expression
evaluates to TRUE.

Real life example:
Circuit design: a logic gate network can be wired in several ways;
this counts the wiring orders that make the output HIGH. Same DP
powers query optimizers counting evaluation plans.

DIAGRAM (count ways to parenthesize to TRUE, T F T with op |):
  split at op: (T) | (F) -> need T|F... evaluate per split:
  left-true x right-true (+ xnor combos per operator)
  dp[i][j][true/false] built over increasing segment lengths.

How the process works:
1. True[i][j] / False[i][j] = number of parenthesizations of
   symbols i..j evaluating to true / false.
2. Split at every operator k between i..j-1:
   ways = True[i][k-1]*True[k+1][j] etc. combined per operator:
   - '|' : T = LT*LT + LT*LF + LF*LT ; F = LF*LF
   - '&' : T = LT*LT ; F = LF*LF + LT*LF + LF*LT
   - '^' : T = LT*LF + LF*LT ; F = LT*LT + LF*LF
3. Fill over increasing interval length; answer = True[0][n-1].

Pseudo code:
START
FOR every single symbol i: set True[i][i] / False[i][i]
FOR length = 3..n step 2
    FOR i in range, k = i+1, i+3 ... (operator positions)
        COMBINE per operator into True[i][i+len-1], False[...]
ANSWER = True[0][n-1]
END

Sample input and dry run:
"T|F&T" -> True[0][4]: splits at '|' and '&'
total ways to evaluate to TRUE = 2

Main logic to understand:
Interval DP over operator splits: each parenthesization picks one
operator as the ROOT, so counts multiply across the two halves and
sum over roots. O(n^3) time, O(n^2) space.
*/

#include <stdio.h>
#include <string.h>

#define MAX_SYM 50

int main() {
    char expr[150];
    static long long True[MAX_SYM][MAX_SYM];
    static long long False[MAX_SYM][MAX_SYM];

    printf("Boolean Parenthesization - Count Ways to get TRUE\n\n");
    printf("Format: symbols T/F, operators | & ^ (no spaces)\n");
    printf("Example: T|F&T^F\n\n");

    printf("Enter expression: ");
    scanf("%149s", expr);

    int len = (int)strlen(expr);

    if (len % 2 == 0 || len < 1 || len > 2 * MAX_SYM - 1) {
        printf("Invalid expression format\n");
        return 1;
    }

    for (int i = 0; i < len; i = i + 2) {
        if (expr[i] != 'T' && expr[i] != 'F') {
            printf("Invalid symbol at position %d\n", i);
            return 1;
        }
    }
    for (int i = 1; i < len; i = i + 2) {
        if (expr[i] != '|' && expr[i] != '&' && expr[i] != '^') {
            printf("Invalid operator at position %d\n", i);
            return 1;
        }
    }

    int symCount = (len + 1) / 2;

    /* STEP: base case - single symbols */
    for (int i = 0; i < symCount; i++) {
        char c = expr[2 * i];
        True[i][i] = (c == 'T') ? 1 : 0;
        False[i][i] = (c == 'F') ? 1 : 0;
    }

    /* STEP: intervals of increasing symbol count */
    for (int count2 = 2; count2 <= symCount; count2++) {
        for (int i = 0; i + count2 - 1 < symCount; i++) {
            int j = i + count2 - 1;
            True[i][j] = 0;
            False[i][j] = 0;

            for (int k = i; k < j; k++) {
                char op = expr[2 * k + 1];
                long long lt = True[i][k];
                long long lf = False[i][k];
                long long rt = True[k + 1][j];
                long long rf = False[k + 1][j];

                if (op == '|') {
                    True[i][j] += lt * rt + lt * rf + lf * rt;
                    False[i][j] += lf * rf;
                } else if (op == '&') {
                    True[i][j] += lt * rt;
                    False[i][j] += lf * rf + lt * rf + lf * rt;
                } else {
                    True[i][j] += lt * rf + lf * rt;
                    False[i][j] += lt * rt + lf * rf;
                }
            }
        }
    }

    printf("\nExpression: %s\n", expr);
    printf("Ways to evaluate to TRUE:  %lld\n", True[0][symCount - 1]);
    printf("Ways to evaluate to FALSE: %lld\n", False[0][symCount - 1]);
    printf("Total parenthesizations:   %lld\n",
           True[0][symCount - 1] + False[0][symCount - 1]);

    return 0;
}
