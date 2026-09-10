/*
DP Interview Problems - Decode Ways, Word Break, Coin Change, Perfect
Squares, LIS in O(n log n), Wildcard Matching (Intermediate -> Master)
--------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 91  - Decode Ways
  LeetCode 139 - Word Break
  LeetCode 322 - Coin Change (minimum coins)
  LeetCode 279 - Perfect Squares
  LeetCode 300 - Longest Increasing Subsequence (the O(n log n) version)
  LeetCode 44  - Wildcard Matching ('?' and '*')
  HackerRank "The Maximum Subarray" family and "Coin Change" problems

PROBLEM DEFINITION:
Six interview classics that each add ONE new DP idea: string
segmentation, dictionary backtracking with memoisation, unbounded
coin choice, composing squares, patience sorting with binary search,
and a two-dimensional string pattern match with backtracking stars.

REAL-LIFE EXAMPLE:
Message decoding in telecom protocols, grammar checking with a
dictionary, vending-machine change, layout packing in squares, and
glob-style file matching in a shell are these six programs.

DIAGRAM (decode ways for "226"):
  dp[0] = 1 (empty string)
  dp[1]: "2" alone            -> 1
  dp[2]: "2"+"2" and "22"     -> 2
  dp[3]: dp[2] + dp[1] (6 and 26 are both valid) -> 3
  answers: 2-2-6, 22-6, 2-26

  LIS of [10,9,2,5,3,7,101,18] with patience tails:
  tails after each element: 10 | 9 | 2 | 2 5 | 2 3 | 2 3 7 | 2 3 7 101 |
                            2 3 7 18        -> length 4

PSEUDO CODE:
  DecodeWays(s): dp[i] = dp[i-1] * (one digit valid) +
                          dp[i-2] * (two digits 10..26)
  WordBreak(s, dict): dp[0] = true; dp[i] |= dp[j] && s[j..i-1] in dict
  CoinChange(coins, amount): dp[0] = 0; dp[v] = min(dp[v], dp[v-coin]+1)
  PerfectSquares(n): dp[i] = min over squares j*j <= i of dp[i-j*j]+1
  LIS(a): tails[] = lower_bound insertion (strictly increasing)
  Wildcard(s, p): dp[i][j] with '?' matching one char and '*' matching
      zero or more of the previous pattern characters

SAMPLE INPUT & DRY RUN:
  226                     <- decode this digit string -> 3 ways
  3                       <- dictionary size
  cat cats and            <- the dictionary (lowercase words)
  catcats                 <- target string -> breakable ("cat"+"cats")
  3                       <- coin denominations
  1 5 6                   <- the coins
  11                      <- amount -> 6 + 5 = 2 coins
  8                       <- LIS input size
  10 9 2 5 3 7 101 18     <- LIS -> length 4
  ab*cd                   <- wildcard pattern (no leading count line!)
  abbbcd                  <- text -> matches ("ab*cd" -> "ab"+"bb"+"cd")
  There is NO count line before the pattern/text pair: the wildcard
  section reads exactly two tokens, pattern first and then the text.
  "226" -> 3 ways (diagram). coins 1,5,6 amount 11 -> 6+5 = 2 coins.
  LIS -> length 4. "ab*cd" matches "abbbcd" -> yes.

SPECIAL CASES:
  - Decode Ways: a leading '0' is invalid; "10" and "20" decode only
    as pairs; "27" cannot be a pair. "0" alone has 0 ways.
  - Decode Ways counts grow like Fibonacci, so the DIGIT COUNT is
    capped at 90: the 91-digit count exceeds 2^64. The result is
    printed as unsigned long long (not int, which the earlier draft
    overflowed after ~45 digits).
  - Word Break: empty string is true with an empty dictionary; a word
    longer than the string can never match; the word list must be
    lowercase to match the lowercase input.
  - Coin Change: unreachable amounts must report "impossible", not 0.
  - LIS is STRICTLY increasing (use upper_bound for the non-strict
    variant - a one-line change worth stating).
  - Wildcard Matching: "*" at the start and repeated "**" both work;
    '?' never matches an empty position; the empty pattern matches
    only the empty string.
  - text <= 1000 characters, coins <= 20, amount <= 5000, n <= 1000.

COMPLEXITY:
  Decode/CoinChange/Squares O(n) or O(n * coins); Word Break
  O(n^2 * word length); LIS O(n log n); Wildcard O(n * m) time and
  O(m) space with a rolling row.

TIPS:
  Write the DP definition as a sentence first ("dp[i] = the number of
  ways to decode the first i characters"). If the sentence needs an
  "and", the recurrence is probably right; if it needs "or", it is a
  minimisation or counting table.
*/

