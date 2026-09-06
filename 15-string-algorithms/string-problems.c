/*
String Problems Ladder (Basic -> Hard)
---------------------------------------
Real life example:
Anagram grouping and string rotations appear in DNA analysis and
captcha/fuzzing tools; the two-pointer tricks here power them.

DIAGRAM (anagram check via count arrays; rotation via concatenation):
  "listen" vs "silent": counts match -> anagram
  is "bcda" a rotation of "abcd"?  "abcdabcd".contains("bcda") -> yes

LADDER (easy -> hard inside this file):
  1. Check anagram (sort)          (the entry problem)
  2. First unique character        (frequency + order)
  3. Longest common prefix         (vertical scan)
  4. String rotation check         (concatenation trick)
  5. Longest common substring      (DP table - the bridge to LCS)

BEGINNER EXPLANATION:
- Anagram: same letters, same counts -> count arrays equal.
- Rotation: s2 is a rotation of s1 iff s2 is a substring of
  s1+s1 ("waterbottle" in "erbottlewat"+"erbottlewat"). One
  concatenation replaces all rotation checks.
- Longest common substring DP: match[i][j] = 1 + match[i-1][j-1]
  when chars agree, else 0 - consecutive runs, reset on mismatch
  (unlike SUBSEQUENCE which skips - that is folder 11's lcs.c).

SUPER SIMPLE EXAMPLE (rotation):
s1 = "abc", s2 = "bca". s1+s1 = "abcabc". Is "bca" inside?
positions: abcabc -> "bca" at index 1 -> YES, rotation.
s2 = "acb": not in "abcabc" -> not a rotation.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Anagram with different lengths: instant NO (the count arrays
  would differ anyway, but the length check saves work).
- Anagram case sensitivity: "Listen"/"Silent" differ by case -
  this file is case-sensitive; normalize for the classic riddle.
- Longest common prefix of ONE string: the string itself; of
  empty set: undefined - guard.
- Rotation of identical strings: trivially yes (any string is a
  rotation of itself by 0).
- Substring vs subsequence: "ace" IS a subsequence of "abcde"
  but NOT a substring - the DP table resets differently; running
  both programs side by side shows the contrast.

DRY RUN (longest common substring "abcdxyz" vs "xyzabc"):
match[i][j] where chars agree: the "xyz" run: at i=4? "abcdxyz":
x at 4, y 5, z 6; "xyzabc": x 0, y 1, z 2.
match[4][0]=1, match[5][1]=2, match[6][2]=3 -> best 3 = "xyz".

TIPS:
- The s1+s1 rotation trick is a one-liner worth memorizing - it
  replaces an O(n^2) enumeration.
- Longest common substring is LCS's sibling: same table shape,
  different recurrence (0-reset vs max-of-two). Compare the two
  programs to internalize the difference.
- These problems power autocomplete (prefix), duplicate detection
  (anagram grouping via sorted-key hashing), and plagiarism
  tools (substring).
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

int main() {
    char s1[200];
    char s2[200];
    int choice;

    printf("String Problems Ladder (easy -> hard)\n\n");

    printf("Enter first string (max 99 chars): ");
    scanf("%99s", s1);

    while (1) {
        printf("\n1. Anagram check (needs second string)\n");
        printf("2. First unique character\n");
        printf("3. Longest common prefix (needs second string)\n");
        printf("4. Rotation check (needs second string)\n");
        printf("5. Longest common substring (needs second string)\n");
        printf("6. Enter new first string\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Second string: ");
            scanf("%99s", s2);
            int c1[256] = {0};
            int c2[256] = {0};
            int len1 = (int)strlen(s1);
            int len2 = (int)strlen(s2);
            if (len1 != len2) {
                printf("Different lengths -> not anagrams\n");
                continue;
            }
            for (int i = 0; i < len1; i++) {
                c1[(unsigned char)s1[i]] = c1[(unsigned char)s1[i]] + 1;
                c2[(unsigned char)s2[i]] = c2[(unsigned char)s2[i]] + 1;
            }
            int same = 1;
            for (int i = 0; i < 256; i++) {
                if (c1[i] != c2[i]) {
                    same = 0;
                    break;
                }
            }
            printf("Anagrams: %s\n", same ? "YES" : "NO");
        } else if (choice == 2) {
            int freq[256] = {0};
            int len = (int)strlen(s1);
            for (int i = 0; i < len; i++) {
                freq[(unsigned char)s1[i]] = freq[(unsigned char)s1[i]] + 1;
            }
            int pos = -1;
            for (int i = 0; i < len; i++) {
                if (freq[(unsigned char)s1[i]] == 1) {
                    pos = i;
                    break;
                }
            }
            if (pos == -1) {
                printf("No unique character\n");
            } else {
                printf("First unique: '%c' at index %d\n", s1[pos], pos);
            }
        } else if (choice == 3) {
            printf("Second string: ");
            scanf("%99s", s2);
            int i = 0;
            while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]) {
                i = i + 1;
            }
            if (i == 0) {
                printf("No common prefix\n");
            } else {
                printf("Common prefix: \"%.*s\" (%d chars)\n", i, s1, i);
            }
        } else if (choice == 4) {
            printf("Second string: ");
            scanf("%99s", s2);
            int len1 = (int)strlen(s1);
            int len2 = (int)strlen(s2);
            if (len1 != len2) {
                printf("Different lengths -> not rotations\n");
                continue;
            }
            char doubled[2 * MAX_LEN];
            snprintf(doubled, sizeof(doubled), "%s%s", s1, s1);
            int isRot = 0;
            for (int i = 0; i + len1 <= len1 * 2; i++) {
                int match = 1;
                for (int j = 0; j < len1; j++) {
                    if (doubled[i + j] != s2[j]) {
                        match = 0;
                        break;
                    }
                }
                if (match) {
                    isRot = 1;
                    break;
                }
            }
            printf("Rotation: %s\n", isRot ? "YES" : "NO");
        } else if (choice == 5) {
            printf("Second string: ");
            scanf("%99s", s2);
            int len1 = (int)strlen(s1);
            int len2 = (int)strlen(s2);
            static int match[200][200];
            int best = 0;
            int bestEnd1 = -1;
            for (int i = 0; i < len1; i++) {
                for (int j = 0; j < len2; j++) {
                    if (s1[i] == s2[j]) {
                        match[i][j] = (i > 0 && j > 0)
                                          ? match[i - 1][j - 1] + 1 : 1;
                        if (match[i][j] > best) {
                            best = match[i][j];
                            bestEnd1 = i;
                        }
                    } else {
                        match[i][j] = 0;
                    }
                }
            }
            if (best == 0) {
                printf("No common substring\n");
            } else {
                printf("Longest common substring: \"%.*s\" (%d chars)\n",
                       best, s1 + bestEnd1 - best + 1, best);
            }
        } else if (choice == 6) {
            printf("New first string: ");
            scanf("%99s", s1);
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
