/*
Binary Search
-------------
What this program solves:
This program finds an element in a sorted list by repeatedly cutting the search
space into two halves. It is much faster than checking every element one by one.

Real life example:
Suppose you are finding a word in a dictionary. You do not start from page 1
and read every word. Instead, you open somewhere in the middle. If the word you
want comes later alphabetically, you search the second half; otherwise you search
the first half. Binary search follows exactly this idea.

DIAGRAM (search 40 in [10 20 30 40 50 60]):
  step 1: [10 20 30 | 40 50 60]
                  ^ mid=30, 40>30 -> go right
  step 2:            [40 50 | 60]
                      ^ mid=50, 40<50 -> go left
  step 3:            [40]
                      ^ found!
Halving: 6 -> 3 -> 1 -> O(log n) steps.

How the process works:
1. Start with the full sorted array.
2. Check the middle element.
3. If it matches, the search is complete.
4. If the key is smaller, search only the left half.
5. If the key is larger, search only the right half.
6. Repeat until the element is found or the range becomes empty.

Pseudo code:
START
Read n, sorted array, and key
Set low = 0 and high = n - 1
While low <= high
    mid = (low + high) / 2
    If array[mid] = key
        Element found
        Stop
    Else if key < array[mid]
        high = mid - 1
    Else
        low = mid + 1
If not found, print not found
END

Sample input and dry run:
Array = 10 20 30 40 50 60, key = 40
low = 0, high = 5, mid = 2, a[mid] = 30
Since 40 > 30, search moves to the right half.
Now low = 3, high = 5, mid = 4, a[mid] = 50
Since 40 < 50, search moves left.
Now low = 3, high = 3, mid = 3, a[mid] = 40, so the element is found.

Important condition:
This method works only when the array is already sorted.
*/
#include <stdio.h>

int main() {
    int a[100], n, i, key;
    int low, high, mid, found = 0;

    printf("Binary Search\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements in sorted order:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    printf("Enter the element to search: ");
    scanf("%d", &key);

    low = 0;
    high = n - 1;

    while (low <= high) {
        mid = (low + high) / 2;

        if (a[mid] == key) {
            found = 1;
            break;
        } else if (key < a[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (found) {
        printf("Element found at position %d\n", mid + 1);
    } else {
        printf("Element not found\n");
    }

    return 0;
}

