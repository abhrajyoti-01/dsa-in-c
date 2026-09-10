/*
Infix to Postfix Conversion (Shunting Yard - Stack Application)
---------------------------------------------------------------
What this program solves:
Converts a normal (infix) expression like "A+B*C" into postfix
"ABC*+" using the shunting-yard algorithm - how compilers and
calculators remove the need for parentheses at runtime.

Real life example:
Calculators and expression engines (Excel, SQL query planners)
convert infix to postfix once, then evaluate fast without worrying
about parentheses again.

DIAGRAM (A + B * C -> ABC*+):
  A         out: A        stack: [
  +         out: A        stack: [+]
  B         out: AB       stack: [+]
  * (>) +   out: AB       stack: [+ *]
  C         out: ABC      stack: [+ *]
  end: pop all -> ABC*+    higher precedence rides ON TOP.

BEGINNER EXPLANATION (the simplest way to think):
Numbers (operands) go straight to the OUTPUT. Operators wait in a
stack, and each new operator POPS every stack operator that has
equal-or-higher precedence (they act first - like * before +).
'(' is a wall: nothing pops past it. ')' pops everything down to
that wall. Precedence: ^ (3) > * / (2) > + - (1).

SUPER SIMPLE EXAMPLE:
"A+B*C"
A -> output. + -> push. B -> output. * -> * beats + so push.
C -> output. End: pop * then +.
Postfix: A B C * +    (read it: A, then B C * [B*C], then +)
No parentheses needed - order is built into the string.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- "(A+B)*C": the ')' forces '+' out early -> A B + C *.
  Notice how parentheses CHANGE the output - that is their job.
- "^" is RIGHT-associative: A^B^C -> A B C ^ ^ (not A B ^ C ^).
  That is why the comparison for '^' pops only STRICTLY greater
  precedence, but '+','*','/' pop equal-or-greater.
- Equal precedence left-associative: "A-B+C" -> A B - C +
  (subtraction must pop the earlier '-' when '+' arrives).
- Single operand "A" -> just "A". Nothing to convert.
- Invalid ')' without '(': stack underflow - the program reports
  an error rather than crashing.

DRY RUN ("(A+B)*C-D"):
( -> push
A -> output: A
+ -> push (on top of ()
B -> output: A B
) -> pop to '(': output: A B +   (discard the parens)
* -> push
C -> output: A B + C
- -> * >= - so pop *: output: A B + C *  then push -
D -> output: A B + C * D
End: pop - -> A B + C * D -
Postfix: A B + C * D -

TIPS:
- Why postfix? It needs NO parentheses and NO precedence rules at
  evaluation time - a single left-to-right scan with one stack
  evaluates it (see postfix_evaluation.c).
- Associativity is the detail everyone forgets: ^ right, rest
  left. Test "A^B^C" and "A-B+C" to verify your implementation.
- This is the same algorithm Python/JS use internally (shunting
  yard) - recognizing it makes parsing feel less magical.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 200

char stackArr[MAX_LEN];
char output[MAX_LEN];
int top = -1;

void push(char c) {
    top = top + 1;
    stackArr[top] = c;
}

char pop(void) {
    char c = stackArr[top];
    top = top - 1;
    return c;
}

char peek(void) {
    return stackArr[top];
}

int isEmpty(void) {
    return top == -1;
}

/*
 * precedence - ^ = 3, * / = 2, + - = 1.
 */
int precedence(char op) {
    if (op == '^') {
        return 3;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    if (op == '+' || op == '-') {
        return 1;
    }
    return 0;
}

/*
 * isRightAssociative - Only ^ is right-associative here.
 */
int isRightAssociative(char op) {
    return op == '^';
}

/*
 * infixToPostfix - Shunting yard with running trace.
 *
 * Writes the postfix form into `postfix` (the caller's buffer) and also
 * keeps the identical running trace in `output` so a partially built
 * string can be printed at every step. Both stay in sync, and the
 * result is terminated in `postfix` before returning.
 */
int infixToPostfix(const char* infix, char* postfix, int verbose) {
    top = -1;
    int out = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];

        if (isalnum((unsigned char)c)) {
            output[out] = c;
            postfix[out] = c;
            out = out + 1;
            if (verbose) printf("  '%c' operand        -> output: %s\n", c, output);
        } else if (c == '(') {
            push(c);
            if (verbose) printf("  '(' push           -> stack top: %c\n", peek());
        } else if (c == ')') {
            if (verbose) printf("  ')' pop to '('     ");
            while (!isEmpty() && peek() != '(') {
                output[out] = pop();
                postfix[out] = output[out];
                out = out + 1;
            }
            if (isEmpty()) {
                printf("-> unmatched ')' ERROR\n");
                return 0;
            }
            pop();
            if (verbose) printf("-> output: %s\n", output);
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (!isEmpty() && peek() != '(') {
                char topOp = peek();
                int topPrec = precedence(topOp);
                int cPrec = precedence(c);
                int shouldPop = (topPrec > cPrec) ||
                                (topPrec == cPrec && !isRightAssociative(c));
                if (!shouldPop) {
                    break;
                }
                output[out] = pop();
                postfix[out] = output[out];
                out = out + 1;
            }
            push(c);
            if (verbose) printf("  '%c' operator       -> stack top: %c, output: %s\n", c, peek(), output);
        } else {
            printf("Invalid character '%c'\n", c);
            return 0;
        }
    }

    while (!isEmpty()) {
        if (peek() == '(') {
            printf("Unmatched '(' ERROR\n");
            return 0;
        }
        output[out] = pop();
        postfix[out] = output[out];
        out = out + 1;
    }
    output[out] = '\0';
    postfix[out] = '\0';
    return 1;
}

int main() {
    char infix[MAX_LEN];
    char again;

    printf("Infix to Postfix (Shunting Yard)\n\n");
    printf("Use single letters/digits as operands. Example: (A+B)*C-D\n\n");

    do {
        printf("Enter infix expression: ");
        scanf("%199s", infix);

        if (infixToPostfix(infix, output, 1)) {
            printf("\nInfix:   %s\n", infix);
            printf("Postfix: %s\n", output);
        }

        printf("\nConvert another? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
