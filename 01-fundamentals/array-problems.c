/*
Array Problems Ladder (Basic -> Hard)
-------------------------------------
Real life example:
Merging two already-sorted piles of marked answer sheets: compare
only the TOP sheets of each pile - the merge step below. Majority
element is an election: one candidate stays ahead of everyone else
combined (Moore's voting).

DIAGRAM (merge sorted A=[2,5,9] and B=[1,6]):
  compare fronts: 1<2 -> out [1]
  compare fronts: 2<6 -> out [1 2]
  compare fronts: 5<6 -> out [1 2 5]
  compare fronts: 6<9 -> out [1 2 5 6]
  B empty -> pour the rest of A -> [1 2 5 6 9]

LADDER (easy -> hard inside this file):
  1. Sum and average            (traversal)
  2. Second largest             (two-variable trick)
  3. Count even/odd             (condition + count)
  4. Left rotate by 1           (single shift)
  5. Remove duplicates (sorted) (two-pointer write)
  6. Merge two sorted arrays    (merge step of merge sort!)
  7. Find missing number        (sum formula / XOR)
  8. Majority element           (Moore's voting)

BEGINNER EXPLANATION:
Each problem trains one reusable idea:
- Two-variable tracking (best, secondBest) beats sorting for
  "second largest".
- Two-pointer technique: a READ pointer scans, a WRITE pointer
  compacts - the backbone of in-place array processing.
- Merging two sorted arrays = the heart of merge sort - learn it
  here and merge_sort.c becomes obvious.

SUPER SIMPLE EXAMPLE (remove duplicates from sorted):
[1, 1, 2, 2, 3]
read=1: arr[1]==arr[0] skip. read=2: 2!=1 -> write=2: arr[2]=2...
wait - write pointer stays at 1, copy arr[2]->arr[1]. read=3:
2==2 skip. read=4: 3 -> copy to write=2. Result [1,2,3] len 3.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Second largest with DUPLICATES of the max: [5, 5, 3] - is the
  second largest 5 (two people share first) or 3? This program
  uses "strictly smaller than max" -> answer 3. State your
  definition when asked!
- Missing number from 1..n of n-1 elements: sum formula
  n(n+1)/2 - actualSum. Overflow-safe version uses XOR instead:
  XOR(1..n) ^ XOR(given) = missing (order irrelevant).
- Majority element may NOT exist: Moore's voting gives a
  CANDIDATE; a second pass must VERIFY it. Skipping verification
  is the classic bug.
- Merge when one array exhausts first: dump the remainder - the
  two while-loops after the main merge do exactly this.

DRY RUN (Moore's voting, [2,2,1,1,1,2,2]):
candidate=2 count=1; 2 -> count=2; 1 -> count=1; 1 -> count=0
-> reset: candidate=1 count=1 (at element index 3... precisely:
after the second 1 the count hits 0 at index 3, then 1 becomes
candidate); 2 -> count=0? (candidate 1 vs 2 -> count 0) ->
candidate=2 count=1; 2 -> count=2.
Candidate 2, verify: 2 appears 4/7 times > 3.5 -> majority YES.

TIPS:
- Moore's voting is O(n) time O(1) space - the optimal majority
  algorithm; the verification pass is non-negotiable.
- "Merge two sorted arrays" IS the merge step - it returns in
  merge sort (04), closest pair (10), and k-way merging.
- XOR trick for missing number extends to "find the duplicate":
  XOR all + XOR 1..n isolates the repeated value.
*/

#include <stdio.h>

#define MAX 100

