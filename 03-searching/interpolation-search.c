/*
Interpolation Search
--------------------
What this program solves:
Searches a SORTED, UNIFORMLY DISTRIBUTED array by PROBING where the
target should mathematically be - like binary search but the probe
position is guessed by linear interpolation instead of the middle.

Real life example:
Opening a phone book for "Patel": you do not open the middle - you
estimate P is ~60% through the alphabet and open there directly.
The better the estimate (uniform distribution), the fewer steps.

DIAGRAM (probe where the value SHOULD be, [10,20,30,...,100], target 70):
  pos = 0 + (70-10)*(9-0)/(100-10) = 6
  arr[6] = 70 -> found in ONE probe (binary search needed ~4)
  works great on uniform data, degrades on skewed data.

How the process works:
1. Probe position:
   pos = left + (target - arr[left]) * (right - left)
              / (arr[right] - arr[left]).
2. If arr[pos] == target -> found.
3. If target > arr[pos] -> search right part, else left part.
4. Stops when target is outside [arr[left], arr[right]].

Pseudo code:
START
WHILE left <= right AND arr[left] <= target <= arr[right]
    pos = left + (target-arr[left])*(right-left)/(arr[right]-arr[left])
    IF arr[pos] == target: RETURN pos
    IF arr[pos] < target: left = pos + 1
    ELSE right = pos - 1
RETURN -1
END

Sample input and dry run:
arr = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100], target = 70
pos = 0 + (70-10)*9/(100-10) = 6 -> arr[6] = 70 found in ONE probe

Main logic to understand:
O(log log n) average on uniform data (the best of all searches),
but O(n) worst case on skewed distributions (e.g. exponential
values). Requires sorted data; O(1) space.
*/

#include <stdio.h>

int main() {
    int arr[200];
    int n;
    int target;

    printf("Interpolation Search\n\n");

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

    int left = 0;
    int right = n - 1;
    int probes = 0;
    int found = -1;

    /* STEP: probe by linear interpolation until found or range empty */
    while (left <= right && target >= arr[left] && target <= arr[right]) {
        if (arr[right] == arr[left]) {
            /* range of equal values - avoid division by zero */
            if (arr[left] == target) {
                found = left;
            }
            break;
        }

        int pos = left + (int)((long long)(target - arr[left]) * (right - left)
                               / (arr[right] - arr[left]));
        if (pos < left) {
            pos = left;
        }
        if (pos > right) {
            pos = right;
        }

        probes = probes + 1;
        printf("Probe %d: position %d holds %d\n", probes, pos, arr[pos]);

        if (arr[pos] == target) {
            found = pos;
            break;
        }
        if (arr[pos] < target) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }

    if (found != -1) {
        printf("Found %d at index %d (probes: %d)\n", target, found, probes);
    } else {
        printf("%d not found (probes: %d)\n", target, probes);
    }

    return 0;
}