#include <stdio.h>
#include <string.h>

#define MAX_TEXT 1000
#define MAX_WORDS 100
#define MAX_WORD 30
#define MAX_COINS 20
#define MAX_AMOUNT 5000
#define MAX_N 1000
#define MAX_PATTERN 100
/* Decode-ways counts grow like Fibonacci, so the count itself - not the
 * string - is the limiting factor: countBits(93 digits) is the largest
 * value that still fits in unsigned long long. Longer digit strings are
 * rejected rather than silently overflowing an int. */
#define MAX_DECODE_DIGITS 90

char text[MAX_TEXT + 1];
char pattern[MAX_PATTERN + 1];
char words[MAX_WORDS][MAX_WORD + 1];
long long value[MAX_N];
int coin[MAX_COINS];
int dpTable[MAX_AMOUNT + 1];
int wildcardRow[MAX_PATTERN + 1];
int wildcardPrev[MAX_PATTERN + 1];

/* Returns the number of decodings, or 0 when a leading zero makes the
 * string undecodable. Fits in unsigned long long for n <= 90. */
unsigned long long decodeWays(const char* s, int n) {
    if (n > 0 && s[0] == '0') return 0;
    unsigned long long previous2 = 1;
    unsigned long long previous1 = (n > 0) ? 1 : 0;
    for (int i = 2; i <= n; i++) {
        unsigned long long current = 0;
        if (s[i - 1] != '0') current += previous1;
        int two = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
        if (two >= 10 && two <= 26) current += previous2;
        previous2 = previous1;
        previous1 = current;
    }
    return previous1;
}

int wordBreak(int n, int wordCount) {
    char reachable[MAX_TEXT + 1];
    memset(reachable, 0, sizeof(reachable));
    reachable[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i && !reachable[i]; j++) {
            if (!reachable[j]) continue;
            int width = i - j;
            for (int w = 0; w < wordCount; w++) {
                int length = (int)strlen(words[w]);
                if (length == width && strncmp(text + j, words[w], (size_t)width) == 0) {
                    reachable[i] = 1;
                    break;
                }
            }
        }
    }
    return reachable[n];
}

int coinChange(int n, int amount) {
    const int impossible = 1000000;
    for (int i = 0; i <= amount; i++) dpTable[i] = impossible;
    dpTable[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int v = coin[i]; v <= amount; v++) {
            if (dpTable[v - coin[i]] + 1 < dpTable[v]) {
                dpTable[v] = dpTable[v - coin[i]] + 1;
            }
        }
    }
    return dpTable[amount] == impossible ? -1 : dpTable[amount];
}

int perfectSquares(int n) {
    for (int i = 0; i <= n; i++) dpTable[i] = i;   /* i ones */
    for (int base = 2; base * base <= n; base++) {
        int square = base * base;
        for (int v = square; v <= n; v++) {
            if (dpTable[v - square] + 1 < dpTable[v]) {
                dpTable[v] = dpTable[v - square] + 1;
            }
        }
    }
    return dpTable[n];
}

int longestIncreasingSubsequence(int n) {
    int tails[MAX_N];
    int length = 0;
    for (int i = 0; i < n; i++) {
        int lo = 0;
        int hi = length;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < (int)value[i]) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        tails[lo] = (int)value[i];
        if (lo == length) length++;
    }
    return length;
}

int wildcardMatch(const char* s, int n, const char* p, int m) {
    wildcardPrev[0] = 1;
    for (int j = 1; j <= m; j++) {
        wildcardPrev[j] = wildcardPrev[j - 1] && (p[j - 1] == '*');
    }
    for (int i = 1; i <= n; i++) {
        wildcardRow[0] = 0;
        for (int j = 1; j <= m; j++) {
            if (p[j - 1] == '*') {
                wildcardRow[j] = wildcardRow[j - 1] || wildcardPrev[j];
            } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                wildcardRow[j] = wildcardPrev[j - 1];
            } else {
                wildcardRow[j] = 0;
            }
        }
        memcpy(wildcardPrev, wildcardRow, sizeof(int) * (size_t)(m + 1));
    }
    return wildcardPrev[m];
}

