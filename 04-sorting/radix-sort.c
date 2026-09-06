/*
 PROBLEM DEFINITION:
 --------------------
 Radix Sort is a NON-COMPARISON sorting algorithm. Instead of comparing
 elements, it sorts digit by digit:
   - Sort all numbers by their LEAST significant digit (LSD),
     then by the next digit, ... up to the most significant digit.
   - Each pass must be STABLE (preserve the order of equal digits) -
     stability is what makes earlier passes' work survive later passes.
 Counting Sort (stable, O(n + k)) is used as the per-digit subroutine.

 REAL-LIFE EXAMPLE - Sorting Library Cards / Punch Cards:
 ---------------------------------------------------------
 Old library card sorters physically sorted punch cards digit by digit:
 first pass separated cards by units digit into 10 bins (0-9), collected
 them in bin order; second pass separated by tens digit; and so on.
 After the final pass the whole deck was perfectly ordered. Postal
 services sorting mail by postal-code digits and banks processing
 checks by account digits used the same multi-pass bin strategy.
 Modern GPU sorting libraries (e.g., CUB radix sort) still use it
 because it beats comparison sorts for integers.

 PROCESS (Step-by-Step Logic):
 ------------------------------
 Think of sorting exam papers by roll number, one digit column at a time:

 1. FIND THE MAXIMUM:
    - The number of digits in the max value decides HOW MANY passes.

 2. REPEAT FOR EACH DIGIT (units, tens, hundreds...):
    - Run a STABLE Counting Sort keyed on the current digit:
        a) Count how many numbers have each digit 0..9.
        b) Convert counts to prefix sums -> each digit's ending slot.
        c) Place numbers into output by scanning input RIGHT to LEFT
           (right-to-left scan + prefix sums = stability).
        d) Copy output back to the array.

 3. STABILITY IS CRITICAL:
    - Numbers sharing the current digit keep the order built by earlier
      (less significant) digits. Lose stability and earlier passes unravel.

 WHY LSD (least significant) FIRST WORKS:
    - After the last pass, the most significant digit dominates ordering;
      ties on it are already correctly ordered by every earlier pass.

 PSEUDO CODE:
 ------------
 START
   Function RadixSort(arr[], n):
     maxVal = maximum(arr)
     For exp = 1; maxVal / exp > 0; exp = exp * 10:
         CountingSortByDigit(arr, n, exp)

   Function CountingSortByDigit(arr[], n, exp):
     count[0..9] = 0
     For i = 0 to n-1:
         count[(arr[i] / exp) mod 10]++
     For d = 1 to 9:
         count[d] = count[d] + count[d-1]
     For i = n-1 down to 0:                  (right-to-left = stable)
         digit = (arr[i] / exp) mod 10
         output[count[digit] - 1] = arr[i]
         count[digit]--
     Copy output back into arr
 END

 SAMPLE INPUT & DRY RUN:
 ------------------------
 Array: [170, 45, 75, 90, 802, 24, 2, 66]
 Max = 802 -> 3 digits -> 3 passes.

 PASS 1 (units digit):
   170->0, 45->5, 75->5, 90->0, 802->2, 24->4, 2->2, 66->6
   Buckets: 0:{170,90} 2:{802,2} 4:{24} 5:{45,75} 6:{66}
   Output:  [170, 90, 802, 2, 24, 45, 75, 66]

 PASS 2 (tens digit):
   170->7, 90->9, 802->0, 2->0, 24->2, 45->4, 75->7, 66->6
   Buckets: 0:{802,2} 2:{24} 4:{45} 6:{66} 7:{170,75} 9:{90}
   Output:  [802, 2, 24, 45, 66, 170, 75, 90]

 PASS 3 (hundreds digit):
   802->8, others ->0
   Buckets: 0:{2,24,45,66,170,75,90} 8:{802}
   Output:  [2, 24, 45, 66, 75, 90, 170, 802]   SORTED!

 Note how 170 stayed before 75 in pass 2 (both tens digit 7) because
 pass 1 had already ordered them by units - stability preserved it.

 TIME COMPLEXITY:
 - O(d x (n + k)) where d = digit count, k = base (10)
 - For fixed-width integers: effectively O(n) - LINEAR time!

 SPACE COMPLEXITY: O(n + k) for output and count arrays

 COMMON USE CASES:
 - Sorting large collections of integers (GPU sorting, databases)
 - Suffix array construction (DC3 algorithm)
 - Sorting fixed-length strings/keys (dates, IDs, ZIP codes)
 - When k (key range) is small and stability matters
DIAGRAM (LSD radix, [170 45 75 90 2 802] - sort by each digit):
  by 1s: 170 90 2 802 45 75
  by 10s: 2 802 45 170 75 90
  by 100s: 2 45 75 90 170 802   <- sorted!
  stability of each pass is what makes earlier passes survive.

*/

#include <stdio.h>
#include <stdlib.h>

/*
 * findMax - Returns the maximum element (decides the number of passes).
 */
int findMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

/*
 * countingSortByDigit - Stable counting sort on the digit at place exp.
 * exp = 1 -> units, 10 -> tens, 100 -> hundreds...
 *
 * Analogy: Dropping library cards into 10 bins by one digit column,
 * then collecting bins 0 to 9 back in order.
 */
void countingSortByDigit(int arr[], int n, int exp, int printPass) {
    int output[100];
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    for (int d = 1; d < 10; d++) {
        count[d] += count[d - 1];
    }

    if (printPass) {
        printf("  Pass (place value %3d):\n", exp);
        printf("    Digit counts : ");
        for (int d = 0; d < 10; d++) {
            printf("%d ", count[d] - ((d > 0) ? count[d - 1] : 0));
        }
        printf(" (for digits 0..9)\n");
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    if (printPass) {
        printf("    After pass   : [");
        for (int i = 0; i < n; i++) {
            printf("%d", arr[i]);
            if (i < n - 1) printf(", ");
        }
        printf("]\n\n");
    }
}

/*
 * radixSort - Orchestrates one stable pass per digit.
 */
void radixSort(int arr[], int n, int printPasses) {
    int max = findMax(arr, n);

    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSortByDigit(arr, n, exp, printPasses);
    }
}

/*
 * printArray - Utility to display the array contents.
 */
void printArray(int arr[], int n) {
    printf("  [");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int arr[100];
    int n;
    int trace;

    printf("Radix Sort - Non-Comparison Linear Sorting\n\n");

    printf("How many elements (1-100)? ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d non-negative integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Show pass-by-pass trace? (1 = yes, 0 = no): ");
    scanf("%d", &trace);

    printf("\nOriginal: ");
    printArray(arr, n);

    int max = findMax(arr, n);
    int digits = 1;
    int tempMax = max;
    while (tempMax >= 10) {
        tempMax = tempMax / 10;
        digits = digits + 1;
    }
    printf("Max = %d -> %d digits -> %d passes\n", max, digits, digits);

    /* STEP: sort with optional pass-by-pass trace */
    radixSort(arr, n, trace);

    printf("\nSorted:   ");
    printArray(arr, n);

    return 0;
}
