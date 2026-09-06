/*
Z-Algorithm - Linear-Time Pattern Matching (Master)
----------------------------------------------------
What this program solves:
Computes the Z-array: for each position i, the length of the
longest substring starting at i that MATCHES the string's prefix.
Then uses it for linear-time pattern search - no hashing, no
failure links, one clean idea.

Real life example:
Pattern-occurrence counting in text editors and bioinformatics
repeat-finding - Z arrays answer "longest match starting here"
in linear time.

DIAGRAM (Z-array of "aabaab": longest prefix match at each i):
  s:  a a b a a b
  z:  0 1 0 4 1 0     z=4 at index 3: "aaba" == prefix "aaba"
  pattern search: build Z of pattern#text; z == |pattern| -> hit

BEGINNER EXPLANATION:
Z[i] answers: "how much of the string's PREFIX appears again,
starting right here?" We build a [left, right] window = the
rightmost match discovered so far. Inside the window, position i
mirrors position i-left - copy its Z value (the text repeats!),
then extend by direct comparison if possible. Outside the window,
compare from scratch. The window trick is what makes it O(n):
each character is compared at most... a constant number of times.

SUPER SIMPLE EXAMPLE ("aabxaab"):
Z[0] = 0 (by convention/length).
i=4 'a': compare a-a, a-a, b-x: match "aab" -> Z[4] = 3.
i=5 'a': inside window? The window from i=4 spans [4,6].
  Z[5] mirrors Z[1] = 1... extend: match 'a' vs 'a' -> Z[5] = 1.
Pattern search: concatenate P + '$' + T; any Z[i] == |P| is a
full match at text position i - |P| - 1.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Z[0] = |string| by definition (whole string matches itself);
  many implementations set it to 0 or n - both are seen; this file
  uses n and documents it.
- All-same string "aaaa": Z = 4,3,2,1 - the window grows once and
  every later position copies - O(n) despite heavy repetition
  (naive comparing would be O(n^2)).
- Separator '$' MUST NOT appear in text or pattern - otherwise
  false matches can span it. Use a character absent from input.
- Empty pattern: no search (reject).
- Overlapping matches: Z finds them all (e.g. "aa" in "aaaa"
  at positions 0, 1, 2).

DRY RUN ("aaabaanaab", pattern "aab"):
combined = "aab$aaabaanaab"
Z: index 9 = 3 (a-a-b matches prefix), index 10 = 1,
   index 8? text starts at index 4 of combined...
Full matches where Z == 3: combined positions 8, 9? Trace the
program output - it prints the Z-array and match positions
directly (that is the point: verify, don't hand-compute).

TIPS:
- Z vs KMP: Z is EASIER to derive (one window idea) and equally
  linear; KMP's LPS is the reverse-engineered cousin. Knowing
  both, pick Z for implementation speed.
- Z-array alone solves: string periodicity (smallest period =
  first i with i + Z[i] == n), number of distinct substrings
  (with suffix automaton ideas), and pattern matching.
- Compare with rabin_karp.c (hashing) and kmp-string-matching.c - three
  linear algorithms, three different invariants.
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000

int zArr[2 * MAX_LEN + 2];

/*
 * buildZ - Z-array of the combined string P$T.
 */
void buildZ(const char* s, int len) {
    zArr[0] = len;
    int left = 0;
    int right = 0;

    for (int i = 1; i < len; i++) {
        if (i <= right) {
            int mirror = i - left;
            if (zArr[mirror] < right - i + 1) {
                zArr[i] = zArr[mirror];
                continue;
            }
            zArr[i] = right - i + 1;
        } else {
            zArr[i] = 0;
        }

        while (i + zArr[i] < len &&
               s[zArr[i]] == s[i + zArr[i]]) {
            zArr[i] = zArr[i] + 1;
        }

        if (i + zArr[i] - 1 > right) {
            left = i;
            right = i + zArr[i] - 1;
        }
    }
}

int main() {
    char pattern[200];
    char text[500];
    char combined[2 * MAX_LEN + 2];
    char again;

    printf("Z-Algorithm Pattern Matching\n\n");

    do {
        printf("Pattern (max 199 chars): ");
        scanf("%199s", pattern);
        printf("Text (max 499 chars): ");
        scanf("%499s", text);

        int plen = (int)strlen(pattern);
        int tlen = (int)strlen(text);

        if (plen == 0 || tlen == 0) {
            printf("Empty input\n");
            continue;
        }

        snprintf(combined, sizeof(combined), "%s$%s", pattern, text);
        int total = plen + 1 + tlen;

        /* STEP: build Z-array of pattern$text */
        buildZ(combined, total);

        printf("\nCombined: %s\n", combined);
        printf("Z-array:  ");
        for (int i = 0; i < total; i++) {
            printf("%d ", zArr[i]);
        }
        printf("\n");

        printf("Matches (Z[i] == %d):\n", plen);
        int count = 0;
        for (int i = plen + 1; i < total; i++) {
            if (zArr[i] == plen) {
                printf("  at text index %d\n", i - plen - 1);
                count = count + 1;
            }
        }
        printf("Total: %d matches\n", count);

        printf("\nRun again? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
