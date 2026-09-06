/*
Ternary Search
--------------
What this program solves:
Searches a SORTED (unimodal-friendly) array by splitting it into
THREE parts with two midpoints and recursing into only one part -
like binary search but with two probes per step.

Real life example:
Finding where a ramp peaks by testing two equally spaced points:
whichever side is higher contains the peak. On ordered data the
same idea discards two-thirds of the range each step.

DIAGRAM (two midpoints split into three parts):
  [0 ......... m1 ... m2 ......... n]
  target < arr[m1]  -> left third
  target > arr[m2]  -> right third
  else              -> middle third
  2 probes discard 2/3 of the range each step (vs 1/2 for binary);
  log base 1.5 n steps but 2 comparisons each.

How the process works:
1. third = (right - left) / 3.
2. mid1 = left + third, mid2 = right - third.
3. If target == arr[mid1] or arr[mid2] -> found.
4. If target < arr[mid1] -> search [left, mid1-1].
   If target > arr[mid2] -> search [mid2+1, right].
   Else -> search (mid1, mid2).
5. Repeat until left > right.

Pseudo code:
START
FUNCTION ternary(left, right)
    IF left > right: RETURN -1
    third = (right - left) / 3
    mid1 = left + third; mid2 = right - third
    IF arr[mid1] == target: RETURN mid1
    IF arr[mid2] == target: RETURN mid2
    IF target < arr[mid1]:  RETURN ternary(left, mid1-1)
    IF target > arr[mid2]:  RETURN ternary(mid2+1, right)
    RETURN ternary(mid1+1, mid2-1)
END

Sample input and dry run:
arr = [1, 3, 5, 7, 9, 11, 13, 15, 17], target = 11
mid1 = 3 (5), mid2 = 5 (11) -> found at index 5 immediately

Main logic to understand:
Each level does 2 comparisons but discards 2/3 of the range ->
O(log3 n) depth, so 2*log3(n) ~ 1.26*log2(n) comparisons: binary
search is still slightly better, but ternary generalizes to
unimodal optimization where two probes are mandatory.
*/

#include <stdio.h>

/*
 * ternarySearch - Recursive three-way split on a sorted array.
 */
int ternarySearch(const int arr[], int left, int right, int target, int* probes) {
    if (left > right) {
        return -1;
    }

    int third = (right - left) / 3;
    int mid1 = left + third;
    int mid2 = right - third;

    *probes = *probes + 2;

    if (arr[mid1] == target) {
        return mid1;
    }
    if (arr[mid2] == target) {
        return mid2;
    }

    if (target < arr[mid1]) {
        return ternarySearch(arr, left, mid1 - 1, target, probes);
    }
    if (target > arr[mid2]) {
        return ternarySearch(arr, mid2 + 1, right, target, probes);
    }
    return ternarySearch(arr, mid1 + 1, mid2 - 1, target, probes);
}

int main() {
    int arr[200];
    int n;
    int target;

    printf("Ternary Search\n\n");

    printf("How many elements (1-200)? ");
    scanf("%d", &n);

    if (n < 1 || n > 200) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers in ASCENDING order separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter value to search: ");
    scanf("%d", &target);

    int probes = 0;
    int pos = ternarySearch(arr, 0, n - 1, target, &probes);

    if (pos != -1) {
        printf("Found %d at index %d (2-way probes used: %d)\n",
               target, pos, probes);
    } else {
        printf("%d not found (probes used: %d)\n", target, probes);
    }

    return 0;
}
