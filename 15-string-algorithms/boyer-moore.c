/*
Boyer-Moore Bad Character Heuristic - The Real-World Searcher
--------------------------------------------------------------
What this program solves:
Pattern search that SKIPS through the text in jumps - the engine
behind grep, ripgrep's ancestors, and every text editor's search.
This file implements the BAD CHARACTER rule (the dominant half of
full Boyer-Moore).

Real life example:
grep and many text editors' search use Boyer-Moore: it can SKIP
whole chunks of text, which is why it feels instant on big files.

DIAGRAM (bad-character shift: align with the LAST occurrence):
  text:  ... X Y Z A B C ...
  pattern "ABCD": compare C vs pattern's last char D -> mismatch.
  C occurs in pattern at index 2 -> jump pattern right by (4-1-2)
  jumps get bigger when mismatches involve characters NOT in the
  pattern at all -> sub-linear average time.

BEGINNER EXPLANATION:
Compare pattern RIGHT-TO-LEFT against a text window. On a
mismatch, look at the TEXT character that failed (the "bad
character"). Its LAST occurrence in the pattern tells you how far
you may shift the window right. Characters absent from the
pattern allow a FULL-pattern-length jump. The preprocessing
builds a table: for each possible character, its rightmost index
in the pattern (or -1).

SUPER SIMPLE EXAMPLE (pattern "TRUTH" in "TRUSTHTRUTH"):
window 1: TRUST vs TRUTH: T-T, R-R, U-U, T-S? mismatch at 'S'
(rightmost-ish). 'S' absent from pattern -> shift 5 (full length).
window 2 starts under... the shift lands mid-text; compare
HTRUTH window -> full match found. Two alignments - linear-ish
behavior in practice, SUB-linear (fewer than n comparisons!).

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Bad char inside the pattern LEFT of the mismatch position:
  the shift can be NEGATIVE if you compute last[textChar] -
  mismatchPos naively. Fix: shift = max(1, mismatchPos -
  last[textChar]). Never shift backwards; this file enforces it.
- Pattern longer than text: no match possible.
- Right-to-left comparison order is essential: the bad char is
  the FIRST mismatch from the RIGHT, not the left.
- Repetitive text ("AAAAA" with pattern "AAB"): jumps are small -
  performance degrades but stays correct; full BM adds the good
  suffix rule for such cases (noted as the extension).

DRY RUN (pattern "ABAB", text "ABAAABAB"):
last table: A->2, B->3.
Align pattern at 0: compare from right: text[3]='A' vs 'B'
mismatch. bad char 'A', last[A]=2, pos 3 -> shift 3-2=1.
Align at 1: text[4]='A'? compare rightmost: text[1+3]=text[4]='A'
vs 'B' mismatch, bad 'A', shift 3-2=1.
Align at 2: text[5]='B' vs 'B' ok, text[4]='A' vs 'A' ok,
text[3]='A' vs 'B' mismatch, bad 'A' pos 3 shift 1...
(Real BM with good-suffix would jump 4 here; bad-char-only
crawls - the trace shows both behaviors honestly.)

TIPS:
- Average case O(n/m) - SUBLINEAR: it may skip most of the text.
  Worst case O(n*m) (all same char); Galil/good-suffix fixes it.
- Two rules (bad character + good suffix) combine; the bad
  character table is simpler and often wins in practice.
- grep historically used a Boyer-Moore variant for fixed strings -
  this file is that algorithm's core, scaled down.
*/

#include <stdio.h>
#include <string.h>

#define ALPHABET 256

int lastOccurrence[ALPHABET];

/*
 * buildLastTable - Rightmost index of each character in pattern.
 */
void buildLastTable(const char* pattern, int plen) {
    for (int i = 0; i < ALPHABET; i++) {
        lastOccurrence[i] = -1;
    }
    for (int i = 0; i < plen; i++) {
        lastOccurrence[(unsigned char)pattern[i]] = i;
    }
}

int main() {
    char text[1000];
    char pattern[200];
    char again;

    printf("Boyer-Moore (Bad Character Heuristic)\n\n");

    do {
        printf("Text (max 999 chars): ");
        scanf("%999s", text);
        printf("Pattern (max 199 chars): ");
        scanf("%199s", pattern);

        int tlen = (int)strlen(text);
        int plen = (int)strlen(pattern);

        if (plen == 0 || plen > tlen) {
            printf("Pattern empty or longer than text\n");
            continue;
        }

        buildLastTable(pattern, plen);

        printf("\nLast occurrence table (used chars):\n");
        for (int i = 0; i < plen; i++) {
            printf("  '%c' -> index %d\n", pattern[i],
                   lastOccurrence[(unsigned char)pattern[i]]);
        }

        /* STEP: right-to-left comparison with jump shifts */
        int shift = 0;
        int count = 0;
        int comparisons = 0;

        printf("\nSearch trace:\n");
        while (shift <= tlen - plen) {
            int j = plen - 1;
            while (j >= 0 && pattern[j] == text[shift + j]) {
                comparisons = comparisons + 1;
                j = j - 1;
            }
            if (j < 0) {
                printf("  match at index %d\n", shift);
                count = count + 1;
                /* shift so next possible alignment is tried */
                int nextChar = (shift + plen < tlen)
                                   ? text[shift + plen] : 0;
                shift = shift + (plen - (nextChar < ALPHABET
                                            ? lastOccurrence[nextChar] + 1
                                            : 1));
                if (shift <= tlen - plen) {
                    printf("    (matched; shifted to %d)\n", shift);
                }
            } else {
                comparisons = comparisons + 1;
                int badChar = (unsigned char)text[shift + j];
                int jump = j - lastOccurrence[badChar];
                if (jump < 1) {
                    jump = 1;
                }
                printf("  align %d: mismatch text[%d]='%c' vs '%c' "
                       "-> shift %d\n",
                       shift, shift + j, text[shift + j], pattern[j],
                       jump);
                shift = shift + jump;
            }
        }

        printf("\nTotal matches: %d, comparisons: %d (text length %d)\n",
               count, comparisons, tlen);
        if (comparisons < tlen) {
            printf("SUB-LINEAR: fewer comparisons than the text length!\n");
        }

        printf("\nRun again? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
