/*
Two Pointers - Sorted Pairs, Reversal, Palindrome, Merge (Basic -> Intermediate)
--------------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 167 - Two Sum II (Input Array Is Sorted)
  LeetCode 125 - Valid Palindrome
  LeetCode 344 - Reverse String
  LeetCode 977 - Squares of a Sorted Array
  LeetCode 88  - Merge Sorted Array
  HackerRank "Print in Reverse" style traversal drills

PROBLEM DEFINITION:
Solve four classic array problems with index pairs moving toward or
away from each other: find a pair summing to a target, reverse a
sequence in place, test a palindrome, and merge two sorted arrays.

REAL-LIFE EXAMPLE:
Reconciling two sorted bank statements into one timeline (merge), or
checking that a scanned ID code reads the same in both directions
(palindrome) - both are two-pointer sweeps without extra memory.

DIAGRAM (pair sum target 10 on [1,2,4,7,9]):
  lo=0 hi=4: 1+9=10 -> found
  too small -> lo++        too large -> hi--
  [1,2,3,6] target 10: 1+6=7 lo++; 2+6=8 lo++; 3+6=9 lo++ -> none

  reverse:  lo ->                <- hi     swap, step inward

PSEUDO CODE:
  PairSum(a, target): lo=0, hi=n-1
      while lo < hi: sum = a[lo]+a[hi]
          sum == target -> return (lo, hi)
          sum <  target -> lo++
          else          -> hi--
  IsPalindrome(s): compare s[i] with s[n-1-i] moving inward
  Reverse(a): swap a[lo] and a[hi] while lo < hi
  Merge(a, b): compare fronts, take the smaller, then drain remainders

SAMPLE INPUT & DRY RUN:
  6
  1 2 4 7 9 11
  10
  madam
  3
  2 4 6
  Pair 1+9 = 10 at indices 0 and 4.
  "madam" reads the same both ways -> palindrome.
  Merge [1,2,4,7,9,11] and [2,4,6] -> 1 2 2 4 4 6 7 9 11.

SPECIAL CASES:
  - The pair-sum sweep REQUIRES a sorted array; unsorted input is rejected.
  - Negative numbers are fine: only comparisons matter.
  - Empty word or single letter is a palindrome.
  - Equal adjacent values with target 2*value work (lo != hi enforced).
  - n <= 1000, values within +/-10^9, words of <= 1000 letters.

COMPLEXITY:
  Each sweep is O(n) time and O(1) extra space; merging two arrays is
  O(n + m). Compare with the O(n^2) brute-force pair search.

TIPS:
  Two pointers replace nested loops when the data is ORDERED. If the
  array is unsorted, a hash set gives O(n) pair sums - see hash-problems.
  Write the loop invariant down: everything outside [lo..hi] is settled.
*/

#include <stdio.h>

#define MAX_N 1000
#define MAX_M 1000
#define MAX_WORD 1000
#define VALUE_LIMIT 1000000000LL

long long first[MAX_N];
long long second[MAX_M];
long long merged[MAX_N + MAX_M];

int findPairSum(long long a[], int n, long long target, int* left, int* right) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        long long sum = a[lo] + a[hi];
        if (sum == target) {
            *left = lo;
            *right = hi;
            return 1;
        }
        if (sum < target) {
            lo++;
        } else {
            hi--;
        }
    }
    return 0;
}

void reverseArray(long long a[], int n) {
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        long long temp = a[lo];
        a[lo++] = a[hi];
        a[hi--] = temp;
    }
}

int isPalindrome(const char word[], int length) {
    int lo = 0;
    int hi = length - 1;
    while (lo < hi) {
        if (word[lo] != word[hi]) return 0;
        lo++;
        hi--;
    }
    return 1;
}

int mergeSorted(const long long a[], int n, const long long b[], int m,
                long long out[]) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < n && j < m) {
        out[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    }
    while (i < n) out[k++] = a[i++];
    while (j < m) out[k++] = b[j++];
    return k;
}

void printArray(const long long a[], int n) {
    for (int i = 0; i < n; i++) printf(" %lld", a[i]);
    printf("\n");
}

int main(void) {
    int n;
    int m;
    int length;
    long long target;
    char word[MAX_WORD + 1];

    printf("Two Pointers - Pairs, Reversal, Palindrome, Merge\n\n");
    printf("How many SORTED integers (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers in non-decreasing order:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &first[i]) != 1 ||
            first[i] < -VALUE_LIMIT || first[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        if (i > 0 && first[i] < first[i - 1]) {
            printf("Invalid input: array must be sorted\n");
            return 1;
        }
    }
    printf("Target sum (within +/-10^9): ");
    if (scanf("%lld", &target) != 1 ||
        target < -2 * VALUE_LIMIT || target > 2 * VALUE_LIMIT) {
        printf("Invalid target\n");
        return 1;
    }
    int left = -1;
    int right = -1;
    if (findPairSum(first, n, target, &left, &right)) {
        printf("Pair found: %lld + %lld = %lld at indices %d and %d\n",
               first[left], first[right], target, left, right);
    } else {
        printf("No pair sums to %lld\n", target);
    }

    printf("\nReversed array:");
    reverseArray(first, n);
    printArray(first, n);
    reverseArray(first, n);

    printf("Enter a word for the palindrome test (no spaces): ");
    if (scanf("%1000s", word) != 1) {
        printf("Invalid word\n");
        return 1;
    }
    length = 0;
    while (word[length] != '\0') length++;
    printf("\"%s\" is %sa palindrome\n", word,
           isPalindrome(word, length) ? "" : "NOT ");

    printf("\nHow many integers in the second sorted array (0-%d)? ", MAX_M);
    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_M) {
        printf("Invalid count\n");
        return 1;
    }
    if (m > 0) {
        printf("Enter %d integers in non-decreasing order:\n", m);
        for (int i = 0; i < m; i++) {
            if (scanf("%lld", &second[i]) != 1 ||
                second[i] < -VALUE_LIMIT || second[i] > VALUE_LIMIT) {
                printf("Invalid value\n");
                return 1;
            }
            if (i > 0 && second[i] < second[i - 1]) {
                printf("Invalid input: array must be sorted\n");
                return 1;
            }
        }
    }
    int mergedLength = mergeSorted(first, n, second, m, merged);
    printf("Merged (%d values):", mergedLength);
    printArray(merged, mergedLength);
    return 0;
}
