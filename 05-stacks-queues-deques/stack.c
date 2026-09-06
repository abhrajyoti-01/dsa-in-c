/*
Stack (LIFO - Last In, First Out)
---------------------------------
What this program solves:
Menu-driven implementation of a Stack using a fixed-size array with
user input for push, pop, peek, display, and overflow/underflow
handling.

Real life example:
A stack of plates in a canteen. You always place a new plate on top
and always take a plate from the top. The last plate placed is the
first one removed (LIFO). Browsers use the same idea for the Back
button, and editors use it for Undo.

DIAGRAM (array of 5 slots, after push 10, 20, 30):
        +-----+
 top=2  |  30 |  <- top: push/pop happen HERE (O(1))
        +-----+
 top=1  |  20 |
        +-----+
 top=0  |  10 |
        +-----+
 top=-1 |  .  |  <- initial state (empty)
        +-----+
          index 0 is the BOTTOM - nothing below it.

PUSH(40):  top 2 -> 3, store 40 at index 3
POP():     read stack[2] = 30, top 2 -> 1

How the process works:
1. top starts at -1 (empty stack).
2. PUSH: increment top, then store the value at stack[top].
   If top is already at MAX-1, the stack is FULL -> overflow.
3. POP: read stack[top], then decrement top.
   If top is already -1, the stack is EMPTY -> underflow.
4. PEEK: read stack[top] without removing it.

Pseudo code:
START
PUSH(value)
    If top == MAX-1: report overflow, stop
    top = top + 1
    stack[top] = value
POP()
    If top == -1: report underflow, stop
    value = stack[top]
    top = top - 1
    Return value
END

Sample input and dry run:
Choice 1, value 10 -> stack: [10]
Choice 1, value 20 -> stack: [10, 20]
Choice 4           -> prints 20 10 (top to bottom)
Choice 3           -> Top element: 20
Choice 2           -> Popped 20
Choice 5           -> exit

Main logic to understand:
All activity happens at one end (the top), so both push and pop take
only O(1) time - no shifting of elements is ever needed.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 5

int stack[MAX];
int top = -1;

/*
 * isEmpty - Returns 1 when the stack has no elements.
 */
int isEmpty() {
    return top == -1;
}

/*
 * isFull - Returns 1 when the stack cannot accept more elements.
 */
int isFull() {
    return top == MAX - 1;
}

/*
 * push - Places a value on top of the stack.
 */
void push(int value) {
    if (isFull()) {
        printf("Overflow! Cannot push %d (stack is full)\n", value);
        return;
    }
    top = top + 1;
    stack[top] = value;
    printf("Pushed %d\n", value);
}

/*
 * pop - Removes and returns the top value of the stack.
 */
int pop() {
    if (isEmpty()) {
        printf("Underflow! Cannot pop (stack is empty)\n");
        return -1;
    }
    int value = stack[top];
    top = top - 1;
    printf("Popped %d\n", value);
    return value;
}

/*
 * peek - Shows the top value without removing it.
 */
void peek() {
    if (isEmpty()) {
        printf("Stack is empty\n");
    } else {
        printf("Top element: %d\n", stack[top]);
    }
}

/*
 * display - Prints the stack from top to bottom.
 */
void display() {
    if (isEmpty()) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack (top -> bottom): ");
    for (int i = top; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

int main() {
    int choice, value;

    while (1) {
        printf("\nStack (Array Implementation) - LIFO\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display stack\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter value to push: ");
            scanf("%d", &value);
            push(value);
        } else if (choice == 2) {
            pop();
        } else if (choice == 3) {
            peek();
        } else if (choice == 4) {
            display();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}