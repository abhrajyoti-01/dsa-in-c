/*
Postfix (RPN) Evaluation - Stack Application
--------------------------------------------
What this program solves:
Evaluates a postfix expression like "23*54*+" directly with one
stack - no parentheses, no precedence rules needed. This is the
reverse of infix_to_postfix.c, and together they show why postfix
exists.

Real life example:
RPN calculators (HP), Forth/PostScript and bytecode VMs evaluate
exactly this way - operands on a stack, operators consume them.

DIAGRAM (evaluate "23*4+"):
  2 push   stack [2]
  3 push   stack [2 3]
  * pop 3,2 -> 6  stack [6]
  4 push   stack [6 4]
  + pop 4,6 -> 10 stack [10]   -> result 10

BEGINNER EXPLANATION (the simplest way to think):
Walk left to right. See a NUMBER? Put it on the pile (stack).
See an OPERATOR? Pull the TOP TWO numbers, apply the operator,
put the RESULT back. At the end the pile holds exactly one number:
the answer. Operands pop in reverse order - second-popped goes
LEFT of the operator for -, /, ^.

SUPER SIMPLE EXAMPLE:
"23*4+"  (which means 2*3+4 = 10)
2 -> push. pile: [2]
3 -> push. pile: [2, 3]
* -> pop 3, pop 2 -> 2*3 = 6 -> push. pile: [6]
4 -> push. pile: [6, 4]
+ -> pop 4, pop 6 -> 6+4 = 10 -> push. pile: [10]
Answer: 10

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Subtraction/division ORDER: "53-" = 5-3 = 2 (NOT 3-5). Pop
  order matters: first pop = RIGHT operand, second pop = LEFT.
  Getting this backwards is the #1 bug here.
- Division by zero: "60/" -> report an error, do not crash.
- Too many operands ("234+" leaves 2,4 on the pile): expression is
  malformed - report leftover operands.
- Too many operators ("2+" tries to pop with 1 value): stack
  underflow -> malformed.
- Single number "7" -> answer 7 (valid postfix).
- Negative results and intermediate negatives are fine: "53-2*"
  = (5-3)*2 = 4.

DRY RUN ("5 3 - 2 *" -> "53-2*"):
5 -> push. pile: [5]
3 -> push. pile: [5, 3]
- -> pop 3 (right), pop 5 (left) -> 5-3 = 2 -> push. pile: [2]
2 -> push. pile: [2, 2]
* -> pop 2, pop 2 -> 2*2 = 4 -> push. pile: [4]
Answer: 4

TIPS:
- Why does the industry use RPN? Calculators (HP), stack VMs
  (JVM, Forth, PostScript) - because evaluation is ONE pass, no
  grammar, no lookahead.
- Combined with infix_to_postfix.c you now have a complete
  expression engine: read infix -> convert -> evaluate. Try
  chaining them manually on "(8+2)*5" -> 82+5* -> 50.
- For multi-digit numbers, real implementations tokenize first -
  here single digits keep the idea visible.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 200

long long stackArr[MAX_LEN];
int top = -1;

void push(long long v) {
    top = top + 1;
    stackArr[top] = v;
}

long long pop(void) {
    long long v = stackArr[top];
    top = top - 1;
    return v;
}

int isEmpty(void) {
    return top == -1;
}

/*
 * applyOp - b op a  (a was pushed first = LEFT operand).
 */
long long applyOp(char op, long long a, long long b, int* error) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                *error = 1;
                return 0;
            }
            return a / b;
        case '^': {
            long long r = 1;
            for (long long i = 0; i < b; i++) {
                r = r * a;
            }
            return r;
        }
    }
    *error = 1;
    return 0;
}

int main() {
    char expr[MAX_LEN];
    char again;

    printf("Postfix (RPN) Evaluation\n\n");
    printf("Single-digit operands, operators + - * / ^\n");
    printf("Example: 23*4+  (means 2*3+4)\n\n");

    do {
        printf("Enter postfix expression: ");
        scanf("%199s", expr);

        top = -1;
        int error = 0;
        int malformed = 0;

        printf("\nStep-by-step:\n");
        for (int i = 0; expr[i] != '\0' && !error && !malformed; i++) {
            char c = expr[i];

            if (isdigit((unsigned char)c)) {
                push(c - '0');
                printf("  '%c' -> push        pile: ", c);
                for (int j = 0; j <= top; j++) {
                    printf("%lld ", stackArr[j]);
                }
                printf("\n");
            } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                if (top < 1) {
                    printf("  '%c' -> NOT ENOUGH operands -> malformed\n", c);
                    malformed = 1;
                    break;
                }
                long long b = pop();
                long long a = pop();
                long long r = applyOp(c, a, b, &error);
                if (error) {
                    printf("  division by zero ERROR\n");
                    break;
                }
                push(r);
                printf("  '%c' -> %lld %c %lld = %lld   pile: ", c, a, c, b, r);
                for (int j = 0; j <= top; j++) {
                    printf("%lld ", stackArr[j]);
                }
                printf("\n");
            } else {
                printf("Invalid character '%c'\n", malformed = 1);
                break;
            }
        }

        if (!error && !malformed) {
            if (top == 0) {
                printf("\nAnswer: %lld\n", stackArr[0]);
            } else if (top > 0) {
                printf("\nLeftover operands on the pile -> malformed expression\n");
            } else {
                printf("\nNothing to evaluate\n");
            }
        }

        printf("\nEvaluate another? (y/n): ");
        scanf(" %c", &again);
        printf("\n");
    } while (again == 'y' || again == 'Y');

    return 0;
}
