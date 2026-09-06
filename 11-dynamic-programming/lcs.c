/*
Longest Common Subsequence
--------------------------
What this program solves:
This program finds the length of the longest subsequence common to two strings.
A subsequence keeps the order of characters but does not require them to be side by side.

Real life example:
Imagine comparing two versions of notes written by two students. Even if words or
letters are not exactly at the same positions, we may still want to know the longest
pattern that appears in both in the same order. This is useful in text comparison,
DNA matching, and version comparison problems.

DIAGRAM (LCS of "abcde" and "ace"):
       ""  a  c  e
   ""   0  0  0  0
   a    0  1  1  1
   b    0  1  1  1
   c    0  1  2  2
   d    0  1  2  2
   e    0  1  2  3    -> LCS length 3 ("ace")

How the process works:
1. Build a table for prefixes of both strings.
2. If two current characters match, add 1 to the answer from the diagonal cell.
3. If they do not match, carry forward the larger answer from top or left cell.
4. Continue until the whole table is filled.
5. The last cell stores the final LCS length.

Pseudo code:
START
Read two strings
Create DP table and initialize first row and column as 0
For each character of first string
    For each character of second string
        If characters match
            Set current cell = diagonal cell + 1
        Else
            Set current cell = max(top cell, left cell)
Print value in last cell as LCS length
END

Sample input and dry run:
First string = ABCDGH
Second string = AEDFHR
When A matches A, the value becomes 1.
Later D matches D, so the value grows further.
Then H matches H, so the longest common subsequence length becomes 3.
One possible common subsequence is ADH.

Main logic to understand:
The answer for larger strings depends on answers already computed for smaller prefixes.
*/
#include <stdio.h>
#include <string.h>

int main() {
    char x[100], y[100];
    int dp[100][100];
    int i, j, m, n;

    printf("Longest Common Subsequence\n");
    printf("Enter the first string: ");
    scanf("%s", x);
    printf("Enter the second string: ");
    scanf("%s", y);

    m = strlen(x);
    n = strlen(y);

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = dp[i][j - 1];
            }
        }
    }

    printf("Length of longest common subsequence: %d\n", dp[m][n]);
    return 0;
}