int main(void) {
    int n;

    printf("DP Interview Problems\n\n");
    printf("== Decode Ways. Enter a digit string (1..%d chars): ", MAX_TEXT);
    if (scanf("%1000s", text) != 1) {
        printf("Invalid string\n");
        return 1;
    }
    n = (int)strlen(text);
    if (n > MAX_DECODE_DIGITS) {
        printf("Invalid string: at most %d digits are supported so that "
               "the count fits in %d bits\n", MAX_DECODE_DIGITS, 64);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (text[i] < '0' || text[i] > '9') {
            printf("Invalid digit string\n");
            return 1;
        }
    }
    printf("Decode ways for \"%s\" = %llu\n", text, decodeWays(text, n));

    printf("\n== Word Break. How many dictionary words (1-%d)? ", MAX_WORDS);
    int wordCount;
    if (scanf("%d", &wordCount) != 1 || wordCount < 1 ||
        wordCount > MAX_WORDS) {
        printf("Invalid word count\n");
        return 1;
    }
    printf("Enter %d lowercase words (length 1..%d):\n", wordCount, MAX_WORD);
    for (int i = 0; i < wordCount; i++) {
        if (scanf("%30s", words[i]) != 1) {
            printf("Invalid word\n");
            return 1;
        }
        for (int j = 0; words[i][j] != '\0'; j++) {
            if (words[i][j] < 'a' || words[i][j] > 'z') {
                printf("Invalid word: lowercase letters only\n");
                return 1;
            }
        }
    }
    printf("Enter the target string (lowercase, 1..%d chars): ", MAX_TEXT);
    if (scanf("%1000s", text) != 1) {
        printf("Invalid string\n");
        return 1;
    }
    n = (int)strlen(text);
    for (int i = 0; i < n; i++) {
        if (text[i] < 'a' || text[i] > 'z') {
            printf("Invalid string: lowercase letters only\n");
            return 1;
        }
    }
    printf("Word break of \"%s\" = %s\n", text,
           wordBreak(n, wordCount) ? "possible" : "impossible");

    printf("\n== Coin Change. How many coin denominations (1-%d)? ",
           MAX_COINS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_COINS) {
        printf("Invalid coin count\n");
        return 1;
    }
    printf("Enter %d positive denominations:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &coin[i]) != 1 || coin[i] < 1 ||
            coin[i] > MAX_AMOUNT) {
            printf("Invalid denomination\n");
            return 1;
        }
    }
    int amount;
    printf("Amount (1..%d): ", MAX_AMOUNT);
    if (scanf("%d", &amount) != 1 || amount < 1 || amount > MAX_AMOUNT) {
        printf("Invalid amount\n");
        return 1;
    }
    int coinsUsed = coinChange(n, amount);
    if (coinsUsed < 0) {
        printf("Amount %d is impossible with these coins\n", amount);
    } else {
        printf("Minimum coins for %d = %d\n", amount, coinsUsed);
    }
    printf("Perfect squares needed for %d = %d\n", amount,
           perfectSquares(amount));

    printf("\n== Longest Increasing Subsequence. How many values (1-%d)? ",
           MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &value[i]) != 1 ||
            value[i] < -1000000000LL || value[i] > 1000000000LL) {
            printf("Invalid value\n");
            return 1;
        }
    }
    printf("LIS length (O(n log n) patience method) = %d\n",
           longestIncreasingSubsequence(n));

    printf("\n== Wildcard Matching. Enter a lowercase pattern (<=%d chars,\n",
           MAX_PATTERN);
    printf("'?' = one character, '*' = any sequence): ");
    if (scanf("%100s", pattern) != 1) {
        printf("Invalid pattern\n");
        return 1;
    }
    printf("Enter the lowercase text to match (<=%d chars): ", MAX_TEXT);
    if (scanf("%1000s", text) != 1) {
        printf("Invalid text\n");
        return 1;
    }
    int m = (int)strlen(pattern);
    n = (int)strlen(text);
    for (int i = 0; i < m; i++) {
        if (pattern[i] != '?' && pattern[i] != '*' &&
            (pattern[i] < 'a' || pattern[i] > 'z')) {
            printf("Invalid pattern character\n");
            return 1;
        }
    }
    printf("\"%s\" matches \"%s\" = %s\n", pattern, text,
           wildcardMatch(text, n, pattern, m) ? "yes" : "no");
    return 0;
}
