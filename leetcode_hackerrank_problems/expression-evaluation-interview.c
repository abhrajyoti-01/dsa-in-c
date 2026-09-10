/*
Expression Evaluation - Calculators, Decode String, Simplify Path (Advanced)
----------------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 227 - Basic Calculator II (+ - * / precedence, no brackets)
  LeetCode 224 - Basic Calculator (brackets and unary minus)
  LeetCode 394 - Decode String (nested k[encoded] groups)
  LeetCode 71  - Simplify Path (canonical Unix path with . and ..)
  related: HackerRank "Balanced Brackets" - the same nesting stack with
           only a validity answer instead of a computed value.

PROBLEM DEFINITION:
Four problems whose input is a grammar rather than a plain list. One
recursive-descent evaluator answers BOTH calculator problems, a stack
of saved (count, position) pairs unpacks nested encoded strings, and a
tokenise-then-rebuild pass canonicalises a path. The shared idea: read
left to right, keep a small stack, and let one peek at the top (or one
level of recursion) decide precedence.

REAL-LIFE EXAMPLE:
Spreadsheet formula bars (calculators), template or macro engines that
expand "3[item]" groups (decode string), and every shell, build tool and
file manager (path canonicalisation) all run this exact kind of parser.

DIAGRAM (recursive descent for "3-4*2+(5-1)"):
  expression := term (('+' | '-') term)*
  term       := factor (('*' | '/') factor)*
  factor     := number | '(' expression ')' | ('-' | '+') factor
  trace:
    term     3            -> 3
    "-" term 4*2          -> 8        running = 3 - 8  = -5
    "+" term (5-1)        -> 4        running = -5 + 4 = -1
  => -1

PSEUDO CODE:
  Evaluate(s):                        (one parser for 224 and 227)
      parseExpression: v = parseTerm; while next is + or -:
          add or subtract the next term
      parseTerm:       v = parseFactor; while next is * or /:
          multiply or divide by the next factor
      parseFactor: skip spaces; consume any unary + or -;
          a digit run is a number; '(' recurses into parseExpression
  DecodeString(s):
      countStack[], posStack[];  text = ""
      digit   -> extend the current multiplier
      '['     -> push multiplier and len(text); reset both
      ']'     -> pop; repeat text[start..] multiplier times
      letter  -> append to text
  SimplifyPath(path):
      split on '/'; "." is ignored; ".." pops the last component;
      anything else is pushed; join with a leading '/'

SAMPLE INPUT & DRY RUN:
  3+2*2                     <- part 1: LeetCode 227
  (1+(4+5+2)-3)+(6+8)       <- part 2: LeetCode 224
  3[a2[c]]                  <- part 3: LeetCode 394
  /a/./b/../../c/           <- part 4: LeetCode 71
  Exactly ONE line per section (four lines total - the program reads a
  single expression/text/path for each LeetCode problem).
  "3+2*2" = 3 + (2*2) = 7 because * binds tighter than +.
  "(1+(4+5+2)-3)+(6+8)" = (1+11-3) + 14 = 9 + 14 = 23.
  3[a2[c]] -> 2[c] = "cc", so a+"cc" = "acc", times 3 = "accaccacc".
  /a/./b/../../c/ -> a, then ".." pops b, ".." pops a, c -> "/c".
  Two more hand-checks you can type in instead (one at a time):
  "14-3/2" = 14 - 1 = 13 (integer division truncates toward zero) and
  "2[abc]3[cd]ef" = "abcabc" + "cdcdcd" + "ef" = "abcabccdcdcdef".

SPECIAL CASES:
  - Integer division truncates toward zero (C99 and both LeetCode
    problems): 3/2 = 1 and -3/2 = -1. Division by zero is rejected.
  - Unary minus is a factor: "-(2+3)" = -5, and a leading '+' is legal.
  - The evaluator skips spaces everywhere, so "3 + 2 * 2" is 7.
  - Decode String multipliers may have several digits (100[ab]), so the
    multiplier is accumulated, not read one digit at a time.
  - Nesting depth is bounded by the stack arrays; exceeding them is
    reported, never silently truncated.
  - The decoded result has its own cap, so an input like 1000[a] that
    would expand enormously is rejected with a message instead of
    overflowing the buffer.
  - ".." at the root cannot pop: "/../" simplifies to "/".
  - Repeated and trailing slashes collapse: "/home//foo/" -> "/home/foo".
  - A stray ']' or an unclosed '[' makes Decode String exit with an
    error rather than print a wrong string.
  - Text is read with %s, so the sample inputs contain no spaces.

COMPLEXITY:
  O(length) time for each part. Recursion depth (or stack depth) is the
  bracket-nesting depth, which costs O(nesting) extra space.

TIPS:
  Write the grammar before the parser (see DIAGRAM). For precedence,
  recurse once per precedence level - that single trick covers 224,
  227 and 772. For nesting, push SAVED STATE ("where I was") rather
  than characters ("what I saw"); that is why the decode stacks store
  the position, not the text.
*/

