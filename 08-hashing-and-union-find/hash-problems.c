/*
Hashing Problems Ladder (Basic -> Hard)
---------------------------------------
Real life example:
"Different items in the last k clicks" (window distinct) and
two-sum are the bread and butter of recommendation engines and
payment fraud checks.

DIAGRAM (two-sum with a hash - one pass):
  [2, 7, 11], target 9
  x=2: need 9-2=7, seen={} -> no;  store 2
  x=7: need 9-7=2, 2 IS in seen -> answer (2, 7)
  O(n) time, no nested loop, no sorting.

LADDER (easy -> hard inside this file):
  1. Count frequency of each element   (count array / hash)
  2. First non-repeating element       (two passes)
  3. Two-sum                           (hash for the complement)
  4. Distinct elements in window of size k
  5. Longest consecutive sequence      (hash-set style, O(n))

BEGINNER EXPLANATION:
A hash table turns "have I seen X?" into O(1). Every problem here
is some combination of:
- count[v]++ while scanning (frequency),
- lookup BEFORE you insert (two-sum: is target - x already seen?),
- a sliding window with a count map (distinct in window),
- jump-testing v+1, v+2... for longest runs.

SUPER SIMPLE EXAMPLE (two-sum, [2, 7, 11], target 9):
x=2: seen={} -> 9-2=7 not seen. seen={2}
x=7: 9-7=2 IS in seen -> answer: 2 + 7 = 9. One pass, O(n),
no sorting, no nested loop.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- First non-repeating when ALL repeat: report "none" - the second
  pass finds nothing; that is an answer, not a crash.
- Two-sum with duplicates: [3, 3], target 6: x=3 not seen yet ->
  seen={3}; second 3: 6-3=3 IS seen -> works because we check
  BEFORE inserting. Checking after inserting misses it.
- Negative numbers: count-array indexing needs an offset or a
  hash table - negatives are why "just use an array" fails.
- Longest consecutive with duplicates: duplicates add nothing -
  skip values already counted.
- Empty window / k > n: invalid - reject.

DRY RUN (longest consecutive, [1, 9, 3, 10, 4, 20, 2]):
Put all in a set: {1,2,3,4,9,10,20}
1: is 1 a chain START? (0 not in set) yes. Extend: 2,3,4 in set,
   5 not -> run length 4.
9: start (8 absent): 10 in, 11 not -> length 2.
20: start: length 1. Best = 4 (the run 1-4). O(n) total because
   each value is visited at most twice (once as start check, once
   in a chain).

TIPS:
- The "check before insert" pattern solves two-sum, three-sum
  variants, and duplicate detection - internalize it.
- Longest consecutive sequence WITHOUT sorting is a famous
  O(n) solution interviewers love - the "chain start" test is
  the key insight (only start counting from true beginnings).
- These all map onto hash_table.c / hash_linear_probing.c from
  this folder - swap the count array for a real hash table as
  the extension exercise.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 200000
#define OFFSET 100000

int count[MAX_V + 1];

int main() {
    int arr[200];
    int n;
    int choice;

    printf("Hashing Problems Ladder (easy -> hard)\n\n");

    printf("How many elements (1-200)? ");
    scanf("%d", &n);

    if (n < 1 || n > 200) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers (-100000..100000): ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    while (1) {
        printf("\n1. Frequency count\n");
        printf("2. First non-repeating element\n");
        printf("3. Two-sum (find pair with given target)\n");
        printf("4. Distinct elements in every window of size k\n");
        printf("5. Longest consecutive sequence\n");
        printf("6. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            memset(count, 0, sizeof(count));
            for (int i = 0; i < n; i++) {
                count[arr[i] + OFFSET] = count[arr[i] + OFFSET] + 1;
            }
            printf("Frequencies:\n");
            for (int i = 0; i < n; i++) {
                int idx = arr[i] + OFFSET;
                if (count[idx] > 0) {
                    printf("  %d -> %d\n", arr[i], count[idx]);
                    count[idx] = 0;
                }
            }
        } else if (choice == 2) {
            memset(count, 0, sizeof(count));
            for (int i = 0; i < n; i++) {
                count[arr[i] + OFFSET] = count[arr[i] + OFFSET] + 1;
            }
            int found = -1;
            for (int i = 0; i < n; i++) {
                if (count[arr[i] + OFFSET] == 1) {
                    found = i;
                    break;
                }
            }
            if (found == -1) {
                printf("No non-repeating element exists\n");
            } else {
                printf("First non-repeating: %d (index %d)\n",
                       arr[found], found);
            }
        } else if (choice == 3) {
            int target;
            printf("Target sum: ");
            scanf("%d", &target);
            memset(count, 0, sizeof(count));
            int foundA = 0;
            for (int i = 0; i < n; i++) {
                int complement = target - arr[i];
                if (complement + OFFSET >= 0 && complement + OFFSET <= MAX_V &&
                    count[complement + OFFSET] > 0) {
                    printf("Pair found: %d + %d = %d\n",
                           complement, arr[i], target);
                    foundA = 1;
                    break;
                }
                count[arr[i] + OFFSET] = count[arr[i] + OFFSET] + 1;
            }
            if (!foundA) {
                printf("No pair sums to %d\n", target);
            }
        } else if (choice == 4) {
            int k;
            printf("Window size k (1-%d)? ", n);
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("k out of range\n");
                continue;
            }
            memset(count, 0, sizeof(count));
            int distinct = 0;
            for (int i = 0; i < n; i++) {
                int idx = arr[i] + OFFSET;
                if (count[idx] == 0) {
                    distinct = distinct + 1;
                }
                count[idx] = count[idx] + 1;

                if (i >= k) {
                    int out = arr[i - k] + OFFSET;
                    count[out] = count[out] - 1;
                    if (count[out] == 0) {
                        distinct = distinct - 1;
                    }
                }
                if (i >= k - 1) {
                    printf("Window [%d..%d]: %d distinct\n",
                           i - k + 1, i, distinct);
                }
            }
        } else if (choice == 5) {
            memset(count, 0, sizeof(count));
            for (int i = 0; i < n; i++) {
                count[arr[i] + OFFSET] = 1;
            }
            int best = 0;
            for (int i = 0; i < n; i++) {
                int v = arr[i];
                if (count[v - 1 + OFFSET] == 0) {
                    int length = 1;
                    while (count[v + length + OFFSET] == 1) {
                        length = length + 1;
                    }
                    if (length > best) {
                        best = length;
                    }
                }
            }
            printf("Longest consecutive run: %d\n", best);
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
