/*
Naive String Matching
---------------------
What this program solves:
Finds ALL occurrences of a pattern in a text by trying every
possible alignment and comparing character by character.

Real life example:
Searching a word in Notepad with "Match Case" off - the simplest
algorithm just slides the search word across the text one position
at a time and compares letters until they stop matching.

DIAGRAM (slide the window one character at a time):
  text "aabaab", pattern "aab":
  [a a b] a a b  match!
   a [a a b] a b ... every position checked from scratch: O(n*m)

How the process works:
1. Slide a window of length m over the text: positions 0..n-m.
2. At each position compare pattern with the window character by
   character.
3. Full match -> report the starting index; a mismatch stops that
   alignment immediately.

Pseudo code:
START
FOR s = 0..n-m
    j = 0
    WHILE j < m AND text[s+j] == pattern[j]: j++
    IF j == m: PRINT "match at s"
END

Sample input and dry run:
text = "ABABAB", pattern = "AB"
s=0 match, s=2 match, s=4 match -> matches at 0, 2, 4

Main logic to understand:
O((n-m+1)*m) worst case (e.g. text "AAAA", pattern "AAB") but often
fast in practice for short patterns. No preprocessing needed - the
baseline that KMP/Rabin-Karp improve upon.
*/

#include <stdio.h>
#include <string.h>

int main() {
    char text[500];
    char pattern[100];

    printf("Naive Pattern Matching\n\n");

    printf("Enter text (max 499 chars): ");
    scanf("%499s", text);

    printf("Enter pattern (max 99 chars): ");
    scanf("%99s", pattern);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);
    int count = 0;
    int comparisons = 0;

    printf("\nText:    %s\n", text);
    printf("Pattern: %s\n", n >= m ? "" : "(longer than text!)");

    /* STEP: slide the pattern over every alignment */
    if (m <= n) {
        for (int s = 0; s <= n - m; s++) {
            int j = 0;
            while (j < m && text[s + j] == pattern[j]) {
                j = j + 1;
                comparisons = comparisons + 1;
            }
            if (j < m) {
                comparisons = comparisons + 1;
            }
            if (j == m) {
                printf("Match at index %d\n", s);
                count = count + 1;
            }
        }
    }

    printf("\nTotal matches: %d, comparisons: %d\n", count, comparisons);

    return 0;
}