#include <stdio.h>

#define MAX_TEXT 500
#define MAX_RESULT 2000
#define MAX_STACK 200

char text[MAX_TEXT + 1];
int position;
int errorFlag;

/* One recursive-descent parser serves BOTH LeetCode 224 and 227: the
 * three levels of the grammar (expression -> term -> factor) encode the
 * precedence, and the brackets simply call back into the top level. */
int parseExpression(void);

void skipSpaces(void) {
    while (text[position] == ' ') position++;
}

int peekToken(void) {
    skipSpaces();
    return text[position];
}

int parseFactor(void) {
    skipSpaces();
    int sign = 1;
    while (text[position] == '-' || text[position] == '+') {
        if (text[position] == '-') sign = -sign;
        position++;
        skipSpaces();
    }
    long long value = 0;
    if (text[position] >= '0' && text[position] <= '9') {
        while (text[position] >= '0' && text[position] <= '9') {
            value = value * 10 + (text[position] - '0');
            position++;
        }
    } else if (text[position] == '(') {
        position++;
        value = parseExpression();
        skipSpaces();
        if (text[position] == ')') {
            position++;
        } else {
            errorFlag = 1;              /* unclosed '(' */
            return 0;
        }
    } else {
        errorFlag = 1;                  /* expected a number or '(' */
        return 0;
    }
    return (int)(sign * value);
}

int parseTerm(void) {
    long long value = parseFactor();
    for (;;) {
        int c = peekToken();
        if (c != '*' && c != '/') break;
        position++;
        long long rhs = parseFactor();
        if (c == '*') {
            value *= rhs;
        } else {
            if (rhs == 0) {
                errorFlag = 2;          /* division by zero */
                return 0;
            }
            value /= rhs;               /* C truncates toward zero */
        }
    }
    return (int)value;
}

int parseExpression(void) {
    long long value = parseTerm();
    for (;;) {
        int c = peekToken();
        if (c != '+' && c != '-') break;
        position++;
        long long rhs = parseTerm();
        if (c == '+') value += rhs;
        else value -= rhs;
    }
    return (int)value;
}

int evaluate(const char* s, int* errorCode) {
    int i;
    for (i = 0; i < MAX_TEXT; i++) {
        text[i] = s[i];
        if (s[i] == '\0') break;
    }
    text[MAX_TEXT] = '\0';
    position = 0;
    errorFlag = 0;
    int value = parseExpression();
    skipSpaces();
    if (text[position] != '\0') errorFlag = 1;   /* trailing garbage */
    *errorCode = errorFlag;
    return value;
}

/* LeetCode 394: a stack of SAVED STATE (count, text length so far). On
 * ']' the block appended since the matching '[' is duplicated count-1
 * more times, in place - which is why the length is saved. */
int decodeString(const char* s, char* out) {
    int counts[MAX_STACK];
    int starts[MAX_STACK];
    int top = 0;
    int length = 0;
    int i = 0;
    while (s[i] != '\0') {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            int count = 0;
            while (s[i] >= '0' && s[i] <= '9') {
                count = count * 10 + (s[i] - '0');
                i++;
            }
            if (s[i] != '[') return -1;          /* digits without a '[' */
            if (top >= MAX_STACK || count <= 0) return -1;
            counts[top] = count;
            starts[top] = length;
            top++;
            i++;                                 /* step past '[' */
            continue;
        }
        if (c == ']') {
            if (top == 0) return -1;             /* stray ']' */
            top--;
            int count = counts[top];
            int start = starts[top];
            int block = length - start;
            for (int repeat = 1; repeat < count; repeat++) {
                if (length + block > MAX_RESULT) return -1;
                for (int k = 0; k < block; k++) out[length++] = out[start + k];
            }
            i++;
            continue;
        }
        if (length >= MAX_RESULT) return -1;
        out[length++] = c;
        i++;
    }
    if (top != 0) return -1;                     /* unclosed '[' */
    out[length] = '\0';
    return length;
}

