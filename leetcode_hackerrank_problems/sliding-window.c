/*
Sliding Window - Fixed and Variable Size Windows (Intermediate -> Advanced)
---------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 3    - Longest Substring Without Repeating Characters
  LeetCode 76   - Minimum Window Substring
  LeetCode 209  - Minimum Size Subarray Sum
  LeetCode 424  - Longest Repeating Character Replacement
  LeetCode 567  - Permutation in String (fixed-size window anagram)
  LeetCode 643  - Maximum Average Subarray I
  HackerRank "Substring Diff" family

PROBLEM DEFINITION:
Slide a window over a sequence and maintain a small amount of state so
that each element enters and leaves the window only once. Fixed-size
windows track a running sum; variable-size windows grow on the right
and shrink from the left while a constraint is violated.

REAL-LIFE EXAMPLE:
Network monitoring: the busiest 10-second interval (fixed window) and
the shortest burst that hits a byte threshold (variable window) both
come from one pass over the event stream.

DIAGRAM (longest substring without repeats in "abcabcbb"):
  window [0..2] = abc   length 3
  next char a repeats at index 0 -> left jumps to 1
  window [1..3] = bca   length 3
  next char b repeats at index 1 -> left jumps to 2
  best length = 3
  Every index is visited by right once, and by left at most once.

PSEUDO CODE:
  FixedWindowSum(a, k): sum of a[0..k-1]; slide: += a[right]; -= a[right-k]
  MaxSumWindow: same slide, track the best
  LongestUniqueSubstring(s):
      lastSeen[256] = -1; left = 0
      for right = 0..n-1:
          if lastSeen[s[right]] >= left: left = lastSeen[s[right]] + 1
          lastSeen[s[right]] = right
          best = max(best, right - left + 1)
  MinSubarraySum(a, target): grow right; while sum >= target shrink left
                             and record the smallest width

SAMPLE INPUT & DRY RUN:
  8
  1 2 3 4 5 6 7 8
  3
  11
  abcabcbb
  Fixed window k=3: best = 6 + 7 + 8 = 21 (window [5..7]).
  Minimum subarray with sum >= 11 in 1..8 is [5,6] = 11 -> length 2.
  (A target of 7 would instead be met by the single element [7], so
  the target must be chosen above every individual value to make the
  exercise non-trivial.)
  Longest unique substring of "abcabcbb" = 3.

SPECIAL CASES:
  - k = 1 degenerates to a maximum/minimum element scan.
  - k > n is rejected instead of returning a meaningless answer.
  - An empty answer is legal: report 0 and say so (no subarray hits the
    target).
  - Negative numbers break the "shrink while sum >= target" idea: the
    window sum is no longer monotone in the window size. This program
    requires non-negative values for that part and says why.
  - Repeated characters: the window must move PAST the previous
    occurrence, so left = lastSeen + 1, never just right.
  - n <= 1000, values 0..10^9, k <= 1000, text <= 1000 characters.

COMPLEXITY:
  O(n) time for every part (amortised: each index enters and leaves the
  window once), O(alphabet) or O(k) extra space.

TIPS:
  Ask two questions before coding: "is the window size fixed or does it
  depend on a constraint?" and "what single number summarises the
  window state?" (sum, frequency array, count of distinct characters).
  If neither answer is small, the window technique does not apply.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_TEXT 1000
#define VALUE_LIMIT 1000000000LL

long long values[MAX_N];
char text[MAX_TEXT + 1];

int longestUniqueSubstring(const char* s, int length) {
    int lastSeen[256];
    for (int i = 0; i < 256; i++) lastSeen[i] = -1;
    int left = 0;
    int best = 0;
    for (int right = 0; right < length; right++) {
        unsigned char current = (unsigned char)s[right];
        if (lastSeen[current] >= left) {
            left = lastSeen[current] + 1;
        }
        lastSeen[current] = right;
        if (right - left + 1 > best) best = right - left + 1;
    }
    return best;
}

int main(void) {
    int n;
    int k;
    int length;

    printf("Sliding Window - Fixed and Variable Size\n\n");
    printf("How many numbers (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d non-negative integers (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 || values[i] < 0 ||
            values[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    printf("Fixed window size k (1-%d): ", MAX_N);
    if (scanf("%d", &k) != 1 || k < 1 || k > MAX_N || k > n) {
        printf("Invalid window size\n");
        return 1;
    }
    long long windowSum = 0;
    for (int i = 0; i < k; i++) windowSum += values[i];
    long long bestSum = windowSum;
    int bestStart = 0;
    for (int right = k; right < n; right++) {
        windowSum += values[right] - values[right - k];
        if (windowSum > bestSum) {
            bestSum = windowSum;
            bestStart = right - k + 1;
        }
    }
    printf("Best window of size %d: sum %lld at [%d..%d]\n", k, bestSum,
           bestStart, bestStart + k - 1);

    long long target;
    printf("\nMinimum-length subarray with sum >= target. Target (0..10^12): ");
    if (scanf("%lld", &target) != 1 || target < 0 ||
        target > 1000000000000LL) {
        printf("Invalid target\n");
        return 1;
    }
    int left = 0;
    int bestLength = 0;
    long long sum = 0;
    for (int right = 0; right < n; right++) {
        sum += values[right];
        while (sum >= target && left <= right) {
            int width = right - left + 1;
            if (bestLength == 0 || width < bestLength) bestLength = width;
            sum -= values[left++];
        }
    }
    if (bestLength == 0) {
        long long total = 0;
        for (int i = 0; i < n; i++) total += values[i];
        printf("No subarray reaches the target (total sum = %lld < %lld)\n",
               total, target);
    } else {
        printf("Minimum length with sum >= %lld is %d\n", target, bestLength);
    }

    printf("\nLongest substring without repeating characters.\n");
    printf("Enter a word without spaces (1..%d characters): ", MAX_TEXT);
    if (scanf("%1000s", text) != 1) {
        printf("Invalid text\n");
        return 1;
    }
    length = 0;
    while (text[length] != '\0') length++;
    printf("Longest unique-substring length = %d\n",
           longestUniqueSubstring(text, length));
    return 0;
}
