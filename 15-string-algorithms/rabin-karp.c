/*
Rabin-Karp String Matching
--------------------------
What this program solves:
Finds pattern occurrences using ROLLING HASHES: compare a numeric
hash of the pattern against the hash of each text window; verify a
real match only when hashes agree.

Real life example:
Checking fingerprint cards: instead of reading every document word
by word, stamp each window with a numeric fingerprint. Only when a
window's fingerprint equals the search fingerprint do you read the
actual text to confirm.

DIAGRAM (rolling hash: O(1) per slide):
  hash("abc") = a*26^2 + b*26 + c
  slide: hash("bcd") = (hash - a*26^2) * 26 + d
  hash matches? then compare characters (guard against collisions)

How the process works:
1. Pick base d (=256 for bytes) and a large prime MOD.
2. hash = sum(char * d^(m-1-i)) mod MOD.
3. Slide the window: new hash =
   (old - text[i]*h) * d + text[i+m]  (mod MOD),
   where h = d^(m-1) mod MOD - O(1) per slide.
4. Hash equal -> compare characters to rule out spurious hits.

Pseudo code:
START
p = hash(pattern); t = hash(text[0..m-1]); h = d^(m-1) mod MOD
FOR s = 0..n-m
    IF p == t AND text[s..s+m-1] == pattern: report s
    IF s < n-m: t = ((t - text[s]*h)*d + text[s+m]) mod MOD
END

Sample input and dry run:
text = "GEEKS FOR GEEKS", pattern = "GEEK"
windows hashed one by one; true matches at indices 0 and 10;
any hash collision gets filtered by the character check.

Main logic to understanding:
Average O(n + m) when hashing distributes well; worst O(n*m) if
all windows collide (rare with a large prime). Multi-pattern
search is its real superpower: hash all patterns once.
*/

#include <stdio.h>
#include <string.h>

#define D 256
#define MOD 101

/*
 * recomputeHash - Rolling hash update when window slides right.
 */
long long slideHash(long long oldHash, char outChar, char inChar, long long h) {
    long long v = oldHash - outChar * h;
    v = ((v % MOD) + MOD) % MOD;
    v = (v * D + inChar) % MOD;
    return v;
}

int main() {
    char text[500];
    char pattern[100];

    printf("Rabin-Karp Pattern Matching (hash = %d, mod = %d)\n\n", D, MOD);

    printf("Enter text (max 499 chars): ");
    scanf("%499s", text);

    printf("Enter pattern (max 99 chars): ");
    scanf("%99s", pattern);

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m > n || m == 0) {
        printf("Pattern longer than text or empty\n");
        return 1;
    }

    long long h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * D) % MOD;
    }

    /* STEP: initial hashes of pattern and first window */
    long long pHash = 0;
    long long tHash = 0;
    for (int i = 0; i < m; i++) {
        pHash = (pHash * D + pattern[i]) % MOD;
        tHash = (tHash * D + text[i]) % MOD;
    }

    printf("Pattern hash: %lld\n\n", pHash);

    int count = 0;
    int spurious = 0;

    for (int s = 0; s <= n - m; s++) {
        if (pHash == tHash) {
            int ok = 1;
            for (int j = 0; j < m; j++) {
                if (text[s + j] != pattern[j]) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                printf("Match at index %d (hash %lld verified)\n", s, tHash);
                count = count + 1;
            } else {
                printf("Hash collision at index %d - characters differ\n", s);
                spurious = spurious + 1;
            }
        }
        if (s < n - m) {
            tHash = slideHash(tHash, text[s], text[s + m], h);
        }
    }

    printf("\nTotal matches: %d, spurious hash hits: %d\n", count, spurious);

    return 0;
}
