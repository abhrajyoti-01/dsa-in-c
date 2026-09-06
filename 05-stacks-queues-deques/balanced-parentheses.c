/*
Balanced Parentheses Checker (Stack Application)
------------------------------------------------
What this program solves:
Checks whether (), [], {} in an expression are balanced and
properly nested - the single most famous stack application.

Real life example:
Compilers and IDEs validate brackets while you type; JSON/XML
parsers reject documents the same way - mismatched nesting.

DIAGRAM ("([{}])" - stack states):
  (  push   [ ( ]
  [  push   [ ( [ ]
  {  push   [ ( [ { ]
  }  pops {   [ ( [ ]
  ]  pops [   [ ( ]
  )  pops (   [ ] empty -> BALANCED
  a mismatch (or leftover opener) means NOT balanced.

BEGINNER EXPLANATION (the simplest way to think):
A stack is a pile of plates. Every OPENING bracket pushes a plate
marked with its type. Every CLOSING bracket asks: "is the top
plate the matching type?" - if yes, pop it; if no (or pile empty),
the expression is broken. At the end the pile must be EMPTY.

SUPER SIMPLE EXAMPLE:
"([{}])"
( -> push (          pile: (
[ -> push [          pile: ( [
{ -> push {          pile: ( [ {
} -> matches {, pop  pile: ( [
] -> matches [, pop  pile: (
) -> matches (, pop  pile: (empty) -> BALANCED

"([)]"
( -> push, [ -> push, ) -> top is [ NOT ( -> BROKEN right there.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Closing bracket with empty stack: ")(" -> broken (nothing to
  match). This is why you check isEmpty BEFORE peeking.
- Leftover openers: "((" -> pile not empty at the end -> broken.
  Forgetting this final check is the classic bug.
- Non-bracket characters: ignore letters, digits, operators -
  only brackets matter.
- Empty string: trivially balanced (nothing can be wrong).
- Interleaved types like "{[}]" - the pop mismatch catches it
  even though counts are equal. COUNTING brackets is not enough;
  ORDER matters - that is why a stack and not a counter.

DRY RUN ("{[()]}"):
{ -> push {          pile: {
[ -> push [          pile: { [
( -> push (          pile: { [ (
) -> top ( matches, pop -> pile: { [
] -> top [ matches, pop -> pile: {
} -> top { matches, pop -> pile: empty -> BALANCED

TIPS:
- One stack + one function "do these two match?" solves all three
  bracket types - never write three separate checkers.
- This exact pattern powers compilers, JSON/XML validators, and
  editor auto-indent.
- Related: the same stack converts infix to postfix (next program)
  and evaluates postfix. Learn this one well - two more come free.
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 200

char stackArr[MAX_LEN];
int top = -1;

void push(char c) {
    if (top >= MAX_LEN - 1) {
        return;
    }
    top = top + 1;
    stackArr[top] = c;
}

char pop(void) {
    if (top == -1) {
        return '\0';
    }
    char c = stackArr[top];
    top = top - 1;
    return c;
}

int isEmpty(void) {
    return top == -1;
}

/*
 * isMatchingPair - Does the opener match this closer?
 */
int isMatchingPair(char opener, char closer) {
    return (opener == '(' && closer == ')') ||
           (opener == '[' && closer == ']') ||
           (opener == '{' && closer == '}');
}

/*
 * isBalanced - Full check with running commentary.
 */
int isBalanced(const char* expr, int verbose) {
    top = -1;

    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];

        if (c == '(' || c == '[' || c == '{') {
            if (verbose) printf("  index %d: push '%c'\n", i, c);
            push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (isEmpty()) {
                if (verbose) {
                    printf("  index %d: closing '%c' but stack empty -> BROKEN\n", i, c);
                }
                return 0;
            }
            char openTop = pop();
            if (verbose) {
                printf("  index %d: '%c' vs popped '%c' -> %s\n",
                       i, c, openTop,
                       isMatchingPair(openTop, c) ? "match" : "MISMATCH");
            }
            if (!isMatchingPair(openTop, c)) {
                return 0;
            }
        }
    }

    if (!isEmpty()) {
        if (verbose) printf("  stack not empty at end -> unclosed opener(s)\n");
        return 0;
    }
    return 1;
}

int main() {
    char expr[MAX_LEN];
    char again;

    printf("Balanced Parentheses Checker\n\n");

    do {
        printf("Enter expression (no spaces, max %d chars): ", MAX_LEN - 1);
        scanf("%199s", expr);

        printf("\nTrace:\n");
        if (isBalanced(expr, 1)) {
            printf("=> BALANCED\n");
        } else {
            printf("=> NOT BALANCED\n");
        }

        printf("\nCheck another? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
