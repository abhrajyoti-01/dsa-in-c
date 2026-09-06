/*
Stack Variants - Min Stack & Two Stacks (Basic -> Hard)
--------------------------------------------------------
Real life example:
Min Stack mirrors how trading apps show "lowest price today" even
after prices change; two-stacks-in-one-array splits a fixed buffer
into two stacks - a classic embedded memory trick.

DIAGRAM (min stack tracks a parallel minimum per push):
  push 5  main [5]      min [5]
  push 3  main [5 3]    min [5 3]
  push 7  main [5 3 7]  min [5 3]   <- 7 not pushed to min
  pop 7   min unchanged; getMin() -> 3
  two stacks in one array: [1 2 3 | 9 8] top1->, <-top2.

LADDER (easy -> hard inside this file):
  1. Two stacks in one array   (grow from both ends)
  2. Min stack                 (get minimum in O(1) with auxiliary stack)

BEGINNER EXPLANATION:
- Two stacks: one grows up from index 0, the other grows DOWN from
  index MAX-1. They collide only when the array is genuinely full -
  space is shared optimally (no fixed half/half split).
- Min stack: a normal stack PLUS a second stack that stores the
  minimum AT EACH PUSH. Pop from both together. The top of the
  min-stack is always the current minimum - computing min by
  scanning would be O(n); the auxiliary stack makes it O(1).

SUPER SIMPLE EXAMPLE (min stack):
push 5 -> main [5]      min [5]
push 2 -> main [5,2]    min [5,2]
push 7 -> main [5,2,7]  min [5,2]     (7 > 2, min-stack unchanged)
pop   -> main [5,2]     min [5,2]
pop   -> main [5]       min [5]      (2 removed -> min back to 5)
getMin always reads min-stack top: 5. Correct after every step.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Equal minimum values: push to min-stack WHEN newMin <= minTop
  (the <= matters!): pop equal mins one per pop. Using < loses an
  entry and getMin breaks after popping duplicates.
- Popping an empty stack: guard with isEmpty - underflow.
- Two stacks overflow when tops CROSS (top1 + 1 == top2) - that
  single condition IS "array full".
- Min stack stores DUPLICATES of the running min - memory O(n)
  worst case (decreasing sequence). A pair-storing variant exists
  with fewer entries.

DRY RUN (two stacks, MAX 6):
push1(10): stack1 top=0. push1(20): top=1.
push2(30): stack2 top=5. push2(40): top=4.
State: [10, 20, _, _, 40, 30] - one array, two logical stacks.
push1(50), push1(60): top1=3; push2(70) -> top2=3 == top1+1 ->
CROSS -> overflow. Correct detection.

TIPS:
- Min-stack pattern ("auxiliary stack mirrors the state") solves
  "min queue", "max sliding window" (see next file) - learn the
  mirroring idea, not just the code.
- Two-stack-in-one-array is the memory layout behind "two ends of
  a train" problems and browser forward/back histories.
- All variants keep O(1) per operation - the defining constraint
  of the stack ADT; if an op is O(n), redesign.
*/

#include <stdio.h>

#define MAX 10

/* ---------- Variant 1: two stacks in one array ---------- */
int shared[MAX];
int top1 = -1;
int top2 = MAX;

/*
 * push1 / pop1 - First stack grows upward from index 0.
 */
void push1(int value) {
    if (top1 + 1 == top2) {
        printf("Overflow (stacks collided)\n");
        return;
    }
    top1 = top1 + 1;
    shared[top1] = value;
    printf("stack1 <- %d (top1=%d)\n", value, top1);
}

int pop1(void) {
    if (top1 == -1) {
        printf("stack1 underflow\n");
        return -1;
    }
    int v = shared[top1];
    top1 = top1 - 1;
    printf("stack1 -> %d (top1=%d)\n", v, top1);
    return v;
}

/*
 * push2 / pop2 - Second stack grows downward from MAX-1.
 */
void push2(int value) {
    if (top2 - 1 == top1) {
        printf("Overflow (stacks collided)\n");
        return;
    }
    top2 = top2 - 1;
    shared[top2] = value;
    printf("stack2 <- %d (top2=%d)\n", value, top2);
}

int pop2(void) {
    if (top2 == MAX) {
        printf("stack2 underflow\n");
        return -1;
    }
    int v = shared[top2];
    top2 = top2 + 1;
    printf("stack2 -> %d (top2=%d)\n", v, top2);
    return v;
}

/* ---------- Variant 2: min stack ---------- */
int mainStack[MAX];
int minStack[MAX];
int mainTop = -1;
int minTop = -1;

/*
 * minPush - Push value; mirror onto min-stack on ties too.
 */
void minPush(int value) {
    mainTop = mainTop + 1;
    mainStack[mainTop] = value;

    if (minTop == -1 || value <= minStack[minTop]) {
        minTop = minTop + 1;
        minStack[minTop] = value;
    }
    printf("push %d (min = %d)\n", value,
           minTop >= 0 ? minStack[minTop] : value);
}

int minPop(void) {
    if (mainTop == -1) {
        printf("Min stack underflow\n");
        return -1;
    }
    int v = mainStack[mainTop];
    mainTop = mainTop - 1;
    if (minTop >= 0 && v == minStack[minTop]) {
        minTop = minTop - 1;
    }
    printf("pop %d (min now %s)\n", v,
           minTop >= 0 ? "updated below" : "stack empty");
    return v;
}

int minGetMin(void) {
    if (minTop == -1) {
        printf("Stack empty\n");
        return -1;
    }
    return minStack[minTop];
}

int main() {
    int choice;
    int value;

    printf("Stack Variants (easy -> hard)\n\n");

    while (1) {
        printf("\n1. Two stacks: push stack1\n");
        printf("2. Two stacks: pop stack1\n");
        printf("3. Two stacks: push stack2\n");
        printf("4. Two stacks: pop stack2\n");
        printf("5. Min stack: push\n");
        printf("6. Min stack: pop\n");
        printf("7. Min stack: getMin (O(1))\n");
        printf("8. Show shared array state\n");
        printf("9. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Value: ");
            scanf("%d", &value);
            push1(value);
        } else if (choice == 2) {
            pop1();
        } else if (choice == 3) {
            printf("Value: ");
            scanf("%d", &value);
            push2(value);
        } else if (choice == 4) {
            pop2();
        } else if (choice == 5) {
            printf("Value: ");
            scanf("%d", &value);
            minPush(value);
        } else if (choice == 6) {
            minPop();
        } else if (choice == 7) {
            printf("Min = %d\n", minGetMin());
        } else if (choice == 8) {
            printf("Shared array: [");
            for (int i = 0; i < MAX; i++) {
                if (i <= top1 || i >= top2) {
                    printf("%d", shared[i]);
                } else {
                    printf("_");
                }
                if (i < MAX - 1) printf(" ");
            }
            printf("]\n");
        } else if (choice == 9) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
