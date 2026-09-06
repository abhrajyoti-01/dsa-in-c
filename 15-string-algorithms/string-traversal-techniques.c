/*
String Traversal Ladder - Char Logic (Basic -> Hard)
-----------------------------------------------------
Real life example:
Password strength meters and input sanitizers scan characters with
exactly these loops - vowels, case flips, digit counts.

DIAGRAM (one pass, many counters):
  "Hello World 123"
  letters 10, digits 3, spaces 2, vowels 3, uppercase 2
  each character examined once with a chain of simple checks.

LADDER (easy -> hard inside this file):
  1. Vowel/consonant counter      (classification)
  2. Toggle case without library  (ASCII arithmetic)
  3. Remove duplicates (keep first occurrence)
  4. Capitalize first letter of each word
  5. Compress runs ("aaabbc" -> "a3b2c1")
  6. Check pangram (all 26 letters present)

BEGINNER EXPLANATION:
Strings are char arrays ending in '\0'. Every "string trick" is
ASCII arithmetic:
- 'a' + 1 == 'b'; lowercase -> uppercase = c - 'a' + 'A'.
- Runs compression = count consecutive equals, print letter+count.
- Pangram = a boolean seen[26] filled by scanning.

SUPER SIMPLE EXAMPLE (run compress "aaabbc"):
a,a,a -> run of 3 -> "a3". b,b -> "b2". c -> "c1". Output: a3b2c1.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Toggle on digits/punctuation: they are unchanged (only letters
  flip) - guard with isalpha-style ranges.
- Run compression of "abc" -> "a1b1c1" which is LONGER - real
  implementations return the original if compression grows it
  (try the extension).
- Remove duplicates: keep FIRST occurrence; case-sensitive here
  ("Aa" keeps both).
- Pangram with uppercase: normalize to lowercase first or the
  seen-array misses half the letters.
- Empty string: all these report zero/empty - the '\0' check at
  loop top handles it naturally.

DRY RUN (remove duplicates from "programming"):
p r o g r a m m i n g
p(new) r(new) o(new) g(new) r(seen) a(new) m(new) m(seen)
i(new) n(new) g(seen) -> "progamin".

TIPS:
- c - '0' converts a digit char to its numeric value - the idiom
  behind every string-to-number parser.
- Run-length encoding is the idea inside JPEG's zero-runs and
  game sprite compression - a real algorithm, not just a puzzle.
- "seen[26]" boolean arrays are the simplest hash table - they
  scale up to count[] in hash_problems.c (folder 08).
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 200

int main() {
    char s[MAX_LEN];
    int choice;

    printf("String Traversal Ladder (easy -> hard)\n\n");

    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    scanf("%199s", s);

    while (1) {
        printf("\nCurrent: \"%s\" (length %d)\n", s, (int)strlen(s));
        printf("1. Vowel / consonant counter\n");
        printf("2. Toggle case\n");
        printf("3. Remove duplicates\n");
        printf("4. Capitalize words (a_b_c -> A_B_C)\n");
        printf("5. Run-length compress\n");
        printf("6. Pangram check\n");
        printf("7. Enter a new string\n");
        printf("8. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        int len = (int)strlen(s);

        if (choice == 1) {
            int vowels = 0;
            int consonants = 0;
            for (int i = 0; i < len; i++) {
                char c = s[i];
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    char lc = (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
                    if (lc == 'a' || lc == 'e' || lc == 'i' ||
                        lc == 'o' || lc == 'u') {
                        vowels = vowels + 1;
                    } else {
                        consonants = consonants + 1;
                    }
                }
            }
            printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
        } else if (choice == 2) {
            for (int i = 0; i < len; i++) {
                if (s[i] >= 'a' && s[i] <= 'z') {
                    s[i] = (char)(s[i] - 'a' + 'A');
                } else if (s[i] >= 'A' && s[i] <= 'Z') {
                    s[i] = (char)(s[i] - 'A' + 'a');
                }
            }
            printf("Toggled: \"%s\"\n", s);
        } else if (choice == 3) {
            int seen[256] = {0};
            int write = 0;
            for (int i = 0; i < len; i++) {
                if (!seen[(unsigned char)s[i]]) {
                    seen[(unsigned char)s[i]] = 1;
                    s[write] = s[i];
                    write = write + 1;
                }
            }
            s[write] = '\0';
            printf("Deduped: \"%s\"\n", s);
        } else if (choice == 4) {
            int newWord = 1;
            for (int i = 0; i < len; i++) {
                if (s[i] == '_' || s[i] == '-') {
                    newWord = 1;
                } else if (newWord && s[i] >= 'a' && s[i] <= 'z') {
                    s[i] = (char)(s[i] - 'a' + 'A');
                    newWord = 0;
                } else {
                    newWord = 0;
                }
            }
            printf("Capitalized: \"%s\"\n", s);
        } else if (choice == 5) {
            char outBuf[MAX_LEN * 2];
            int outLen = 0;
            for (int i = 0; i < len; i++) {
                int run = 1;
                while (i + run < len && s[i + run] == s[i]) {
                    run = run + 1;
                }
                outLen = outLen + sprintf(outBuf + outLen, "%c%d",
                                          s[i], run);
                i = i + run - 1;
            }
            outBuf[outLen] = '\0';
            printf("Compressed: \"%s\"\n", outBuf);
        } else if (choice == 6) {
            int seen[26] = {0};
            for (int i = 0; i < len; i++) {
                char c = s[i];
                if (c >= 'a' && c <= 'z') {
                    seen[c - 'a'] = 1;
                } else if (c >= 'A' && c <= 'Z') {
                    seen[c - 'A'] = 1;
                }
            }
            int all = 1;
            for (int i = 0; i < 26; i++) {
                if (!seen[i]) {
                    all = 0;
                    break;
                }
            }
            printf("Pangram: %s\n", all ? "YES" : "NO");
        } else if (choice == 7) {
            printf("New string: ");
            scanf("%199s", s);
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
