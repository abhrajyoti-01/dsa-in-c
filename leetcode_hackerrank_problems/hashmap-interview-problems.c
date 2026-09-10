/*
Hash Map Interview Problems - Two Sum, Longest Consecutive, Anagrams,
Subarray Sum, Duplicates (Intermediate -> Advanced)
---------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 1   - Two Sum (unsorted, the canonical hash map answer)
  LeetCode 128 - Longest Consecutive Sequence (O(n) with a hash set)
  LeetCode 49  - Group Anagrams
  LeetCode 560 - Subarray Sum Equals K (prefix sums + counts)
  LeetCode 217 - Contains Duplicate
  HackerRank "Pairs" / "Count Triplets" style dictionary tasks

PROBLEM DEFINITION:
Five problems that look different but all replace a nested loop with a
hash map: "have I seen this value before?", "how many times?", "which
group does it belong to?". The map turns an O(n^2) scan into O(n).

REAL-LIFE EXAMPLE:
Duplicate invoice detection (found before?), customer grouping by
identical purchase signature (anagrams), and "how many contiguous
batches made exactly this revenue?" (subarray sums).

DIAGRAM (Two Sum on [2,7,11,15] for 9):
  index 0 value 2 -> need 7, map empty -> store 2
  index 1 value 7 -> need 2, FOUND in map at index 0 -> (0,1)

  Longest consecutive on [100,4,200,1,3,2]:
  set = {1,2,3,4,100,200}
  only start counting at values with no predecessor:
    1 (0 absent) -> 1,2,3,4 -> length 4     <- best
    100 (99 absent) -> length 1
    200 (199 absent) -> length 1

  Subarray sum k=3 on [1,2,1,2]: prefix sums 1 3 4 6
    prefix 3 with need 0 (count 1) -> [1,2]
    prefix 4 with need 1 (count 1) -> [2,1]
    prefix 6 with need 3 (count 1) -> [1,2]  (the second window)
    total 3 subarrays

PSEUDO CODE:
  TwoSum(a, target): for each value, if target - value is in the map
      return the stored index; else store value -> index
  LongestConsecutive(a): put all in a set; for each x with x-1 absent,
      extend while x+1, x+2, ... are present; track the best length
  GroupAnagrams(words): canonical key = letters sorted (counting
      signature); group in a map from key to the first representative
  SubarraySum(a, k): running prefix; count += counts[prefix - k];
      then counts[prefix]++
  ContainsDuplicate(a): set membership test

SAMPLE INPUT & DRY RUN:
  4
  2 7 11 15
  9
  6
  100 4 200 1 3 2
  6
  eat tea ate cat act tac
  4
  1 2 1 2
  3
  Two Sum 9 -> indices (0,1). Longest consecutive = 4.
  Anagrams of "eat tea ate cat act tac" -> two groups:
  {ate, eat, tea} and {act, cat, tac}.
  Subarray sum 3 -> 3 subarrays (traced in the diagram).

SPECIAL CASES:
  - Two Sum duplicates: [3,3] with target 6 must use two DIFFERENT
    indices - storing first and only returning a match keeps that true.
  - Negative values in Two Sum and subarray sums are fine; longest
    consecutive requires INTEGER values (neighbour +1) - both handled.
  - Subarray sum needs counts[0] = 1 before the scan (the empty
    prefix), otherwise subarrays starting at index 0 are missed.
  - Ties in longest consecutive: length is unique even if the sequence
    is not.
  - Values <= 1000 elements, within +/-10^9, words <= 200 of length 20.

COMPLEXITY:
  Expected O(n) time and O(n) space per part with an open-addressing
  table. Group anagrams sorts the words by their 26-letter counting
  signature: O(n log n * 26) time, which is the "canonical key" method
  the pseudocode describes (comparing every pair instead would be the
  slower O(n^2 * 26) variant).
  The longest-consecutive scan is O(n) amortised: the inner extension
  runs only for values with no predecessor, so each stored value is
  visited O(1) times overall.

TIPS:
  The phrase "count how many ... exactly k" usually means prefix sums
  plus a frequency map. The phrase "have I seen" means a plain set.
  Say the expected O(n) out loud and mention worst-case O(n^2) hashing
  collisions - interviewers ask about it.
  Range-check every coordinate BEFORE subtracting: target and prefix
  sums here are long long precisely because target - value can reach
  +/-3 * 10^9, which does not fit in int.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000
#define MAX_WORDS 200
#define MAX_WORD 20
#define TABLE_SIZE 4096
#define VALUE_LIMIT 1000000000LL

/* Open-addressing hash table keyed by long long. */
typedef struct {
    long long keys[TABLE_SIZE];
    int counts[TABLE_SIZE];
    int used[TABLE_SIZE];
} LongTable;

LongTable longTable;

unsigned long long hashLong(long long value) {
    unsigned long long x = (unsigned long long)value;
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return x;
}

void clearLongTable(void) {
    memset(longTable.used, 0, sizeof(longTable.used));
    memset(longTable.counts, 0, sizeof(longTable.counts));
}

int* findOrInsertLong(long long key) {
    size_t index = (size_t)(hashLong(key) % TABLE_SIZE);
    while (longTable.used[index] && longTable.keys[index] != key) {
        index = (index + 1) % TABLE_SIZE;
    }
    if (!longTable.used[index]) {
        longTable.used[index] = 1;
        longTable.keys[index] = key;
        longTable.counts[index] = 0;
    }
    return &longTable.counts[index];
}

int countOfLong(long long key) {
    size_t index = (size_t)(hashLong(key) % TABLE_SIZE);
    while (longTable.used[index]) {
        if (longTable.keys[index] == key) return longTable.counts[index];
        index = (index + 1) % TABLE_SIZE;
    }
    return 0;
}

