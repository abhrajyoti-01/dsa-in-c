/*
Sorting Structures & Quickselect (Basic -> Hard)
------------------------------------------------
Real life example:
Leaderboards: sort players by score (secondary key: name) and
"top 10 only" is quickselect - no full sort needed.

DIAGRAM (quickselect k=2 in [7,2,9,4]):
  partition around 7: [2 4] 7 [9] -> 7 is 3rd smallest
  k=2 < 3 -> recurse LEFT only: [2 4] -> answer 4
  one side per step: n + n/2 + n/4 ... = O(n) average.

LADDER (easy -> hard inside this file):
  1. Sort an array of STRUCTS by one key    (qsort comparator)
  2. Multi-key sort                         (marks desc, then roll asc)
  3. Stability demo                         (stable vs unstable)
  4. Quickselect: kth smallest in O(n) avg  (partition without full sort)

BEGINNER EXPLANATION:
Real data has multiple fields - you sort STRUCTS, and secondary
keys break ties. Stability means equal keys keep their original
relative order (crucial when sorting by column B after column A).
Quickselect = quicksort's partition used to answer ONLY "what is
the k-th smallest?" - recursing into ONE side instead of both,
giving O(n) average instead of O(n log n).

SUPER SIMPLE EXAMPLE (multi-key):
Students: (88, roll 3), (88, roll 1), (75, roll 2)
Sort by marks desc, tie -> roll asc:
(88, 1), (88, 3), (75, 2)
The comparator returns y->marks - x->marks; if 0, x->roll - y->roll.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Unstable sort + multi-pass: sort by name then by marks with an
  UNSTABLE sort -> name order among equal marks is LOST. Stable
  sorts (merge) preserve it. This is THE reason stability matters.
- Quickselect worst case O(n^2): adversarial input (all equal,
  bad pivots) degrades it - median-of-medians fixes to O(n)
  worst-case but is rarely worth the constant.
- k out of range (k < 1 or k > n): report error, don't index.
- All elements equal: every k returns the same value instantly -
  partitions become lopsided (worst-case trigger!).

DRY RUN (quickselect, [7, 2, 9, 4], k = 2nd smallest):
partition around pivot 4 (last): [2, 4, 9, 7] -> pivot at index 1
k = 2 means index 1 -> pivot IS the answer: 4.
Only one partition + zero recursion - O(n) this time.

TIPS:
- qsort is the standard library's quicksort-family function: your
  comparator is the only thing you write - see function_pointers.c
  in folder 20 for the callback mechanics.
- Quickselect returns to find medians (folder 10) and kth
  statistics; combine with quicksort understanding for full marks.
- Stability check trick: sort pairs (value, originalIndex) and
  see whether originalIndex stays ordered among equal values.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 50

typedef struct {
    char name[20];
    int marks;
    int roll;
} Student;

/*
 * compareMarksDescRollAsc - Multi-key comparator.
 */
int compareMarksDescRollAsc(const void* a, const void* b) {
    const Student* x = (const Student*)a;
    const Student* y = (const Student*)b;
    if (x->marks != y->marks) {
        return y->marks - x->marks;
    }
    return x->roll - y->roll;
}

/*
 * swapInt - Utility.
 */
void swapInt(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/*
 * partition - Lomuto scheme; returns pivot's final index.
 */
int partition(int arr[], int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swapInt(&arr[i], &arr[j]);
        }
    }
    swapInt(&arr[i + 1], &arr[hi]);
    return i + 1;
}

/*
 * quickselect - Iterative k-th smallest (0-based k).
 */
int quickselect(int arr[], int n, int k) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int p = partition(arr, lo, hi);
        if (p == k) {
            return arr[p];
        }
        if (p < k) {
            lo = p + 1;
        } else {
            hi = p - 1;
        }
    }
    return -1;
}

int main() {
    int choice;

    printf("Sorting Structures & Quickselect (easy -> hard)\n\n");

    /* PART 1-3: struct sorting and stability */
    Student students[MAX_ITEMS];
    int n;

    printf("How many students (1-%d)? ", MAX_ITEMS);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ITEMS) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter name marks roll for each:\n");
    for (int i = 0; i < n; i++) {
        printf("  Student %d: ", i + 1);
        scanf("%19s %d %d", students[i].name, &students[i].marks,
              &students[i].roll);
    }

    while (1) {
        printf("\n1. Sort by marks desc (ties -> roll asc)\n");
        printf("2. Sort by roll asc (show original order effect)\n");
        printf("3. Quickselect: k-th smallest marks (re-uses marks)\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            qsort(students, (size_t)n, sizeof(Student),
                  compareMarksDescRollAsc);
            printf("Sorted (marks desc, roll asc):\n");
            for (int i = 0; i < n; i++) {
                printf("  %-10s marks %3d roll %d\n",
                       students[i].name, students[i].marks,
                       students[i].roll);
            }
        } else if (choice == 2) {
            /* insertion sort by roll - stable, preserves ties */
            for (int i = 1; i < n; i++) {
                Student key = students[i];
                int j = i - 1;
                while (j >= 0 && students[j].roll > key.roll) {
                    students[j + 1] = students[j];
                    j = j - 1;
                }
                students[j + 1] = key;
            }
            printf("Sorted by roll (insertion - STABLE):\n");
            for (int i = 0; i < n; i++) {
                printf("  %-10s marks %3d roll %d\n",
                       students[i].name, students[i].marks,
                       students[i].roll);
            }
        } else if (choice == 3) {
            int k;
            printf("k (1-%d, 1 = smallest): ", n);
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("k out of range\n");
            } else {
                int marks[MAX_ITEMS];
                for (int i = 0; i < n; i++) {
                    marks[i] = students[i].marks;
                }
                int answer = quickselect(marks, n, k - 1);
                printf("%d-th smallest marks = %d (array partially "
                       "rearranged - that is quickselect!)\n", k, answer);
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
