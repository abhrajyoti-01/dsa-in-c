/*
String Interview Problems - atoi, Valid Parentheses, Run-Length, Longest
Common Prefix, Reverse Words, Valid Palindrome (Intermediate -> Advanced)
-----------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 8    - String to Integer (atoi) with all edge rules
  LeetCode 20   - Valid Parentheses
  LeetCode 443  - String Compression (run-length)
  LeetCode 14   - Longest Common Prefix
  LeetCode 151  - Reverse Words in a String
  LeetCode 125  - Valid Palindrome (letter/digit filtering)
  HackerRank "Super Reduced String" (adjacent duplicate removal) family

PROBLEM DEFINITION:
Six parsing and formatting classics that appear constantly in screens.
Each one is a single left-to-right scan with an explicit rule for
whitespace, signs, overflow, nesting depth or word boundaries.

REAL-LIFE EXAMPLE:
Config-file parsing (atoi with overflow guards), JSON/template brace
validation (matched pairs), log compression (run-length) and search-box
input normalisation (palindrome over letters/digits only) are these same
scans in production code.

DIAGRAM (atoi of "   -42abc"):
  skip spaces -> '-' -> digits 4, 2 -> stop at 'a'
  sign applies: -42; the trailing "abc" is IGNORED (not an error)
  saturation: "2147483648" overflows -> clamp to 2147483647
              "-2147483649" underflows -> clamp to -2147483648

  valid parentheses "{[()]}": push { [ ( ; ')' matches ( ; ']' matches
  [ ; '}' matches { -> empty at the end -> valid. "([)]" fails because
  the top of the stack is '[' when ')' arrives.

  palindrome "A man, a plan, a canal: Panama":
  keep only alphanumerics -> amanaplanacanalpanama -> reads the same
  both ways -> true. Two pointers skip every other character in O(n).

PSEUDO CODE:
  Atoi(s): skip leading spaces; read one optional sign; read digits
      accumulating into long long; stop at the first non-digit; clamp
  ValidParentheses(s): stack; push openers; a closer must match the
      top; the stack must be empty at the end
  RunLength(s): scan equal runs; emit character + count (>1 only)
  LongestCommonPrefix(words): shrink the prefix while any word does
      not start with it
  ReverseWords(s): split on spaces, keep the words, print them backwards
  IsPalindrome(s): two pointers ignoring non-alphanumeric characters

SAMPLE INPUT & DRY RUN:
  3                          <- atoi: how many strings
    -42abc
  +0042
  2147483648
  3                          <- valid parentheses: how many strings
  {[()]}
  ([)]
                             <- a BLANK line = the empty string (valid)
  aabcccccaaa                <- string compression (NO count line)
  3                          <- longest common prefix: how many words
  apple
  apply
  apricot
  never odd or even          <- reverse words (one sentence)
  A man, a plan, a canal: Panama   <- valid palindrome (one sentence)
  Every section reads its OWN line(s): atoi and brackets take a count
  first; compression takes a single word; the prefix list takes a count
  then one word PER LINE; reverse and palindrome each take one sentence.
  atoi: -42, 42, and 2147483648 clamped to 2147483647.
  "{()[]}"-style nesting is accepted; the mismatched-type "([)]" is
  rejected even though its bracket COUNTS are balanced.
  Run-length of aabcccccaaa -> a2bc5a3.
  Longest common prefix of apple/apply/apricot -> "ap".
  Reversed words of "never odd or even" -> "even or odd never".
  Palindrome of "A man, a plan, a canal: Panama" -> true (case and
  punctuation ignored).

SPECIAL CASES:
  - atoi ignores trailing garbage, clamps on overflow, and accepts only
    ONE leading sign; "++1", an empty line, and "-" alone give 0.
  - A blank/whitespace-only line IS read and reported as 0, matching the
    documented rule (a previous draft silently skipped such lines).
  - Valid parentheses: an empty string is valid; a lone closer fails;
    mismatched TYPE ( "([)]" ) fails even though the counts match.
  - Run-length compression of "abc" is "abc" (count 1 omitted); of ""
    is "" - the convention is stated in the printed output.
  - Longest common prefix: an empty word list is rejected; a word that
    becomes empty short-circuits the prefix to "".
  - Reverse words collapses multiple spaces and tabs; a line with only
    spaces yields 0 words.
  - Palindrome ignores case and every non-alphanumeric character; an
    empty string is a palindrome.
  - Lines are read with fgets, so SPACES and EMPTY lines survive; the
    over-long line is safely truncated by fgets (never desynchronises
    the following reads) and the truncation is reported.

COMPLEXITY:
  O(n) time, O(1) or O(n) extra space per routine; prefix shrinking is
  O(total characters) amortised.

TIPS:
  Interviews watch the edge rules more than the happy path. State the
  rules BEFORE coding: whitespace, sign, overflow, trailing junk. That
  single habit is what separates a passing atoi from a failing one.
  For line-oriented input prefer fgets over scanf("%s"): scanf stops at
  whitespace, cannot return empty lines, and leaves the newline behind.
*/

