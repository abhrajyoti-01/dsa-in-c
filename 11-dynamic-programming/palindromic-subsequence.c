/*
Longest Palindromic Subsequence
-------------------------------
What this program solves:
Finds the length of the longest subsequence (not necessarily
contiguous) of a string that reads the same forwards and backwards,
and reconstructs one such subsequence.

Real life example:
Bioinformatics: DNA palindromic sequences act as binding sites.
Also used in text processing - the answer relates to how many
characters you must delete to make a string a palindrome
(n - LPS length).

DIAGRAM (LPS of "bbbab"):
       ""  b  b  b  a  b
   ""   0  0  0  0  0  0
   b    0  1  2  3  3  4
   ... cell rules: ends match -> 2 + inner cell,
   else max(skip left, skip right). Answer: 4 ("bbbb")

How the process works:
1. dp[i][j] = LPS length inside substring s[i..j].
2. Recurrence:
   - i == j: dp = 1 (single char is a palindrome).
   - s[i] == s[j]: dp = 2 + dp[i+1][j-1] (match the ends).
   - else: dp = max(dp[i+1][j], dp[i][j-1]) (drop one end).
3. Fill by increasing substring length; dp[0][n-1] is the answer.
4. Reconstruct by walking from (0, n-1) following the recurrence.

Pseudo code:
START
FOR i = n-1..0
    dp[i][i] = 1
    FOR j = i+1..n-1
        IF s[i] == s[j]: dp[i][j] = 2 + dp[i+1][j-1]
        ELSE: dp[i][j] = MAX(dp[i+1][j], dp[i][j-1])
ANSWER = dp[0][n-1]
END

Sample input and dry run:
"BBABCBCAB" -> LPS length 7
one LPS: "BACBCAB"
dp[0][8] built from smaller intervals, e.g. "BCB" inside.

Main logic to understand:
Interval DP: solve every shorter substring first, then extend.
Matching ends contribute 2 plus the inner answer; mismatched ends
mean the answer lives entirely on one side. O(n^2) time and space.
LPS(s) = LCS(s, reverse(s)) - the two problems are equivalent.
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

int main() {
    char s[MAX_LEN];
    static int dp[MAX_LEN][MAX_LEN];
    char lps[MAX_LEN];

    printf("Longest Palindromic Subsequence\n\n");

    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    scanf("%99s", s);

    int n = (int)strlen(s);

    /* STEP: interval DP - shorter substrings first */
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                int dropLeft = dp[i + 1][j];
                int dropRight = dp[i][j - 1];
                dp[i][j] = (dropLeft > dropRight) ? dropLeft : dropRight;
            }
        }
    }

    printf("String: %s\n", s);
    printf("LPS length: %d\n", dp[0][n - 1]);

    /* STEP: reconstruct the subsequence */
    int left = 0;
    int right = n - 1;
    int front = 0;
    int back = dp[0][n - 1];

    while (left <= right) {
        if (left == right) {
            lps[front] = s[left];
            front = front + 1;
            break;
        }
        if (s[left] == s[right]) {
            lps[front] = s[left];
            lps[back - 1] = s[right];
            front = front + 1;
            back = back - 1;
            left = left + 1;
            right = right - 1;
        } else if (dp[left + 1][right] >= dp[left][right - 1]) {
            left = left + 1;
        } else {
            right = right - 1;
        }
    }
    lps[dp[0][n - 1]] = '\0';

    printf("One LPS: %s\n", lps);
    printf("Min deletions to make a palindrome: %d\n", n - dp[0][n - 1]);

    return 0;
}
