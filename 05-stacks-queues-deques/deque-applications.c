/*
Deque Problems Ladder - Applications (Basic -> Hard)
-----------------------------------------------------
Real life example:
Browser history and undo lists trim old entries at one end while
adding at the other; A/C or fan controllers track "first negative"
sensor reading per time window with a deque.

DIAGRAM (first negative in window k=3, [-2 3 -1 4]):
  window [-2 3 -1] -> front -2 is negative -> answer -2
  window [3 -1 4]  -> -2 left the window (front expired) -> -1
  a deque of CANDIDATE indexes keeps the front always valid.

LADDER (easy -> hard inside this file):
  1. First negative in every window of size k
  2. Sum of min and max of every window of size k
  3. Palindrome check using a deque (both ends)

BEGINNER EXPLANATION:
A deque lets you push/pop at BOTH ends - perfect for windows:
- Negative-in-window: keep a queue of indexes of negative numbers
  in the current window; front is the FIRST negative. Evict the
  front when it slides out.
- Window min/max sum: the monotonic deque from sliding-window-maximum
  twice (once for min, once for max), adding results.
- Palindrome: load the string, pop from both ends comparing - a
  deque IS the two-pointer palindrome.

SUPER SIMPLE EXAMPLE (first negative, [12,-1,-7,8,-15,30,16], k=3):
Window [12,-1,-7]: first negative -1.
[-1,-7,8]: -1. [-7,8,-15]: -7. [8,-15,30]: -15. [-15,30,16]: -15.
Answers: -1 -1 -7 -15 -15.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- No negative in a window: print 0 (the conventional answer).
- k = 1: answers are just the array with 0s for non-negatives.
- Min+max sum with all equal values: min == max == value - sum is
  2*value per window.
- Palindrome with odd length: middle char pops alone (left ==
  right) - the while loop handles it naturally.
- Empty deque mid-algorithm: the guards (front > rear) keep the
  monotonic logic safe.

DRY RUN (window min/max sum, [2, 5, -1, 7], k = 2):
[2,5]: min 2, max 5 -> sum 7.
[5,-1]: min -1, max 5 -> 4.
[-1,7]: min -1, max 7 -> 6.
Total of window sums: 7, 4, 6 (program prints per-window).

TIPS:
- The monotonic deque pattern is one of the most reusable in
  competitive programming: window max, min, first-negative, and
  DP transition optimization all use it.
- Palindrome-via-deque is how you check palindromes in linked
  lists too (load into a deque, pop both ends).
- These applications are why deque exists as a separate ADT -
  a queue with memory of both ends.
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 200

int dq[MAX_N];

int main() {
    int arr[MAX_N];
    int n;
    int k;
    int choice;

    printf("Deque Applications Ladder (easy -> hard)\n\n");

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
        printf("k out of range\n");
        return 1;
    }

    while (1) {
        printf("\n1. First negative in every window\n");
        printf("2. Sum of min+max in every window\n");
        printf("3. Palindrome check via deque\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int negQueue[MAX_N];
            int front = 0;
            int rear = -1;
            printf("First negatives: ");
            for (int i = 0; i < n; i++) {
                if (arr[i] < 0) {
                    rear = rear + 1;
                    negQueue[rear] = i;
                }
                if (i >= k - 1) {
                    while (front <= rear && negQueue[front] <= i - k) {
                        front = front + 1;
                    }
                    if (front <= rear) {
                        printf("%d ", arr[negQueue[front]]);
                    } else {
                        printf("0 ");
                    }
                    if (arr[i - k + 1] < 0 && negQueue[front] == i - k + 1) {
                        front = front + 1;
                    }
                }
            }
            printf("\n");
        } else if (choice == 2) {
            int maxDeque[MAX_N];
            int minDeque[MAX_N];
            int maxF = 0;
            int maxR = -1;
            int minF = 0;
            int minR = -1;
            long long grandTotal = 0;
            printf("Per-window (min + max):\n");
            for (int i = 0; i < n; i++) {
                while (maxF <= maxR && arr[i] >= arr[maxDeque[maxR]]) {
                    maxR = maxR - 1;
                }
                while (minF <= minR && arr[i] <= arr[minDeque[minR]]) {
                    minR = minR - 1;
                }
                maxR = maxR + 1;
                maxDeque[maxR] = i;
                minR = minR + 1;
                minDeque[minR] = i;

                if (maxDeque[maxF] <= i - k) {
                    maxF = maxF + 1;
                }
                if (minDeque[minF] <= i - k) {
                    minF = minF + 1;
                }
                if (i >= k - 1) {
                    int mn = arr[minDeque[minF]];
                    int mx = arr[maxDeque[maxF]];
                    printf("  window [%d..%d]: min %d + max %d = %d\n",
                           i - k + 1, i, mn, mx, mn + mx);
                    grandTotal = grandTotal + mn + mx;
                }
            }
            printf("Grand total of all window (min+max): %lld\n",
                   grandTotal);
        } else if (choice == 3) {
            char str[100];
            printf("Enter a word: ");
            scanf("%99s", str);
            int len = (int)strlen(str);
            char dqs[100];
            for (int i = 0; i < len; i++) {
                dqs[i] = str[i];
            }
            int left = 0;
            int right = len - 1;
            int isPal = 1;
            while (left < right) {
                if (dqs[left] != dqs[right]) {
                    isPal = 0;
                    break;
                }
                left = left + 1;
                right = right - 1;
            }
            printf("\"%s\" palindrome: %s\n", str, isPal ? "YES" : "NO");
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