#include <stdio.h>
#include <string.h>

#define MAX_TEXT 500
#define MAX_WORDS 100

char line[MAX_TEXT + 2];
char words[MAX_WORDS][MAX_TEXT + 1];
char encoded[MAX_TEXT * 2 + 2];

/* Reads one line into `line`, returns 1 on success, 0 at EOF.
 * Reports (but tolerates) truncation of over-long lines. */
int readLine(const char* prompt) {
    if (prompt != NULL) printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    size_t rawLength = strlen(line);
    /* fgets keeps the newline when it is present, so a line that does
     * NOT end in a newline while filling the buffer was too long. */
    int endedWithNewline =
        (rawLength > 0 && (line[rawLength - 1] == '\n' ||
                           line[rawLength - 1] == '\r'));
    size_t length = strcspn(line, "\r\n");
    line[length] = '\0';
    if (!endedWithNewline && rawLength == sizeof(line) - 1) {
        /* Drain the rest of the over-long line so the next read is
         * aligned, then say so rather than corrupting later input. */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        printf("   (note: line truncated to %d characters)\n", MAX_TEXT);
    }
    return 1;
}

/* Skips whitespace, reads one optional sign, then digits; clamps to the
 * 32-bit int range. *valid is set to 1 only when at least one digit was
 * read, so "" and "-" report 0 with valid = 0. */
int atoiClamped(const char* s, int* valid) {
    const long long low = -2147483648LL;
    const long long high = 2147483647LL;
    int i = 0;
    while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') i++;
    int sign = 1;
    if (s[i] == '+' || s[i] == '-') {
        if (s[i] == '-') sign = -1;
        i++;
    }
    long long value = 0;
    int digits = 0;
    while (s[i] >= '0' && s[i] <= '9') {
        digits = 1;
        /* Stop accumulating well before overflow: the clamp below then
         * sees a value that is still far outside the int range. */
        if (value < high + 2) value = value * 10 + (s[i] - '0');
        i++;
    }
    *valid = digits;
    value *= sign;
    if (value > high) return (int)high;
    if (value < low) return (int)low;
    return (int)value;
}

int validParentheses(const char* s) {
    char stack[MAX_TEXT];
    int top = -1;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            if (top + 1 >= MAX_TEXT) return 0;      /* defensive depth cap */
            stack[++top] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top < 0) return 0;
            char open = stack[top--];
            if ((c == ')' && open != '(') || (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
                return 0;
            }
        } else {
            return 0;                        /* unexpected character */
        }
    }
    return top == -1;
}

/* Writes the run-length encoding of `s` into `out` (caller-sized for
 * the worst case: every character distinct -> 2 * strlen, and every
 * character identical -> length + digits). */
void runLengthEncode(const char* s, char* out) {
    int write = 0;
    int i = 0;
    while (s[i] != '\0') {
        int run = 1;
        while (s[i + run] == s[i]) run++;
        out[write++] = s[i];
        if (run > 1) {
            char digits[16];
            int length = 0;
            int value = run;
            while (value > 0) {
                digits[length++] = (char)('0' + value % 10);
                value /= 10;
            }
            while (length > 0) out[write++] = digits[--length];
        }
        i += run;
    }
    out[write] = '\0';
}

void longestCommonPrefix(int count) {
    char prefix[MAX_TEXT + 1];
    strcpy(prefix, words[0]);
    for (int w = 1; w < count; w++) {
        size_t match = 0;
        while (prefix[match] != '\0' && words[w][match] != '\0' &&
               prefix[match] == words[w][match]) {
            match++;
        }
        prefix[match] = '\0';
        if (match == 0) break;
    }
    printf("Longest common prefix = \"%s\"\n", prefix);
}

/* LeetCode 125: two pointers, skipping non-alphanumerics, case-insensitive. */
int isPalindrome(const char* s) {
    int left = 0;
    int right = (int)strlen(s) - 1;
    while (left < right) {
        unsigned char a = (unsigned char)s[left];
        unsigned char b = (unsigned char)s[right];
        if (a >= 'A' && a <= 'Z') a = (unsigned char)(a - 'A' + 'a');
        if (b >= 'A' && b <= 'Z') b = (unsigned char)(b - 'A' + 'a');
        int aIs = (a >= 'a' && a <= 'z') || (a >= '0' && a <= '9');
        int bIs = (b >= 'a' && b <= 'z') || (b >= '0' && b <= '9');
        if (!aIs) { left++; continue; }
        if (!bIs) { right--; continue; }
        if (a != b) return 0;
        left++;
        right--;
    }
    return 1;
}

