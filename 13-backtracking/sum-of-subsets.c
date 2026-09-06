/*
Sum of Subsets
--------------
What this program solves:
This program finds all subsets of a given set whose sum is equal to a target value.

Real life example:
Imagine a student has coins or notes of different values and wants to know which
combinations exactly make a required amount, such as 15 rupees. This problem checks
all meaningful combinations using backtracking.

DIAGRAM (DFS on remaining sum, items sorted):
  target 10, items [1 2 5 7 8]
  take 7 -> need 3 -> take 1+2 -> {7,1,2} HIT
  prune: if even ALL remaining items < need, stop that branch.

How the process works:
1. Read the set elements and the target sum.
2. Start from the first element.
3. For each element, there are two choices:
   include it in the subset or exclude it.
4. If the current sum becomes equal to the target, print that subset.
5. If the current sum exceeds the target, stop exploring that path.

Pseudo code:
START
Read set elements and target sum
Start recursive search from first element
For each element
    Include the element and recurse
    Exclude the element and recurse
If current sum = target
    Print the subset
If current sum > target or all elements used
    Stop that path
END

Dry run idea:
If set = {5, 10, 12, 13, 15, 18} and target = 30,
possible answers include {5, 10, 15} and {12, 18}.
*/
#include <stdio.h>

#define MAX 20

int set[MAX], subset[MAX];
int n, target;

void sumOfSubsets(int index, int currentSum, int subsetSize) {
    int i;

    if (currentSum == target) {
        printf("{ ");
        for (i = 0; i < subsetSize; i++) {
            printf("%d ", subset[i]);
        }
        printf("}\n");
        return;
    }

    if (index == n || currentSum > target) {
        return;
    }

    subset[subsetSize] = set[index];
    sumOfSubsets(index + 1, currentSum + set[index], subsetSize + 1);
    sumOfSubsets(index + 1, currentSum, subsetSize);
}

int main() {
    int i;

    printf("Sum of Subsets Problem\n");
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &set[i]);
    }

    printf("Enter the target sum: ");
    scanf("%d", &target);

    printf("Subsets with sum %d are:\n", target);
    sumOfSubsets(0, 0, 0);

    return 0;
}

