/*
Loops - for, while, do-while, nested loops, and loop control
------------------------------------------------------------
What this program demonstrates:
The three loop forms in C, when each is useful, nested loops, and
how break and continue change the flow of a loop.

A loop repeats a block while its condition is true:
- for:       best when the number of repetitions is known.
- while:     checks before each repetition; useful when repetition
             depends on a condition or input.
- do-while:  checks after each repetition; always runs at least once.

FOR loop anatomy:
for (initialization; condition; update) {
    repeated statements;
}
Initialization runs once; condition is checked before each pass;
update runs after each pass.

DIAGRAM:
  initialize -> condition? -- no --> done
                    |
                   yes
                    v
                  body -> update -> condition?

NESTED LOOPS:
The inner loop completes all its iterations for every one iteration
of the outer loop. A 2-by-3 nested loop therefore runs its body 6 times.

LOOP CONTROL:
- break exits the nearest enclosing loop immediately.
- continue skips the rest of this pass and begins the next pass.

This file focuses on loop mechanics. Practice problems and their
solutions are kept separately in loop-problems.c.
*/

#include <stdio.h>

int main() {
    printf("Loop Forms and Control Flow\n\n");

    /* FOR: use when the iteration count or range is known. */
    printf("1. for loop (count from 1 through 5):\n");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n\n");

    /* WHILE: test first, then run the body. */
    printf("2. while loop (count down from 3):\n");
    int count = 3;
    while (count > 0) {
        printf("%d ", count);
        count--;
    }
    printf("\n\n");

    /* DO-WHILE: the body runs once before the condition is tested. */
    printf("3. do-while loop (body runs once even when condition is false):\n");
    int value = 5;
    do {
        printf("value = %d; condition value < 5 is false\n", value);
    } while (value < 5);
    printf("\n");

    /* NESTED LOOPS: print a small row-and-column coordinate grid. */
    printf("4. nested loops (2 rows by 3 columns):\n");
    for (int row = 1; row <= 2; row++) {
        for (int column = 1; column <= 3; column++) {
            printf("(%d,%d) ", row, column);
        }
        printf("\n");
    }
    printf("\n");

    /* break exits the loop as soon as the condition is met. */
    printf("5. break (stop when i reaches 4):\n");
    for (int i = 1; i <= 6; i++) {
        if (i == 4) {
            break;
        }
        printf("%d ", i);
    }
    printf("\n\n");

    /* continue skips printing the even values, but keeps looping. */
    printf("6. continue (print only odd values from 1 through 6):\n");
    for (int i = 1; i <= 6; i++) {
        if (i % 2 == 0) {
            continue;
        }
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}
