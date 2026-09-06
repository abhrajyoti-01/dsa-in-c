/*
Sliding Window Maximum (Monotonic Deque, Master Problem)
--------------------------------------------------------
What this program solves:
For every window of size k in an array, reports the MAXIMUM - in
O(n) total using a monotonic DEQUE (vs O(n*k) brute force).

Real life example:
Live dashboards ("max latency in the last 5 minutes") and trading
systems compute rolling maximums in O(n) with a monotonic deque.

DIAGRAM ([1 3 -1 -3 5 3 6 7], k=3, deque holds indexes, values decrease):
  window [1 3 -1]  deque: 3,-1      -> max 3
  slide: -3 enters, -1 stays       -> max 3
  5 enters: pops -1,-3 (smaller)   -> max 5
  6 enters: pops 5, 3              -> max 6
  7 enters: pops 6                 -> max 7
  front of the deque is ALWAYS the window maximum.

BEGINNER EXPLANATION:
Keep a deque of INDEXES whose values are DECREASING. When the
window slides right by one:
1. Front of deque is out of the window? Drop it (it is too old).
2. New element pops every deque element SMALLER than it - they can
   never be a future maximum (a bigger, newer element shadows them).
3. Push the new index. The FRONT is always the current window max.
Each index enters and leaves the deque exactly once -> O(n).

SUPER SIMPLE EXAMPLE ([1,3,-1,-3,5,3,6,7], k = 3):
Window [1,3,-1] -> deque [3@1,-1@2] -> max 3
slide to [3,-1,-3]: 3@1 still in window -> max 3
slide to [-1,-3,5]: 5 pops -1,-3 -> deque [5@4] -> max 5
slide to [-3,5,3]: 3@5 < 5 -> pushed -> max 5
slide to [5,3,6]: 6 pops all -> max 6
slide to [3,6,7]: 7 pops all -> max 7
Answer: 3 3 5 5 6 7.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- k = 1: every element is its own window max - deque stays tiny.
- k = n: one answer, the global maximum.
- Increasing input [1,2,3,4]: every new element pops the whole
  deque (all older smaller) -> deque holds just the newest - the
  fast path.
- Decreasing input [5,4,3,2]: nothing pops -> deque fills with
  everything, but evictions from the FRONT handle aging - still
  O(n) amortized.
- k > n or k <= 0: invalid - reject.

DRY RUN (same example, window 2):
i=0: push 1@0. deque [1@0]
i=1: 3 pops 1@0; push 3@1. window full -> front 3@1 in [0..1]
     -> max 3.
i=2: front 3@1: window [0..2]? i-k+1 = 0 -> 1 >= 0 stays.
     -1@2 pushed. max = 3.
i=3: evict? front 3@1 >= 1 (i-k+1=1) stays. -3 pushed. max 3.
i=4: front 3@1 < 1? i-k+1 = 2 -> 1 < 2 evict. push 5 (pops -1,-3).
     max 5.

TIPS:
- This is the "monotonic queue" - a deque used like the monotonic
  stack but with BOTH-end maintenance: head evicts expired, tail
  evicts dominated. Same trick gives sliding window MINIMUM
  (flip the comparison).
- Appears in: DP optimization (constrained transitions), network
  rate limiting, stock/chart analysis.
- Brute force O(n*k) first, then optimize - the interview wants
  to SEE that progression.
*/

#include <stdio.h>

#define MAX_N 500

int main() {
    int arr[MAX_N];
    int dequeIdx[MAX_N];
    int n;
    int k;

    printf("Sliding Window Maximum (monotonic deque)\n\n");

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

    printf("Window size k (1-%d)? ", n);
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("k must be 1..%d\n", n);
        return 1;
    }

    int front = 0;
    int rear = -1;
    int results[MAX_N];
    int resultCount = 0;

    /* STEP: slide the window, maintaining decreasing deque */
    for (int i = 0; i < n; i++) {
        if (front <= rear && dequeIdx[front] <= i - k) {
            front = front + 1;
        }
        while (front <= rear && arr[i] >= arr[dequeIdx[rear]]) {
            rear = rear - 1;
        }
        rear = rear + 1;
        dequeIdx[rear] = i;

        if (i >= k - 1) {
            int maxVal = arr[dequeIdx[front]];
            results[resultCount] = maxVal;
            resultCount = resultCount + 1;
            printf("Window [%d..%d]: max %d (deque holds indexes",
                   i - k + 1, i, maxVal);
            for (int j = front; j <= rear; j++) {
                printf(" %d", dequeIdx[j]);
            }
            printf(")\n");
        }
    }

    printf("\nAll window maximums: [");
    for (int i = 0; i < resultCount; i++) {
        printf("%d", results[i]);
        if (i < resultCount - 1) printf(", ");
    }
    printf("]\n");

    return 0;
}
