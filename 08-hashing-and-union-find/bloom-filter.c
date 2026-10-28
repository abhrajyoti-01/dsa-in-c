/*
Bloom Filter - Probabilistic Set Membership in a Few Bits (Intermediate -> Advanced)
------------------------------------------------------------------------------------
PROBLEM DEFINITION:
Store a set of words in an array of m bits so that the question "is
word w in the set?" is answered with either "DEFINITELY NOT" (always
correct) or "PROBABLY YES" (occasionally wrong). Each word sets k bits
chosen by k hash functions; a query checks those same k bits. There
are no false negatives, only false positives - and the filter never
stores the words themselves.

REAL-LIFE EXAMPLE:
A browser keeps a compact filter of known malicious URLs: if the filter
says "definitely not", the page loads instantly; only "probably yes"
answers trigger a slow server lookup. Databases (Cassandra, HBase,
RocksDB) check a Bloom filter before reading a file from disk.

DIAGRAM (m = 16 bits, k = 2, illustrative positions):
  insert "cat": h(cat) -> 3, 11       bits: 0001000000010000
  insert "dog": h(dog) -> 6, 11       bits: 0001001000010000
  query  "cow": h(cow) -> 3, 6   -> both set -> "probably yes" but cow
                was never inserted: a FALSE POSITIVE made of other
                words' bits.
  query  "pig": h(pig) -> 2, 11  -> bit 2 is 0 -> "definitely not".

PSEUDO CODE:
  h1 = FNV-1a(word), h2 = djb2(word) forced odd
  Position(i) = (h1 + i * h2) mod m        for i = 0..k-1  (double hashing)
  Insert(w): set bit Position(i) for every i
  Query(w):  if any bit Position(i) is 0: "definitely not"
             else "probably yes"
  Estimated false-positive rate = (bits set / m)^k

SAMPLE INPUT & DRY RUN:
  32 3
  5
  apple banana cherry grape mango
  6
  apple kiwi fig mango orange lemon
  insert: apple -> 31 22 13, banana -> 16 23 30, cherry -> 24 11 30,
          grape -> 16 5 26,  mango -> 21 12 3
  bits:   00010100000111001000011110100011  (13 of 32 set)
  estimate (13/32)^3 = 6.7% false positives
  apple  31 22 13: all set              -> probably yes (correct)
  kiwi   25 18 11: bit 25 is 0          -> DEFINITELY NOT
  fig    21 16 11: set by mango, banana/grape, cherry -> FALSE POSITIVE
  mango  21 12 3:  all set              -> probably yes (correct)
  orange 11 12 13: set by cherry, mango, apple        -> FALSE POSITIVE
  lemon  16 17 18: bit 17 is 0          -> DEFINITELY NOT
  False positives: 2 of 4 queries for absent words (the filter is
  tiny on purpose; with m = 4096 they disappear).

SPECIAL CASES:
  - A word inserted twice changes nothing (its bits are already set).
  - Deleting is impossible: clearing a bit may erase other words (use a
    counting Bloom filter for deletions).
  - k = 1 is a plain bitmap hash; too many hashes fill the filter fast.
    The best k is about (m / n) * ln 2.
  - The program keeps the real word list ONLY to label answers as true
    or false positives for teaching; a real filter cannot do that.
  - 8 <= m <= 4096 bits, 1 <= k <= 8, up to 500 words of <= 31 chars.

COMPLEXITY:
  O(k * L) per insert or query for words of length L; m bits of space,
  independent of how long the words are.

TIPS:
  The trade-off is memory versus false-positive rate: about 9.6 bits
  per element gives 1% errors. Double hashing (h1 + i*h2) makes k hash
  functions out of two. Compare hash-table, which is exact but stores
  every key.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_BITS 4096
#define MAX_HASHES 8
#define MAX_WORDS 500
#define WORD_LENGTH 31

unsigned char bits[MAX_BITS];
char inserted[MAX_WORDS][WORD_LENGTH + 1];
int bitCount;
int hashCount;
int insertedCount;

uint32_t fnv1a(const char *word) {
    uint32_t hash = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)word; *p; p++) {
        hash ^= *p;
        hash *= 16777619u;
    }
    return hash;
}

uint32_t djb2(const char *word) {
    uint32_t hash = 5381u;
    for (const unsigned char *p = (const unsigned char *)word; *p; p++) {
        hash = hash * 33u + *p;
    }
    return hash;
}

int position(uint32_t h1, uint32_t h2, int i) {
    return (int)((h1 + (uint32_t)i * h2) % (uint32_t)bitCount);
}

void insertWord(const char *word) {
    uint32_t h1 = fnv1a(word);
    uint32_t h2 = djb2(word) | 1u;
    printf("  insert %-12s -> bits", word);
    for (int i = 0; i < hashCount; i++) {
        int p = position(h1, h2, i);
        bits[p] = 1;
        printf(" %d", p);
    }
    printf("\n");
}

int queryWord(const char *word) {
    uint32_t h1 = fnv1a(word);
    uint32_t h2 = djb2(word) | 1u;
    int allSet = 1;
    printf("  query  %-12s -> bits", word);
    for (int i = 0; i < hashCount; i++) {
        int p = position(h1, h2, i);
        printf(" %d%s", p, bits[p] ? "" : "(0)");
        if (!bits[p]) allSet = 0;
    }
    return allSet;
}

int wasInserted(const char *word) {
    for (int i = 0; i < insertedCount; i++) {
        if (strcmp(inserted[i], word) == 0) return 1;
    }
    return 0;
}

int readWord(char *word) {
    char buffer[64];
    if (scanf("%63s", buffer) != 1 || strlen(buffer) > WORD_LENGTH) return 0;
    strcpy(word, buffer);
    return 1;
}

int main(void) {
    int queryCount;
    char word[WORD_LENGTH + 1];

    printf("Bloom Filter - Probabilistic Membership\n\n");
    printf("Number of bits m (8-%d) and hash functions k (1-%d): ", MAX_BITS,
           MAX_HASHES);
    if (scanf("%d %d", &bitCount, &hashCount) != 2 || bitCount < 8 ||
        bitCount > MAX_BITS || hashCount < 1 || hashCount > MAX_HASHES) {
        printf("Invalid filter size\n");
        return 1;
    }
    printf("How many words to insert (1-%d)? ", MAX_WORDS);
    if (scanf("%d", &insertedCount) != 1 || insertedCount < 1 ||
        insertedCount > MAX_WORDS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d words (<= %d chars each):\n", insertedCount, WORD_LENGTH);
    for (int i = 0; i < insertedCount; i++) {
        if (!readWord(inserted[i])) {
            printf("Invalid word\n");
            return 1;
        }
    }
    printf("\n");
    for (int i = 0; i < insertedCount; i++) insertWord(inserted[i]);

    int setBits = 0;
    for (int i = 0; i < bitCount; i++) setBits += bits[i];
    if (bitCount <= 64) {
        printf("\n  bit array: ");
        for (int i = 0; i < bitCount; i++) printf("%d", bits[i]);
        printf("\n");
    }
    double fill = (double)setBits / bitCount;
    double estimate = 1.0;
    for (int i = 0; i < hashCount; i++) estimate *= fill;
    printf("  %d of %d bits set; estimated false-positive rate %.1f%%\n\n",
           setBits, bitCount, estimate * 100.0);

    printf("How many queries (1-%d)? ", MAX_WORDS);
    if (scanf("%d", &queryCount) != 1 || queryCount < 1 ||
        queryCount > MAX_WORDS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("\n");
    int falsePositives = 0;
    int absentQueries = 0;
    for (int i = 0; i < queryCount; i++) {
        if (!readWord(word)) {
            printf("Invalid word\n");
            return 1;
        }
        int probablyYes = queryWord(word);
        int truth = wasInserted(word);
        if (!truth) absentQueries++;
        if (!probablyYes) {
            printf("  : DEFINITELY NOT\n");
        } else if (truth) {
            printf("  : probably yes (correct)\n");
        } else {
            printf("  : probably yes - FALSE POSITIVE\n");
            falsePositives++;
        }
    }
    printf("\nFalse positives: %d of %d queries for absent words\n",
           falsePositives, absentQueries);
    return 0;
}