void printArray(const int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int arr[MAX];
    int arr2[MAX];
    int merged[2 * MAX];
    int n;
    int n2;
    int choice;

    printf("Array Problems Ladder (easy -> hard)\n\n");

    printf("How many elements (1-%d)? ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Array: ");
    printArray(arr, n);

    while (1) {
        printf("\n1. Sum and average\n");
        printf("2. Second largest (distinct definition)\n");
        printf("3. Count even and odd\n");
        printf("4. Left rotate by 1\n");
        printf("5. Remove duplicates (needs sorted array)\n");
        printf("6. Merge with another sorted array\n");
        printf("7. Find missing number (1..n pattern)\n");
        printf("8. Majority element (Moore's voting)\n");
        printf("9. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            int sum = 0;
            for (int i = 0; i < n; i++) {
                sum = sum + arr[i];
            }
            printf("Sum = %d, Average = %.2f\n", sum, (double)sum / n);
        } else if (choice == 2) {
            int best = -1000000000;
            int second = -1000000000;
            for (int i = 0; i < n; i++) {
                if (arr[i] > best) {
                    second = best;
                    best = arr[i];
                } else if (arr[i] > second && arr[i] != best) {
                    second = arr[i];
                }
            }
            if (second == -1000000000) {
                printf("No distinct second largest exists\n");
            } else {
                printf("Largest = %d, Second largest = %d\n", best, second);
            }
        } else if (choice == 3) {
            int even = 0;
            int odd = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] % 2 == 0) {
                    even = even + 1;
                } else {
                    odd = odd + 1;
                }
            }
            printf("Even: %d, Odd: %d\n", even, odd);
        } else if (choice == 4) {
            if (n > 1) {
                int first = arr[0];
                for (int i = 0; i < n - 1; i++) {
                    arr[i] = arr[i + 1];
                }
                arr[n - 1] = first;
                printf("Rotated: ");
                printArray(arr, n);
            }
        } else if (choice == 5) {
            int write = 1;
            for (int read = 1; read < n; read++) {
                if (arr[read] != arr[write - 1]) {
                    arr[write] = arr[read];
                    write = write + 1;
                }
            }
            printf("After dedup (len %d): ", write);
            printArray(arr, write);
        } else if (choice == 6) {
            printf("How many in second sorted array (1-%d)? ", MAX);
            scanf("%d", &n2);
            printf("Enter %d sorted integers: ", n2);
            for (int i = 0; i < n2; i++) {
                scanf("%d", &arr2[i]);
            }
            int i = 0;
            int j = 0;
            int k = 0;
            while (i < n && j < n2) {
                if (arr[i] <= arr2[j]) {
                    merged[k] = arr[i];
                    i = i + 1;
                } else {
                    merged[k] = arr2[j];
                    j = j + 1;
                }
                k = k + 1;
            }
            while (i < n) {
                merged[k] = arr[i];
                i = i + 1;
                k = k + 1;
            }
            while (j < n2) {
                merged[k] = arr2[j];
                j = j + 1;
                k = k + 1;
            }
            printf("Merged: ");
            printArray(merged, n + n2);
        } else if (choice == 7) {
            int total = 0;
            int given = 0;
            printf("Array is %d numbers from 1..%d with one missing\n",
                   n, n + 1);
            for (int i = 0; i < n; i++) {
                given = given + arr[i];
            }
            int m = n + 1;
            total = m * (m + 1) / 2;
            printf("Missing number (sum method): %d\n", total - given);
            int x = 0;
            for (int i = 1; i <= m; i++) {
                x = x ^ i;
            }
            for (int i = 0; i < n; i++) {
                x = x ^ arr[i];
            }
            printf("Missing number (XOR method): %d\n", x);
        } else if (choice == 8) {
            int candidate = arr[0];
            int count = 1;
            for (int i = 1; i < n; i++) {
                if (arr[i] == candidate) {
                    count = count + 1;
                } else {
                    count = count - 1;
                    if (count == 0) {
                        candidate = arr[i];
                        count = 1;
                    }
                }
            }
            int verify = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] == candidate) {
                    verify = verify + 1;
                }
            }
            if (verify > n / 2) {
                printf("Majority element: %d (appears %d/%d times)\n",
                       candidate, verify, n);
            } else {
                printf("No majority element exists\n");
            }
        } else if (choice == 9) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
