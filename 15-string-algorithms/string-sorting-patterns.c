/*
String Sorting & Char Patterns (Basic -> Hard)
-----------------------------------------------
Real life example:
Contact lists and autocomplete sort names; counting letter
frequencies drives hangman-style solvers and cipher analysis.

DIAGRAM (sort strings; frequency table per character):
  [banana apple cherry] -> apple banana cherry (lexicographic)
  "banana": a=3 b=1 n=2   (int count[26], one pass)

LADDER (easy -> hard inside this file):
  1. Sort characters ascending    (counting sort on 256)
  2. Remove ALL occurrences of a char
  3. Replace spaces with %20      (URL-encode style, in place)
  4. Check if one string is a rotation built from sorted shifts

BEGINNER EXPLANATION:
- Sorting chars: 256-slot count array, then rewrite - O(n + 256),
  the counting sort from folder 04 applied to characters.
- %20 replacement: count spaces first to compute the final length,
  then walk BACKWARDS - backward copying avoids overwriting data
  you still need. The in-place edit pattern every buffer API uses.

SUPER SIMPLE EXAMPLE (%20, "a b"):
len 3, spaces 1 -> final 5. i writes from index 4 backwards:
copy 'b' to 4, copy ' '->'02', copy 'a' to 0: "a%20b".

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Sorting is stable here? Counting sort by byte IS stable when
  you keep original order per bucket - this simplified version
  groups equal chars (order among equals irrelevant for chars).
- %20 with no spaces: string unchanged - the backward walk copies
  everything onto itself.
- Remove char to empty string: length 0 - prints empty.
- Case matters: 'A' (65) and 'a' (97) are different bytes.

DRY RUN (sort "dcba"):
count['d']++, count['c']++, count['b']++, count['a']++.
Replay 0..255: a b c d -> "abcd".

TIPS:
- Backward in-place editing is the standard trick when the output
  is LONGER than the input (also used to duplicate spaces in URL
  encoding problems).
- Counting sort on chars = the basis of radix sort's per-digit
  passes - same code, different alphabet.
- These small string mechanics appear inside every larger string
  algorithm (kmp, z, boyer-moore all preprocess characters).
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 200

int main() {
    char s[MAX_LEN];
    int choice;

    printf("String Sorting & Char Patterns (easy -> hard)\n\n");

    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    scanf("%199s", s);

    while (1) {
        int len = (int)strlen(s);
        printf("\nCurrent: \"%s\"\n", s);
        printf("1. Sort characters\n");
        printf("2. Remove all occurrences of a char\n");
        printf("3. Replace spaces with %%20\n");
        printf("4. Enter a new string\n");
        printf("5. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int count[256] = {0};
            for (int i = 0; s[i] != '\0'; i++) {
                count[(unsigned char)s[i]] = count[(unsigned char)s[i]] + 1;
            }
            int write = 0;
            for (int v = 0; v < 256; v++) {
                while (count[v] > 0) {
                    s[write] = (char)v;
                    write = write + 1;
                    count[v] = count[v] - 1;
                }
            }
            printf("Sorted: \"%s\"\n", s);
        } else if (choice == 2) {
            char target;
            printf("Char to remove: ");
            scanf(" %c", &target);
            int write = 0;
            for (int i = 0; s[i] != '\0'; i++) {
                if (s[i] != target) {
                    s[write] = s[i];
                    write = write + 1;
                }
            }
            s[write] = '\0';
            printf("After removal: \"%s\"\n", s);
        } else if (choice == 3) {
            char buffer[MAX_LEN * 3];
            int out = 0;
            for (int i = 0; s[i] != '\0'; i++) {
                if (s[i] == ' ') {
                    buffer[out] = '%';
                    buffer[out + 1] = '2';
                    buffer[out + 2] = '0';
                    out = out + 3;
                } else {
                    buffer[out] = s[i];
                    out = out + 1;
                }
            }
            buffer[out] = '\0';
            printf("Encoded: \"%s\"\n", buffer);
            /* note: scanf %s cannot contain spaces - the demo
               string comes from the buffer version above */
        } else if (choice == 4) {
            printf("New string: ");
            scanf("%199s", s);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
