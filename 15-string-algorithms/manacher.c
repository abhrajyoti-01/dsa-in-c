/*
Manacher's Algorithm - Longest Palindromic Substring (Advanced)
-------------------------------------------------------------
PROBLEM DEFINITION:
Find the longest contiguous palindrome in a line of text in O(n) time,
including both odd- and even-length palindromes. On ties choose the
leftmost occurrence. Spaces and punctuation count as ordinary bytes.

REAL-LIFE EXAMPLE:
Detect symmetric text fragments or repeated mirrored patterns without
trying every substring independently.

DIAGRAM:
  odd:   r a c e c a r       center e, radius 4 (includes center)
  even:    a b b a          center between b's, radius 2
  Maintain the rightmost known palindrome [left..right]. A center
  inside it mirrors another center; reuse its radius up to the boundary.

PSEUDO CODE:
  For each center i, separately for odd and even palindromes:
      If outside known window, start radius at 1 (odd) or 0 (even)
      Otherwise copy min(mirrored radius, distance to right boundary)
      Expand while both sides exist and bytes match
      Save radius; extend the known window if needed
      Update best by length, then by leftmost starting position

SAMPLE INPUT & DRY RUN:
  abba
  Odd radii:  [1,1,1,1].
  Even radii: [0,0,2,0]. At i=2 compare b/b, then a/a.
  start = 2-2 = 0, length = 2*2 = 4 -> "abba".

SPECIAL CASES:
  - Empty line gives length 0; a single byte gives length 1.
  - "babad" gives "bab", not the equally long later "aba".
  - All identical bytes still take O(n), not O(n^2).
  - No separator/sentinel byte is required, so '#' and '$' are safe.
  - At most 1000 bytes; comparison is case-sensitive, not Unicode-aware.
    EOF without a line is invalid; a final line need not end in newline.

COMPLEXITY:
  O(n) time and O(n) radius storage. Successful expansions beyond a
  reused window advance its right boundary; it never moves backwards.

TIPS:
  A substring is contiguous; palindromic-subsequence solves a different
  problem. Separate odd/even arrays avoid transformed-string indexing.
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000

int minimum(int a, int b) {
    return a < b ? a : b;
}

void updateBest(int start, int length, int* bestStart, int* bestLength) {
    if (length > *bestLength || (length == *bestLength && start < *bestStart)) {
        *bestStart = start;
        *bestLength = length;
    }
}

void manacher(const char* text, int n, int* bestStart, int* bestLength) {
    int odd[MAX_LEN];
    int even[MAX_LEN];
    int left = 0;
    int right = -1;
    *bestStart = 0;
    *bestLength = 0;

    for (int i = 0; i < n; i++) {
        int radius = i > right ? 1 : minimum(odd[left + right - i], right - i + 1);
        while (i - radius >= 0 && i + radius < n &&
               text[i - radius] == text[i + radius]) radius++;
        odd[i] = radius;
        updateBest(i - radius + 1, 2 * radius - 1, bestStart, bestLength);
        if (i + radius - 1 > right) {
            left = i - radius + 1;
            right = i + radius - 1;
        }
    }
    left = 0;
    right = -1;
    for (int i = 0; i < n; i++) {
        int radius = i > right ? 0 : minimum(even[left + right - i + 1], right - i + 1);
        while (i - radius - 1 >= 0 && i + radius < n &&
               text[i - radius - 1] == text[i + radius]) radius++;
        even[i] = radius;
        updateBest(i - radius, 2 * radius, bestStart, bestLength);
        if (i + radius - 1 > right) {
            left = i - radius;
            right = i + radius - 1;
        }
    }
}

int main(void) {
    char text[MAX_LEN + 3];
    int bestStart;
    int bestLength;
    printf("Manacher's Algorithm - Longest Palindromic Substring\n\n");
    printf("Enter a line (0-%d bytes, spaces allowed): ", MAX_LEN);
    if (fgets(text, sizeof(text), stdin) == NULL) {
        printf("Invalid input\n");
        return 1;
    }
    size_t length = strlen(text);
    if (length > 0 && text[length - 1] == '\n') text[--length] = '\0';
    if (length > 0 && text[length - 1] == '\r') text[--length] = '\0';
    if (length > MAX_LEN) {
        printf("Input too long\n");
        return 1;
    }
    manacher(text, (int)length, &bestStart, &bestLength);
    printf("Longest palindrome: \"%.*s\"\n", bestLength, text + bestStart);
    printf("Start index: %d\nLength: %d\n", bestStart, bestLength);
    return 0;
}
