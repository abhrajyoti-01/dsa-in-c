/*
Next Greater Element & Stock Span (Monotonic Stack, Basic -> Hard)
-------------------------------------------------------------------
Real life example:
The stock span is literally today's problem: "for how many days
has this price been the highest?" Monotonic stacks also power
"next warmer day" weather and price alerts.

DIAGRAM (next greater in [4, 5, 2, 25]):
  4 -> waits (stack [4])
  5 -> 5 > 4: 4's answer = 5, push 5
  2 -> 2 < 5: push (stack [5 2])
  25 -> pops 2 (ans 25), pops 5 (ans 25)
  each element is pushed and popped once -> O(n).

LADDER (easy -> hard inside this file):
  1. Next Greater Element (right side)    (brute O(n^2) -> stack O(n))
  2. Stock Span (consecutive smaller days on the LEFT)

BEGINNER EXPLANATION:
Both problems share one idea - the MONOTONIC STACK: keep a stack
whose values are decreasing (or increasing). A new element POPS
everything smaller than it - each popped element just found its
"next greater" (or its span ends here). Each index is pushed and
popped at most once -> O(n) total, vs O(n^2) double loops.

SUPER SIMPLE EXAMPLE (next greater, [4, 5, 2, 25]):
4 pushed. 5 arrives: 5 > 4 -> pop 4 (answer for index0 = 5); push 5.
2 pushed (smaller). 25 arrives: pop 2 (answer 25), pop 5 (answer
25); push 25. End: leftovers have no greater element (-1).
Answers: [5, 25, 25, -1].

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Decreasing input [5,4,3,2]: nothing ever pops during arrival;
  at the END all remain unpopped -> every answer is -1.
- Equal elements: "next strictly greater" vs ">=": use strict >
  for popping; ties stay on the stack. Define your tie rule first!
- Stock span of day 0 is always 1 (itself).
- Circular next-greater (wrap-around) variant: run the loop
  TWICE over indices i % n - try as an extension.

DRY RUN (stock span, prices [100, 80, 60, 70, 60, 75, 85]):
100: stack empty -> span 1. stack [100@0]
80:  top 100 > 80 -> span 1. stack [100@0, 80@1]
60:  span 1. stack [..., 60@2]
70:  pop 60@2 (span gain 1), top 80@1 > 70 -> span = 3-1 = 2
     (days 2,3). stack [100@0, 80@1, 70@3]
60:  span 1.
75:  pop 60@5, pop 70@3 -> stop at 80@1 -> span = 6-1 = 5
85:  pop 75?no wait - after 75 push, stack [100@0,80@1,75@6];
     85 pops 75@6, 80@1 -> stop at 100@0 -> span = 7-0 = 7.
Spans: 1 1 1 2 1 4 6. Matches the classic example.

TIPS:
- The monotonic stack amortization: total pops <= total pushes =
  n -> O(n) even though the inner pop-loop LOOKS nested.
- Next-greater-left/next-smaller-* are the same code with one
  comparison sign flipped and direction reversed - four problems
  for the price of one template.
- Sliding window maximum (next file) is the deque cousin of this
  exact technique - study them back to back.
*/

#include <stdio.h>

#define MAX_N 200

/*
 * nextGreater - Right-side next strictly greater, O(n).
 */
void nextGreater(const int arr[], int n, int result[]) {
    int stack[MAX_N];
    int top = -1;

    for (int i = 0; i < n; i++) {
        result[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        while (top >= 0 && arr[i] > arr[stack[top]]) {
            result[stack[top]] = arr[i];
            top = top - 1;
        }
        top = top + 1;
        stack[top] = i;
    }
}

/*
 * bruteForceNG - O(n^2) baseline for comparison.
 */
void bruteForceNG(const int arr[], int n, int result[]) {
    for (int i = 0; i < n; i++) {
        result[i] = -1;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[i]) {
                result[i] = arr[j];
                break;
            }
        }
    }
}

/*
 * stockSpan - Spans via index stack, O(n).
 */
void stockSpan(const int prices[], int n, int span[]) {
    int stack[MAX_N];
    int top = -1;

    for (int i = 0; i < n; i++) {
        while (top >= 0 && prices[stack[top]] <= prices[i]) {
            top = top - 1;
        }
        span[i] = (top == -1) ? (i + 1) : (i - stack[top]);
        top = top + 1;
        stack[top] = i;
    }
}

int main() {
    int arr[MAX_N];
    int result[MAX_N];
    int result2[MAX_N];
    int span[MAX_N];
    int n;
    int choice;

    printf("Next Greater Element & Stock Span (easy -> hard)\n\n");

    printf("How many elements (1-%d)? ", MAX_N);
    scanf("%d", &n);

    if (n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Array: [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");

    while (1) {
        printf("\n1. Next greater element (stack, O(n))\n");
        printf("2. Next greater element (brute force O(n^2) check)\n");
        printf("3. Stock span\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            nextGreater(arr, n, result);
            printf("Next greater: [");
            for (int i = 0; i < n; i++) {
                printf("%d", result[i]);
                if (i < n - 1) printf(", ");
            }
            printf("]\n");
        } else if (choice == 2) {
            bruteForceNG(arr, n, result2);
            nextGreater(arr, n, result);
            printf("Brute:  [");
            for (int i = 0; i < n; i++) {
                printf("%d", result2[i]);
                if (i < n - 1) printf(", ");
            }
            printf("]\n");
            printf("Stack:  [");
            for (int i = 0; i < n; i++) {
                printf("%d", result[i]);
                if (i < n - 1) printf(", ");
            }
            printf("]\n");
            int same = 1;
            for (int i = 0; i < n; i++) {
                if (result[i] != result2[i]) {
                    same = 0;
                }
            }
            printf("Agree: %s\n", same ? "YES" : "NO");
        } else if (choice == 3) {
            stockSpan(arr, n, span);
            printf("Spans:  [");
            for (int i = 0; i < n; i++) {
                printf("%d", span[i]);
                if (i < n - 1) printf(", ");
            }
            printf("]\n");
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
