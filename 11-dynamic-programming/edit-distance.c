/*
 PROBLEM DEFINITION:
 --------------------
 Edit Distance (Levenshtein Distance) measures how many single-character
 operations are needed to transform one string into another. Allowed
 operations:
   1. INSERT a character
   2. DELETE a character
   3. REPLACE a character

 Dynamic Programming fills a table where cell [i][j] holds the minimum
 operations to convert the first i characters of word1 into the first j
 characters of word2.

 REAL-LIFE EXAMPLE - Spell Checkers & DNA Alignment:
 -----------------------------------------------------
 When you type "recieve" instead of "receive", the spell checker suggests
 the correction because the edit distance is tiny (1 swap ~ 1 operation).
 Search engines show "Did you mean...?" based on the same idea.
 In bioinformatics, edit distance aligns DNA sequences: mutating one base,
 inserting/deleting a base - each is one evolutionary edit. Smaller
 distance implies closer relationship between species.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of a grid where rows = characters of word1, columns = characters
 of word2. Each cell answers: "How many edits to convert word1[0..i)
 into word2[0..j)?"

 1. BASE CASES (empty string edges):
    - dp[0][j] = j   (insert j characters to build word2's prefix)
    - dp[i][0] = i   (delete i characters to shrink word1 to empty)

 2. FILL THE GRID:
    For each cell (i, j):
      - If word1[i-1] == word2[j-1]:
            Characters match -> no operation needed: dp[i][j] = dp[i-1][j-1]
      - Else, take 1 + the best of three choices:
            dp[i][j] = 1 + min(
                dp[i-1][j-1],   // REPLACE: fix both chars, move diagonally
                dp[i-1][j],     // DELETE:  drop word1's char, move up
                dp[i][j-1]      // INSERT:  add word2's char, move left
            )

 3. ANSWER:
    - dp[m][n] (bottom-right cell) = minimum edits for the full strings.

 4. INTUITION (diagonal moves):
    - Matching characters let us "walk diagonally for free".
    - Every mismatch costs exactly one operation, and we choose the
      cheapest direction: replace (diagonal), delete (up), insert (left).

 PSEUDO CODE:
 ------------
 START
   Function EditDistance(word1, word2):
     m = length(word1), n = length(word2)
     For i = 0 to m: dp[i][0] = i
     For j = 0 to n: dp[0][j] = j

     For i = 1 to m:
         For j = 1 to n:
             If word1[i-1] == word2[j-1]:
                 dp[i][j] = dp[i-1][j-1]
             Else:
                 dp[i][j] = 1 + MIN(dp[i-1][j-1], dp[i-1][j], dp[i][j-1])

     Return dp[m][n]
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 word1 = "horse", word2 = "ros"

 dp table (rows: _ h o r s e; cols: _ r o s):

          ""  r   o   s
     ""    0  1   2   3
     h     1  1   2   3
     o     2  2   1   2
     r     3  2   2   2
     s     4  3   3   2
     e     5  4   4   3

 Walkthrough of key cells:
   dp[2][2] (o vs o):  match -> dp[1][1] = 1
   dp[3][1] (r vs r):  match -> dp[2][0] = 2
   dp[4][3] (s vs s):  match -> dp[3][2] = 2
   dp[5][3] (e vs s):  mismatch -> 1 + min(dp[4][2]=3, dp[4][3]=2, dp[5][2]=4)
                                 = 1 + 2 = 3

 Result: 3 edits.
 One optimal sequence: horse -> rorse (replace h with r)
                       rorse -> rose  (delete r)
                       rose  -> ros   (delete e)

 TIME COMPLEXITY: O(m x n)  - every cell computed once

 SPACE COMPLEXITY: O(m x n)  (reducible to O(min(m,n)) keeping two rows)

 COMMON USE CASES:
 - Spell checkers and "Did you mean?" suggestions
 - Fuzzy search in editors and databases
 - DNA sequence alignment in bioinformatics
 - Plagiarism detection and document similarity
 - Speech recognition (word-level matching)
DIAGRAM (transform "cat" -> "cut", fill cell by cell):
        ""  c  u  t
    ""   0  1  2  3
    c    1  0  1  2
    a    2  1  1  2
    t    3  2  2  1
  cell = min(top, left, diag) + (chars differ ? 1 : 0)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

/*
 * minimum - Returns the smallest of three integers.
 * Used when choosing the cheapest of insert/delete/replace.
 */
int minimum(int a, int b, int c) {
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    return min;
}

/*
 * editDistance - Fills the DP table and returns the minimum edits.
 * Optionally prints the table when verbose is 1.
 *
 * Analogy: Filling a grid where each step is one edit; matching letters
 * let us move diagonally for free.
 */
int editDistance(const char* word1, const char* word2, int printTable) {
    int m = (int)strlen(word1);
    int n = (int)strlen(word2);

    int dp[MAX_LEN][MAX_LEN];

    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + minimum(dp[i - 1][j - 1],
                                       dp[i - 1][j],
                                       dp[i][j - 1]);
            }
        }
    }

    if (printTable) {
        printf("        ");
        for (int j = 0; j < n; j++) {
            printf("%4c", word2[j]);
        }
        printf("\n");

        for (int i = 0; i <= m; i++) {
            if (i == 0) {
                printf("    ");
            } else {
                printf("  %c ", word1[i - 1]);
            }
            printf("%3d", dp[i][0]);
            for (int j = 1; j <= n; j++) {
                printf("%4d", dp[i][j]);
            }
            printf("\n");
        }
    }

    return dp[m][n];
}

int main() {
    char w1[MAX_LEN];
    char w2[MAX_LEN];

    printf("Edit Distance (Levenshtein) - Dynamic Programming\n\n");

    printf("Enter first word (max %d chars): ", MAX_LEN - 1);
    scanf("%99s", w1);

    printf("Enter second word (max %d chars): ", MAX_LEN - 1);
    scanf("%99s", w2);

    printf("\nWord1 = \"%s\" (rows)\n", w1);
    printf("Word2 = \"%s\" (columns)\n\n", w2);

    int dist = editDistance(w1, w2, 1);

    printf("\nMinimum edits: %d\n", dist);

    return 0;
}