/* LeetCode 71: split on '/', ignore '.', pop on '..', push otherwise. */
int simplifyPath(const char* path, char* out) {
    static char parts[MAX_STACK][64];
    int count = 0;
    int i = 0;
    while (path[i] != '\0') {
        while (path[i] == '/') i++;
        if (path[i] == '\0') break;
        char token[64];
        int length = 0;
        while (path[i] != '\0' && path[i] != '/') {
            if (length >= 63) return -1;         /* component too long */
            token[length++] = path[i++];
        }
        token[length] = '\0';
        if (length == 1 && token[0] == '.') continue;
        if (length == 2 && token[0] == '.' && token[1] == '.') {
            if (count > 0) count--;              /* '..' at root is a no-op */
            continue;
        }
        if (count >= MAX_STACK) return -1;
        for (int k = 0; k <= length; k++) parts[count][k] = token[k];
        count++;
    }
    int written = 0;
    if (count == 0) {
        out[written++] = '/';
    } else {
        for (int p = 0; p < count; p++) {
            out[written++] = '/';
            for (int k = 0; parts[p][k] != '\0'; k++) out[written++] = parts[p][k];
        }
    }
    out[written] = '\0';
    return written;
}

int runCalculator(const char* label, const char* prompt) {
    char line[MAX_TEXT + 1];
    printf("%s\n", label);
    printf("%s", prompt);
    if (scanf("%500s", line) != 1) {
        printf("Invalid expression\n");
        return -1;
    }
    int code = 0;
    int value = evaluate(line, &code);
    if (code == 1) {
        printf("Malformed expression (check the brackets and operators)\n");
        return -1;
    }
    if (code == 2) {
        printf("Division by zero\n");
        return -1;
    }
    printf("%s = %d\n", line, value);
    return 0;
}

int main(void) {
    char line[MAX_TEXT + 1];
    char result[MAX_RESULT + 1];

    printf("Expression Evaluation - Calculator, Decode String, Simplify "
           "Path\n");

    if (runCalculator("\n--- LeetCode 227 (no brackets; * and / bind "
                      "tighter than + and -) ---",
                      "Enter an expression like 3+2*2: ") != 0) return 1;

    printf("\n--- LeetCode 224 (brackets and unary minus) ---\n");
    printf("Enter an expression like (1+(4+5+2)-3)+(6+8): ");
    if (scanf("%500s", line) != 1) {
        printf("Invalid expression\n");
        return 1;
    }
    int code = 0;
    int value = evaluate(line, &code);
    if (code == 1) {
        printf("Malformed expression (check the brackets and operators)\n");
        return 1;
    }
    if (code == 2) {
        printf("Division by zero\n");
        return 1;
    }
    printf("%s = %d\n", line, value);

    printf("\n--- LeetCode 394 (decode string) ---\n");
    printf("Enter an encoded string like 3[a2[c]]: ");
    if (scanf("%500s", line) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    int length = decodeString(line, result);
    if (length < 0) {
        printf("Malformed encoding (unbalanced brackets or too long a "
               "result)\n");
        return 1;
    }
    printf("%s -> %s  (%d characters)\n", line, result, length);

    printf("\n--- LeetCode 71 (simplify path) ---\n");
    printf("Enter an absolute path like /a/./b/../../c/: ");
    if (scanf("%500s", line) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (line[0] != '/') {
        printf("Invalid path (must start with '/')\n");
        return 1;
    }
    if (simplifyPath(line, result) < 0) {
        printf("Invalid path (a component is too long or too deep)\n");
        return 1;
    }
    printf("%s -> %s\n", line, result);
    return 0;
}
