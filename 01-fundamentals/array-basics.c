/*
Array Basics - Traversal, Insert, Delete, Search, Max/Min, Reverse
------------------------------------------------------------------
What this program solves:
The essential array operations every data structure builds on:
traverse, insert at position, delete at position, linear search,
find max/min, reverse, rotate.

Real life example:
A row of cinema seats: booking a seat in the middle makes every
assigned seat after it shuffle one place - exactly the shifting
cost an array pays for a middle insert.

DIAGRAM (insert 99 at index 1 in [10, 20, 30, 40]):
  start:   [10 | 20 30 40]     shift right, starting at the END
  step 1:  [10 | _ 20 30 40]   40->box4, 30->box3, 20->box2
  step 2:  [10 99 20 30 40]    write 99 into box1
  delete index 2 -> [10 99 40]: 40->box2, size shrinks by 1

BEGINNER EXPLANATION (the simplest way to think):
An array is a row of numbered boxes. The number on the box is the
index (starting at 0). Inserting in the middle means SHOVING every
box after it one step to the right first - arrays cannot leave
gaps. Deleting means CLOSING the gap by pulling everything after
it one step left.

SUPER SIMPLE EXAMPLE:
Boxes: [10, 20, 30, 40]
Insert 99 at index 1:  make room  [10, _, 20, 30, 40]
                       place 99   [10, 99, 20, 30, 40]
Delete index 2:        pull left  [10, 99, 40]

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Insert at index n (the very end): no shifting needed at all -
  just place and increase size. This is the ONLY O(1) insert.
- Delete from an empty array: nothing to delete - report an error.
- All elements equal: max == min, both are that value (this is
  correct, not a bug).
- Rotating by 0 or by n: the array comes back unchanged.
- Negative or huge index: invalid - bounds must be 0..n-1 (or n
  for "insert at end").

DRY RUN (step by step):
arr = [5, 8, 2], size = 3
delete index 0: pull 8 to box0, pull 2 to box1, size = 2
                arr = [8, 2]
insert 7 at index 1: push 2 to box2, write 7 into box1, size = 3
                arr = [8, 7, 2]
reverse: swap(8,2) -> [2, 7, 8], middle box 7 stays put

TIPS:
- Array insert/delete in the middle is O(n) because of shifting.
  When your program inserts/deletes in the middle constantly, that
  is the signal to switch to a linked list (06-linked-lists).
- Max/min in ONE pass: keep two variables, update both per element
  - do not scan twice.
- Reversing twice returns the original array - handy for testing.
- index = position - 1 when a human says "3rd element".
*/

#include <stdio.h>

#define MAX 100

void printArray(const int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int arr[MAX];
    int size = 0;
    int choice;
    int value;
    int pos;

    printf("Array Basics\n\n");

    printf("How many elements (0-%d)? ", MAX);
    scanf("%d", &size);

    if (size < 0 || size > MAX) {
        printf("Invalid size\n");
        return 1;
    }

    if (size > 0) {
        printf("Enter %d integers separated by space: ", size);
        for (int i = 0; i < size; i++) {
            scanf("%d", &arr[i]);
        }
    }

    while (1) {
        printf("\nArray: ");
        printArray(arr, size);
        printf("1. Insert at position\n");
        printf("2. Delete at position\n");
        printf("3. Linear search\n");
        printf("4. Max and Min (single pass)\n");
        printf("5. Reverse\n");
        printf("6. Rotate right by k\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Value and position (0-%d, %d = end): ", size, size);
            scanf("%d %d", &value, &pos);
            if (pos < 0 || pos > size || size >= MAX) {
                printf("Invalid position or array full\n");
            } else {
                /* STEP: shift everything right from the end backwards */
                for (int i = size; i > pos; i--) {
                    arr[i] = arr[i - 1];
                }
                arr[pos] = value;
                size = size + 1;
                printf("Inserted %d at index %d\n", value, pos);
            }
        } else if (choice == 2) {
            if (size == 0) {
                printf("Array is empty - nothing to delete\n");
            } else {
                printf("Index to delete (0-%d): ", size - 1);
                scanf("%d", &pos);
                if (pos < 0 || pos >= size) {
                    printf("Invalid index\n");
                } else {
                    /* STEP: pull everything left over the deleted slot */
                    for (int i = pos; i < size - 1; i++) {
                        arr[i] = arr[i + 1];
                    }
                    size = size - 1;
                    printf("Deleted index %d\n", pos);
                }
            }
        } else if (choice == 3) {
            printf("Value to search: ");
            scanf("%d", &value);
            int found = -1;
            for (int i = 0; i < size; i++) {
                if (arr[i] == value) {
                    found = i;
                    break;
                }
            }
            if (found != -1) {
                printf("Found at index %d\n", found);
            } else {
                printf("Not found\n");
            }
        } else if (choice == 4) {
            if (size == 0) {
                printf("Array is empty\n");
            } else {
                /* STEP: one pass tracks both extremes */
                int mn = arr[0];
                int mx = arr[0];
                for (int i = 1; i < size; i++) {
                    if (arr[i] < mn) {
                        mn = arr[i];
                    }
                    if (arr[i] > mx) {
                        mx = arr[i];
                    }
                }
                printf("Min = %d, Max = %d\n", mn, mx);
            }
        } else if (choice == 5) {
            /* STEP: swap ends moving inward */
            for (int i = 0; i < size / 2; i++) {
                int t = arr[i];
                arr[i] = arr[size - 1 - i];
                arr[size - 1 - i] = t;
            }
            printf("Reversed\n");
        } else if (choice == 6) {
            printf("Rotate right by k: ");
            scanf("%d", &value);
            if (size > 0) {
                int k = value % size;
                if (k < 0) {
                    k = k + size;
                }
                /* STEP: rotate = reverse parts, then reverse whole */
                for (int i = 0; i < size / 2; i++) {
                    int t = arr[i];
                    arr[i] = arr[size - 1 - i];
                    arr[size - 1 - i] = t;
                }
                for (int i = 0; i < k / 2; i++) {
                    int t = arr[i];
                    arr[i] = arr[k - 1 - i];
                    arr[k - 1 - i] = t;
                }
                for (int i = k; i < (size + k) / 2; i++) {
                    int t = arr[i];
                    arr[i] = arr[size - 1 - (i - k)];
                    arr[size - 1 - (i - k)] = t;
                }
                printf("Rotated right by %d\n", k);
            }
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
