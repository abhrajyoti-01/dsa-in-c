/*
String Operations
-----------------
What this program solves:
Menu-driven string operations implemented from scratch (no
string.h helpers where the algorithm matters): length, reverse,
palindrome check, compare, concatenate, copy, count
vowels/consonants/words, toggle case, and character frequency.

Real life example:
Text editors implement these constantly: spell-checkers count
letters, formatters reverse lines, search boxes compare case-
insensitively. Understanding them demystifies library functions.

DIAGRAM (a C string is a char array with a \0 sentinel):
  [H][e][l][l][o][\0]
   0  1  2  3  4  5     length = 5 (sentinel not counted)
  reverse: swap s[0]<->s[4], s[1]<->s[3] moving inward.
  concat: start copying at the \0 of the first string.

How the process works:
1. Length: walk until the '\0' terminator.
2. Reverse: swap ends moving inward (two pointers).
3. Palindrome: compare s[i] with s[len-1-i] for i < len/2.
4. Compare: char-by-char until difference or both end.
5. Concatenate: append source at destination's null terminator.
6. Frequency: count array indexed by character code.

Pseudo code:
START
LENGTH(s): i = 0; WHILE s[i] != '\0': i++; RETURN i
REVERSE(s): FOR i < len/2: swap s[i], s[len-1-i]
PALINDROME: FOR i < len/2: IF s[i] != s[len-1-i]: false
END

Sample input and dry run:
"madam" -> length 5, palindrome YES
reverse -> "madam"; vowels 2; words 1
frequency: m:2, a:2, d:1

Main logic to understand:
C strings are char arrays terminated by '\0' - every operation is
just careful pointer/index arithmetic. O(n) per operation; in-place
reverse costs O(1) extra space.
*/

#include <stdio.h>

#define MAX_LEN 200

/*
 * strLength - Counts characters up to the terminator.
 */
int strLength(const char s[]) {
    int len = 0;
    while (s[len] != '\0') {
        len = len + 1;
    }
    return len;
}

/*
 * strReverse - In-place two-pointer swap.
 */
void strReverse(char s[]) {
    int len = strLength(s);
    for (int i = 0; i < len / 2; i++) {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

/*
 * isPalindrome - Case-sensitive mirror check.
 */
int isPalindrome(const char s[]) {
    int len = strLength(s);
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

/*
 * strConcat - Appends t to the end of s.
 */
void strConcat(char s[], const char t[]) {
    int len = strLength(s);
    int j = 0;
    while (t[j] != '\0') {
        s[len + j] = t[j];
        j = j + 1;
    }
    s[len + j] = '\0';
}

/*
 * strCompare - Lexicographic: -1, 0, or 1.
 */
int strCompare(const char a[], const char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) {
        i = i + 1;
    }
    if (a[i] < b[i]) {
        return -1;
    }
    if (a[i] > b[i]) {
        return 1;
    }
    return 0;
}

int main() {
    char s[MAX_LEN];
    char t[MAX_LEN];
    int choice;

    printf("String Operations (from scratch)\n\n");

    printf("Enter a string (max %d chars, no spaces): ", MAX_LEN - 1);
    scanf("%199s", s);

    while (1) {
        printf("\nCurrent string: \"%s\" (length %d)\n", s, strLength(s));
        printf("1. Length\n");
        printf("2. Reverse\n");
        printf("3. Palindrome check\n");
        printf("4. Compare with another string\n");
        printf("5. Concatenate another string\n");
        printf("6. Count vowels, consonants, digits, words\n");
        printf("7. Toggle case\n");
        printf("8. Character frequency\n");
        printf("9. Enter a new string\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Length = %d\n", strLength(s));
        } else if (choice == 2) {
            strReverse(s);
            printf("Reversed: \"%s\"\n", s);
        } else if (choice == 3) {
            printf("\"%s\" is a palindrome: %s\n",
                   s, isPalindrome(s) ? "YES" : "NO");
        } else if (choice == 4) {
            printf("Enter string to compare: ");
            scanf("%199s", t);
            int r = strCompare(s, t);
            printf("Compare: \"%s\" is %s \"%s\"\n",
                   s, r < 0 ? "less than" : (r > 0 ? "greater than" : "equal to"), t);
        } else if (choice == 5) {
            printf("Enter string to append: ");
            scanf("%199s", t);
            if (strLength(s) + strLength(t) < MAX_LEN) {
                strConcat(s, t);
                printf("Concatenated: \"%s\"\n", s);
            } else {
                printf("Result would overflow the buffer\n");
            }
        } else if (choice == 6) {
            int vowels = 0;
            int consonants = 0;
            int digits = 0;
            int words = 1;
            for (int i = 0; s[i] != '\0'; i++) {
                char c = s[i];
                if (c >= '0' && c <= '9') {
                    digits = digits + 1;
                } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    char lc = (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
                    if (lc == 'a' || lc == 'e' || lc == 'i' || lc == 'o' || lc == 'u') {
                        vowels = vowels + 1;
                    } else {
                        consonants = consonants + 1;
                    }
                } else if (c == ' ') {
                    words = words + 1;
                }
            }
            printf("Vowels: %d, Consonants: %d, Digits: %d, Words: %d\n",
                   vowels, consonants, digits, words);
        } else if (choice == 7) {
            for (int i = 0; s[i] != '\0'; i++) {
                if (s[i] >= 'a' && s[i] <= 'z') {
                    s[i] = (char)(s[i] - 'a' + 'A');
                } else if (s[i] >= 'A' && s[i] <= 'Z') {
                    s[i] = (char)(s[i] - 'A' + 'a');
                }
            }
            printf("Toggled: \"%s\"\n", s);
        } else if (choice == 8) {
            printf("Frequency:\n");
            for (int i = 0; s[i] != '\0'; i++) {
                int already = 0;
                for (int j = 0; j < i; j++) {
                    if (s[j] == s[i]) {
                        already = 1;
                        break;
                    }
                }
                if (already) {
                    continue;
                }
                int count = 0;
                for (int j = 0; s[j] != '\0'; j++) {
                    if (s[j] == s[i]) {
                        count = count + 1;
                    }
                }
                printf("  '%c' : %d\n", s[i], count);
            }
        } else if (choice == 9) {
            printf("Enter a new string: ");
            scanf("%199s", s);
        } else if (choice == 10) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