int main(void) {
    printf("String Interview Problems\n\n");

    /* ---------------- atoi ---------------- */
    printf("== atoi. How many strings to convert (1-100)? ");
    int count;
    if (scanf("%d", &count) != 1 || count < 1 || count > 100) {
        printf("Invalid count\n");
        return 1;
    }
    int newline;
    while ((newline = getchar()) != '\n' && newline != EOF) { }
    printf("Enter each string on its own line (leading spaces allowed,\n");
    printf("a blank line is a valid empty conversion -> 0):\n");
    int consumed = 0;
    while (consumed < count) {
        if (!readLine(NULL)) break;
        int valid = 0;
        int value = atoiClamped(line, &valid);
        printf("   atoi(\"%s\") = %d", line, value);
        if (!valid) printf("  (no digits -> 0)");
        printf("\n");
        consumed++;
    }
    if (consumed < count) {
        printf("Only %d of %d strings were read (end of input)\n", consumed,
               count);
    }

    /* -------- valid parentheses -------- */
    printf("\n== Valid parentheses. How many strings (1-100)? ");
    if (scanf("%d", &count) != 1 || count < 1 || count > 100) {
        printf("Invalid count\n");
        return 1;
    }
    while ((newline = getchar()) != '\n' && newline != EOF) { }
    for (int i = 0; i < count; i++) {
        printf("Bracket line %d (may be empty for the empty string): ",
               i + 1);
        if (!readLine(NULL)) {
            printf("Invalid string\n");
            return 1;
        }
        printf("   \"%s\" is %s\n", line,
               validParentheses(line) ? "valid" : "NOT valid");
    }

    /* -------- run-length compression -------- */
    printf("\n== String compression. Enter a word (may be empty): ");
    if (!readLine(NULL)) {
        printf("Invalid word\n");
        return 1;
    }
    runLengthEncode(line, encoded);
    printf("Run-length encoding of \"%s\" = \"%s\"\n", line, encoded);
    printf("(count 1 is omitted, so \"abc\" stays \"abc\" and \"\" stays \"\")\n");

    /* -------- longest common prefix -------- */
    printf("\n== Longest common prefix. How many words (1-%d)? ", MAX_WORDS);
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_WORDS) {
        printf("Invalid word count\n");
        return 1;
    }
    while ((newline = getchar()) != '\n' && newline != EOF) { }
    printf("Enter %d words, one per line (no spaces needed):\n", count);
    for (int i = 0; i < count; i++) {
        if (!readLine(NULL)) {
            printf("Invalid word\n");
            return 1;
        }
        /* readLine already capped the content at MAX_TEXT characters,
         * so a plain copy of the terminated string always fits. */
        size_t copyLength = strlen(line);
        if (copyLength > MAX_TEXT) copyLength = MAX_TEXT;
        memcpy(words[i], line, copyLength);
        words[i][copyLength] = '\0';
    }
    longestCommonPrefix(count);

    /* -------- reverse words -------- */
    printf("\n== Reverse words. Enter a sentence (spaces/tabs collapse):\n");
    if (!readLine(NULL)) {
        printf("Invalid line\n");
        return 1;
    }
    int wordCount = 0;
    char* cursor = line;
    while (*cursor != '\0') {
        while (*cursor == ' ' || *cursor == '\t') cursor++;
        if (*cursor == '\0') break;
        if (wordCount >= MAX_WORDS) {
            printf("   (only the first %d words are kept)\n", MAX_WORDS);
            break;
        }
        int length = 0;
        while (cursor[length] != '\0' && cursor[length] != ' ' &&
               cursor[length] != '\t') {
            if (length < MAX_TEXT) {
                words[wordCount][length] = cursor[length];
            }
            length++;
        }
        if (length > MAX_TEXT) length = MAX_TEXT;
        words[wordCount][length] = '\0';
        wordCount++;
        cursor += length;
    }
    printf("Reversed words:");
    for (int i = wordCount - 1; i >= 0; i--) printf(" %s", words[i]);
    printf("\nWord count = %d\n", wordCount);

    /* -------- valid palindrome -------- */
    printf("\n== Valid palindrome. Enter a sentence (case and punctuation\n");
    printf("   are ignored; an empty string is a palindrome):\n");
    if (!readLine(NULL)) {
        printf("Invalid line\n");
        return 1;
    }
    printf("\"%s\" is %sa palindrome (LeetCode 125)\n", line,
           isPalindrome(line) ? "" : "NOT ");
    return 0;
}