long long values[MAX_N];
char words[MAX_WORDS][MAX_WORD + 1];
int order[MAX_WORDS];
unsigned char signatureOf[MAX_WORDS][26];

/* Compares two words by their 26-letter counting signature, then by the
 * text itself so the sort is deterministic. This turns grouping into a
 * single O(n log n) sort instead of an O(n^2) pairwise comparison. */
int compareSignatures(const void* a, const void* b) {
    int i = *(const int*)a;
    int j = *(const int*)b;
    for (int c = 0; c < 26; c++) {
        if (signatureOf[i][c] != signatureOf[j][c]) {
            return signatureOf[i][c] < signatureOf[j][c] ? -1 : 1;
        }
    }
    return strcmp(words[i], words[j]);
}

/* Grouping must compare ONLY the 26 counts: the qsort comparator above
 * adds a strcmp tie-break so that the sort is deterministic, which
 * makes it non-zero for two anagrams and therefore useless for asking
 * "same group?". Keeping the two questions separate is the bug fix. */
int sameSignature(int i, int j) {
    for (int c = 0; c < 26; c++) {
        if (signatureOf[i][c] != signatureOf[j][c]) return 0;
    }
    return 1;
}

int main(void) {
    int n;

    printf("Hash Map Interview Problems\n\n");
    printf("== Two Sum: how many values (2-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 2 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 ||
            values[i] < -VALUE_LIMIT || values[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    long long target;
    /* values are within +/-10^9, so a sum of two of them fits in
     * +/-2*10^9; anything beyond that can never be formed and would
     * overflow the need = target - value subtraction if unchecked. */
    printf("Target sum (within +/-2*10^9): ");
    if (scanf("%lld", &target) != 1 ||
        target < -2 * VALUE_LIMIT || target > 2 * VALUE_LIMIT) {
        printf("Invalid target\n");
        return 1;
    }
    clearLongTable();
    int found = 0;
    for (int i = 0; i < n && !found; i++) {
        long long need = target - values[i];
        int index = countOfLong(need);
        if (index != 0) {
            printf("Two Sum: values[%d] + values[%d] = %lld + %lld = %lld\n",
                   index - 1, i, values[index - 1], values[i], target);
            found = 1;
        }
        *findOrInsertLong(values[i]) = i + 1;   /* store index + 1 */
    }
    if (!found) printf("Two Sum: no pair sums to %lld\n", target);

    printf("\n== Longest consecutive sequence: how many values (1-%d)? ",
           MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 ||
            values[i] < -VALUE_LIMIT || values[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    clearLongTable();
    int distinct = 0;
    for (int i = 0; i < n; i++) {
        int* seen = findOrInsertLong(values[i]);
        if (*seen == 0) distinct++;
        *seen += 1;
    }
    int bestRun = 0;
    long long bestStart = 0;
    for (int i = 0; i < n; i++) {
        if (countOfLong(values[i] - 1) != 0) continue;
        int length = 1;
        long long next = values[i] + 1;
        while (countOfLong(next) != 0) {
            length++;
            next++;
        }
        if (length > bestRun) {
            bestRun = length;
            bestStart = values[i];
        }
    }
    printf("Longest consecutive run = %d starting at %lld\n", bestRun,
           bestStart);
    printf("Distinct values = %d, contains duplicates = %s\n", distinct,
           distinct < n ? "yes (LeetCode 217)" : "no (LeetCode 217)");

    printf("\n== Group anagrams: how many words (1-%d)? ", MAX_WORDS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_WORDS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d lowercase words (length 1..%d):\n", n, MAX_WORD);
    for (int i = 0; i < n; i++) {
        if (scanf("%20s", words[i]) != 1) {
            printf("Invalid word\n");
            return 1;
        }
        for (int j = 0; words[i][j] != '\0'; j++) {
            if (words[i][j] < 'a' || words[i][j] > 'z') {
                printf("Invalid word: use lowercase letters only\n");
                return 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < 26; c++) signatureOf[i][c] = 0;
        for (int j = 0; words[i][j] != '\0'; j++) {
            signatureOf[i][words[i][j] - 'a']++;
        }
        order[i] = i;
    }
    qsort(order, (size_t)n, sizeof(order[0]), compareSignatures);
    int groups = 0;
    printf("Anagram groups (sorted by canonical signature):\n");
    for (int i = 0; i < n;) {
        int j = i + 1;
        while (j < n && sameSignature(order[i], order[j])) j++;
        groups++;
        printf("  group %d:", groups);
        for (int k = i; k < j; k++) printf(" %s", words[order[k]]);
        printf("\n");
        i = j;
    }
    printf("Total groups = %d\n", groups);

    printf("\n== Subarray sum equals k: reuse the same array length (1-%d): ",
           MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &values[i]) != 1 ||
            values[i] < -VALUE_LIMIT || values[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    long long k;
    printf("k (within +/-10^12): ");
    if (scanf("%lld", &k) != 1 || k < -1000000000000LL ||
        k > 1000000000000LL) {
        printf("Invalid k\n");
        return 1;
    }
    clearLongTable();
    *findOrInsertLong(0) = 1;               /* the empty prefix */
    long long prefix = 0;
    long long subarrays = 0;
    for (int i = 0; i < n; i++) {
        prefix += values[i];
        subarrays += countOfLong(prefix - k);
        (*findOrInsertLong(prefix))++;
    }
    printf("Subarrays with sum %lld = %lld\n", k, subarrays);
    printf("\nDone: every nested loop replaced by one hash lookup.\n");
    return 0;
}
