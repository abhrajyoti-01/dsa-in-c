/*
KMP (Knuth-Morris-Pratt) String Matching
----------------------------------------
What this program solves:
Finds all pattern occurrences in O(n + m) by precomputing the
"failure function" (LPS array) so the text pointer NEVER moves
backwards - no re-comparison of text characters.

Real life example:
Proofreading a long contract for the phrase "notwithstanding":
after a partial match fails, you know exactly how much of the
pattern is still valid from the failure table - you never re-read
characters you have already seen.

DIAGRAM (failure function lets the text pointer NEVER move back):
  pattern "ababca": lps = [0 0 1 2 0 1]
  mismatch after matching "abab": fall back to lps[3]=2, compare
  from pattern index 2 - the matched prefix "ab" is still aligned.
  O(n + m) total.

How the process works:
1. LPS[i] = length of the longest proper prefix of pattern that is
   also a suffix of pattern[0..i] (precomputed in O(m)).
2. Scan the text with pointers i (text, never decreases) and j
   (pattern).
3. On mismatch, do NOT restart - jump j back to LPS[j-1].
4. On full match, record i-j, then j = LPS[j-1] to keep scanning.

Pseudo code:
START
BUILD LPS: len = 0; FOR i = 1..m-1
    WHILE len > 0 AND p[i] != p[len]: len = LPS[len-1]
    IF p[i] == p[len]: len++; LPS[i] = len
SEARCH: i = j = 0
    WHILE i < n
        IF t[i] == p[j]: i++; j++
        IF j == m: report i-j; j = LPS[j-1]
        ELSE IF i < n AND t[i] != p[j]:
            IF j != 0: j = LPS[j-1] ELSE i++
END

Sample input and dry run:
text = "ABABDABACDABABCABAB", pattern = "ABABCABAB"
LPS = [0,0,1,2,0,1,2,3,4]
matches at index 10

Main logic to understand:
The LPS array encodes "when I fail here, how many pattern chars
are still guaranteed matched?" - turning redundant re-comparisons
into a table lookup. Linear time guaranteed, no worst case blowup.
*/

#include <stdio.h>
#include <string.h>

/*
 * buildLPS - Longest proper prefix that is also a suffix.
 */
void buildLPS(const char* pattern, int m, int lps[]) {
    lps[0] = 0;
    int len = 0;

    for (int i = 1; i < m; i++) {
        while (len > 0 && pattern[i] != pattern[len]) {
            len = lps[len - 1];
        }
        if (pattern[i] == pattern[len]) {
            len = len + 1;
        }
        lps[i] = len;
    }
}

int main() {
    char text[500];
    char pattern[100];
    int lps[100];

    printf("KMP Pattern Matching\n\n");

    printf("Enter text (max 499 chars): ");
    scanf("%499s", text);

    printf("Enter pattern (max 99 chars): ");
    scanf("%99s", pattern);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m > n || m == 0) {
        printf("Pattern longer than text or empty\n");
        return 1;
    }

    /* STEP: precompute the failure function */
    buildLPS(pattern, m, lps);

    printf("\nLPS array: ");
    for (int i = 0; i < m; i++) {
        printf("%d ", lps[i]);
    }
    printf("\n");

    /* STEP: scan with two pointers, text never rewinds */
    int count = 0;
    int j = 0;

    printf("Matches:\n");
    for (int i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = lps[j - 1];
        }
        if (text[i] == pattern[j]) {
            j = j + 1;
        }
        if (j == m) {
            printf("  Match at index %d\n", i - j + 1);
            count = count + 1;
            j = lps[j - 1];
        }
    }

    printf("Total matches: %d\n", count);

    return 0;
}
