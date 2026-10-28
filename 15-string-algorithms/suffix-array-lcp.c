/*
Suffix Array + LCP (Kasai) - Every Substring Question in Sorted Order (Master)
-----------------------------------------------------------------------------
PROBLEM DEFINITION:
Sort all suffixes of a string (the suffix array SA) and compute, for
each neighbouring pair in that order, the length of their longest
common prefix (the LCP array). With these two arrays:
  - count distinct substrings:  n(n+1)/2 - sum(LCP)
  - find the longest repeated substring:  max(LCP)
  - find every occurrence of a pattern by binary search over SA
SA is built by prefix doubling (sort by first 1, 2, 4, 8... characters
using the ranks from the previous round); LCP by Kasai's algorithm.

REAL-LIFE EXAMPLE:
Genome tools (read alignment, repeat finding), plagiarism detectors
and full-text indexes (bzip2's Burrows-Wheeler transform is built from
a suffix array) all start from "sort every suffix".

DIAGRAM (text "banana"):
  i  SA[i]  LCP[i]  suffix
  0    5      0     a
  1    3      1     ana        shares "a" with "a"
  2    1      3     anana      shares "ana" with "ana"
  3    0      0     banana
  4    4      0     na
  5    2      2     nana       shares "na" with "na"

PSEUDO CODE:
  BuildSA:
      rank[i] = character code of s[i];  SA = 0..n-1
      for k = 1, 2, 4, ...:
          sort SA by the pair (rank[i], rank[i+k] or -1 past the end)
          re-rank: equal pairs share a rank
          stop when all n ranks are different
  Kasai (uses position[i] = index of suffix i inside SA):
      h = 0
      for i = 0..n-1 (text order):
          if position[i] > 0:
              j = SA[position[i] - 1]
              while s[i+h] == s[j+h]: h++
              LCP[position[i]] = h;  if h > 0: h--
          else h = 0
  Occurrences(p): lower/upper bound of p among the sorted suffixes

SAMPLE INPUT & DRY RUN:
  banana
  3
  ana
  nab
  a
  Round k=1 sorts by (s[i], s[i+1]): ranks of "a?" < "b?" < "n?"...
  after k=2 every suffix has a distinct rank -> the table above.
  sum(LCP) = 0+1+3+0+0+2 = 6;  distinct substrings = 21 - 6 = 15
  longest repeated substring: LCP 3 at SA[2] -> "ana"
  "ana": suffixes 3 and 1 start with it -> 2 occurrences at 1 3
  "nab": none;  "a": 3 occurrences at 1 3 5

SPECIAL CASES:
  - A string of one repeated character ("aaaa") has distinct
    substrings n and longest repeat of length n-1.
  - No repeated substring at all: max LCP = 0, reported as none.
  - Kasai's "h decreases by at most 1" step is what makes it O(n).
  - Text: 1..10000 printable non-space characters; up to 100 queries
    of 1..10000 characters; at most 20 positions are listed per query.

COMPLEXITY:
  Prefix doubling with qsort: O(n log^2 n); Kasai: O(n);
  each query O(m log n). Space O(n).
  (Radix sorting each round gives O(n log n); SA-IS gives O(n).)

TIPS:
  The LCP of two suffixes far apart in SA is the MINIMUM of the LCP
  values between them - combine with a sparse-table for O(1) LCP
  queries. Compare z-algorithm (one pattern) and aho-corasick (many
  patterns, unknown text).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 10000
#define MAX_QUERIES 100
#define TRACE_LIMIT 20
#define MAX_LISTED 20

char text[MAX_LEN + 2];
int n;
int suffixArray[MAX_LEN];
int rankOf[MAX_LEN];
int newRank[MAX_LEN];
int lcp[MAX_LEN];
int gap;

int compareSuffixes(const void *a, const void *b) {
    int i = *(const int *)a;
    int j = *(const int *)b;
    if (rankOf[i] != rankOf[j]) return rankOf[i] - rankOf[j];
    int ri = i + gap < n ? rankOf[i + gap] : -1;
    int rj = j + gap < n ? rankOf[j + gap] : -1;
    return ri - rj;
}

void buildSuffixArray(void) {
    for (int i = 0; i < n; i++) {
        suffixArray[i] = i;
        rankOf[i] = (unsigned char)text[i];
    }
    for (gap = 1;; gap *= 2) {
        qsort(suffixArray, n, sizeof(int), compareSuffixes);
        newRank[suffixArray[0]] = 0;
        for (int i = 1; i < n; i++) {
            newRank[suffixArray[i]] = newRank[suffixArray[i - 1]] +
                (compareSuffixes(&suffixArray[i - 1], &suffixArray[i]) < 0);
        }
        memcpy(rankOf, newRank, n * sizeof(int));
        if (rankOf[suffixArray[n - 1]] == n - 1) break;
    }
}

/* After buildSuffixArray, rankOf[i] is the position of suffix i in SA. */
void buildLcp(void) {
    int h = 0;
    lcp[0] = 0;
    for (int i = 0; i < n; i++) {
        if (rankOf[i] > 0) {
            int j = suffixArray[rankOf[i] - 1];
            while (i + h < n && j + h < n && text[i + h] == text[j + h]) h++;
            lcp[rankOf[i]] = h;
            if (h > 0) h--;
        } else {
            h = 0;
        }
    }
}

