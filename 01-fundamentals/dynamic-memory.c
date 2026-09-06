/*
Dynamic Memory - malloc, calloc, realloc, free
----------------------------------------------
What this program solves:
Demonstrates heap allocation: malloc (uninitialized), calloc
(zeroed), resizing with realloc, freeing memory, and checking for
allocation failure - with user input driving sizes and values.

Real life example:
A text editor starts with a small buffer; as you type, it grows
with realloc instead of restarting. Databases, image loaders, and
every linked structure from this course lives on the heap.

DIAGRAM (stack pointer vs heap block):
  STACK (auto, freed on return)     HEAP (lives until free)
  +------------------+              +------------------+
  | main: p          |              | 10 20 30 40      |<-malloc
  |   p = 0x5f00 ----+------------> +------------------+
  +------------------+              realloc copies into a
                                    bigger block; free(p) releases

How the process works:
1. malloc(n * size): reserves UNINITIALIZED bytes.
2. calloc(count, size): reserves AND zeroes - good for counters.
3. realloc(ptr, newSize): grows/shrinks in place if possible,
   otherwise copies content to a new block (old pointer invalid!).
4. free(ptr): returns the block; using ptr afterwards is a bug
   (dangling pointer) - set to NULL.
5. Memory leaks: every allocation must pair with exactly one free.

Pseudo code:
START
a = malloc(n * sizeof(int)); fill from input
b = calloc(n, sizeof(int)); show all zero
b = realloc(b, 2n * sizeof(int)); append more input
free(a); free(b)
END

Sample input and dry run:
n = 3, values 5 10 15 -> malloc block shows 5 10 15
calloc shows 0 0 0
grow to 5, add 20 30 -> block shows 5 10 15 20 30
all freed cleanly

Main logic to understand:
The heap gives runtime-sized memory that stack arrays cannot.
Always check for NULL (allocation can fail), never touch freed
memory, and match every malloc/calloc/realloc with one free.
realloc may move the block - always use its return value.
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    int newSize;

    printf("Dynamic Memory Demo (malloc / calloc / realloc / free)\n\n");

    /* STEP: malloc - uninitialized block of n ints */
    printf("How many integers to store with malloc (1-100)? ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid size\n");
        return 1;
    }

    int* a = (int*)malloc((size_t)n * sizeof(int));
    if (a == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    printf("malloc block: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    /* STEP: calloc - zero-initialized block */
    int* b = (int*)calloc((size_t)n, sizeof(int));
    if (b == NULL) {
        printf("calloc failed\n");
        free(a);
        return 1;
    }

    printf("calloc block (all zeros): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", b[i]);
    }
    printf("\n");

    /* STEP: realloc - grow the block */
    printf("\nNew size to grow the calloc block to (>= %d, max 200): ", n);
    scanf("%d", &newSize);

    if (newSize < n || newSize > 1000) {
        printf("Invalid size\n");
        free(a);
        free(b);
        return 1;
    }

    int* bigger = (int*)realloc(b, (size_t)newSize * sizeof(int));
    if (bigger == NULL) {
        printf("realloc failed\n");
        free(a);
        free(b);
        return 1;
    }
    b = bigger;

    printf("Enter %d more integers for the new slots: ", newSize - n);
    for (int i = n; i < newSize; i++) {
        scanf("%d", &b[i]);
    }

    printf("After realloc, block holds: ");
    for (int i = 0; i < newSize; i++) {
        printf("%d ", b[i]);
    }
    printf("\n");

    /* STEP: free everything - paired with every allocation */
    free(a);
    free(b);
    a = NULL;
    b = NULL;
    printf("\nAll memory freed (a = NULL, b = NULL)\n");

    return 0;
}
