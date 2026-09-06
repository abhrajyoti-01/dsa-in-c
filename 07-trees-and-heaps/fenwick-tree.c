/*
Fenwick Tree (Binary Indexed Tree)
----------------------------------
What this program solves:
Supports point updates and PREFIX SUM queries in O(log n) each -
faster than recomputing sums O(n) and simpler than a segment tree.

Real life example:
A running leaderboard: player scores update constantly, and the
organizer asks "total score of the top 50 players?" repeatedly.
BIT maintains cumulative totals with logarithmic work per change.

DIAGRAM (BIT: each index stores a block ending at it):
  index:  1   2   3   4   5   6   7   8
  tree:   a1  a1+ a3  a1+ a5  a5+ a7  a1..a8
              a2      a4          a6
  update i: i += i & (-i)   (climb right/up)
  prefix(i): i -= i & (-i)  (walk left/down, summing)
  O(log n) for both, using one plain array.

How the process works:
1. tree[i] stores the sum of a block of LOWBIT(i) elements ending
   at i, where LOWBIT(i) = i & (-i) (the lowest set bit).
2. PREFIX SUM (1..i): walk i -> i - LOWBIT(i) -> ... -> 0,
   adding tree values.
3. POINT UPDATE (+delta at i): walk i -> i + LOWBIT(i) -> ... -> n,
   adding delta to each covering block.

Pseudo code:
START
UPDATE(i, delta): WHILE i <= n: tree[i] += delta; i += i & (-i)
QUERY(i): sum = 0
    WHILE i > 0: sum += tree[i]; i -= i & (-i)
    RETURN sum
END

Sample input and dry run:
arr = [3, 1, 4, 1, 5] (1-indexed)
query(3) = 3+1+4 = 8
update(2, +10) -> arr[2] = 11
query(4) = 3+11+4+1 = 19

Main logic to understand:
The magic is i & (-i): it isolates the lowest set bit, defining the
block each node covers. Both operations take O(log n) - there are
at most log n set bits to walk. 1-indexed by design.
*/

#include <stdio.h>

#define MAX_ELEMENTS 1000

int tree[MAX_ELEMENTS + 1];
int n;

/*
 * lowbit - Isolates the lowest set bit of i.
 */
int lowbit(int i) {
    return i & (-i);
}

/*
 * bitUpdate - Adds delta to position i (1-indexed).
 */
void bitUpdate(int i, int delta) {
    while (i <= n) {
        tree[i] = tree[i] + delta;
        i = i + lowbit(i);
    }
}

/*
 * bitQuery - Returns prefix sum of positions 1..i.
 */
int bitQuery(int i) {
    int sum = 0;
    while (i > 0) {
        sum = sum + tree[i];
        i = i - lowbit(i);
    }
    return sum;
}

/*
 * rangeSum - Sum over [l, r] = prefix(r) - prefix(l-1).
 */
int rangeSum(int l, int r) {
    return bitQuery(r) - bitQuery(l - 1);
}

/*
 * bitBuild - Constructs the tree from the initial array.
 */
void bitBuild(const int arr[], int size) {
    for (int i = 1; i <= size; i++) {
        bitUpdate(i, arr[i]);
    }
}

int main() {
    int arr[MAX_ELEMENTS + 1];
    int choice;
    int i;
    int value;
    int l;
    int r;

    printf("Fenwick Tree (Binary Indexed Tree)\n\n");

    printf("How many elements (1-%d)? ", MAX_ELEMENTS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ELEMENTS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space (1-indexed): ", n);
    for (int k = 1; k <= n; k++) {
        scanf("%d", &arr[k]);
    }

    for (int k = 0; k <= n; k++) {
        tree[k] = 0;
    }
    bitBuild(arr, n);
    printf("Fenwick tree built\n");

    while (1) {
        printf("\n1. Prefix sum (1..i)\n");
        printf("2. Range sum [l, r]\n");
        printf("3. Point update arr[i] += value\n");
        printf("4. Point update arr[i] = value\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Enter i (1..%d): ", n);
            scanf("%d", &i);
            if (i >= 1 && i <= n) {
                printf("Prefix sum(1..%d) = %d\n", i, bitQuery(i));
            } else {
                printf("Invalid index\n");
            }
        } else if (choice == 2) {
            printf("Enter l and r (1..%d): ", n);
            scanf("%d %d", &l, &r);
            if (l >= 1 && r <= n && l <= r) {
                printf("Sum[%d..%d] = %d\n", l, r, rangeSum(l, r));
            } else {
                printf("Invalid range\n");
            }
        } else if (choice == 3) {
            printf("Enter index and delta: ");
            scanf("%d %d", &i, &value);
            if (i >= 1 && i <= n) {
                bitUpdate(i, value);
                arr[i] = arr[i] + value;
                printf("arr[%d] is now %d\n", i, arr[i]);
            } else {
                printf("Invalid index\n");
            }
        } else if (choice == 4) {
            printf("Enter index and new value: ");
            scanf("%d %d", &i, &value);
            if (i >= 1 && i <= n) {
                bitUpdate(i, value - arr[i]);
                arr[i] = value;
                printf("arr[%d] is now %d\n", i, arr[i]);
            } else {
                printf("Invalid index\n");
            }
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