/* First SA index whose suffix compares >= pattern (or > when strict). */
int bound(const char *pattern, int length, int strict) {
    int low = 0;
    int high = n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        int cmp = strncmp(text + suffixArray[mid], pattern, length);
        if (cmp < 0 || (strict && cmp == 0)) low = mid + 1;
        else high = mid;
    }
    return low;
}

int compareInt(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

int main(void) {
    static char pattern[MAX_LEN + 2];
    static int positions[MAX_LEN];
    int q;

    printf("Suffix Array + LCP (Kasai)\n\n");
    printf("Text (1-%d characters, no spaces): ", MAX_LEN);
    if (scanf("%10001s", text) != 1 || strlen(text) > MAX_LEN) {
        printf("Invalid text\n");
        return 1;
    }
    n = (int)strlen(text);
    buildSuffixArray();
    buildLcp();

    if (n <= TRACE_LIMIT) {
        printf("\n   i  SA[i]  LCP[i]  suffix\n");
        for (int i = 0; i < n; i++) {
            printf("  %2d   %3d    %3d    %s\n", i, suffixArray[i], lcp[i],
                   text + suffixArray[i]);
        }
    }

    long long lcpSum = 0;
    int best = 0;
    for (int i = 1; i < n; i++) {
        lcpSum += lcp[i];
        if (lcp[i] > lcp[best]) best = i;
    }
    long long total = (long long)n * (n + 1) / 2;
    printf("\nDistinct substrings: %lld - %lld = %lld\n", total, lcpSum,
           total - lcpSum);
    if (lcp[best] == 0) {
        printf("Longest repeated substring: none\n");
    } else {
        printf("Longest repeated substring: \"%.*s\" (length %d)\n", lcp[best],
               text + suffixArray[best], lcp[best]);
    }

    printf("\nHow many pattern queries (0-%d)? ", MAX_QUERIES);
    if (scanf("%d", &q) != 1 || q < 0 || q > MAX_QUERIES) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        printf("Pattern %d: ", i + 1);
        if (scanf("%10001s", pattern) != 1 || strlen(pattern) > MAX_LEN) {
            printf("Invalid pattern\n");
            return 1;
        }
        int length = (int)strlen(pattern);
        int first = bound(pattern, length, 0);
        int last = bound(pattern, length, 1);
        int count = last - first;
        printf("\"%s\": %d occurrence%s", pattern, count, count == 1 ? "" : "s");
        if (count > 0) {
            for (int j = 0; j < count; j++) positions[j] = suffixArray[first + j];
            qsort(positions, count, sizeof(int), compareInt);
            printf(" at");
            for (int j = 0; j < count && j < MAX_LISTED; j++) {
                printf(" %d", positions[j]);
            }
            if (count > MAX_LISTED) printf(" ...");
        }
        printf("\n");
    }
    return 0;
}
