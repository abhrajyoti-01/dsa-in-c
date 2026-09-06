/*
Pointer Operations - Parts of a Pointer
---------------------------------------
What this program solves:
Menu-driven program demonstrating the "parts" (operations) of a
pointer: address-of (&), dereference (*), pointer-to-pointer (**),
pointer arithmetic, array-pointer relationship, pointer comparison,
pointers with functions (call by reference), NULL and void
pointers, and dynamic memory with malloc/free.

Real life example:
Think of a delivery system:
- &house        = the plot number where the house is built
- pointer p     = the delivery slip that stores the plot number
- *p            = actually entering the house
- int **pp      = a register that stores the location OF the slip
Moving the slip's plot number (p = p + 1) walks to the neighbouring
plot - that is pointer arithmetic.

How the process works (the parts):
1. DECLARATION:  int *p;        - holds an address of an int.
2. ADDRESS-OF:   p = &x;        - captures where x lives.
3. DEREFERENCE:  *p read/write  - follows the address to the value.
4. POINTER-TO-POINTER: int **pp = &p;  *pp = p, **pp = x.
5. ARITHMETIC:   p+1 advances sizeof(int) bytes; p2-p1 gives the
                 element count between two pointers of same type.
6. COMPARISON:   p1 < p2, p1 == p2 compare addresses.
7. FUNCTIONS:    passing &x lets a function modify x.
8. NULL POINTER: p = NULL means "points nowhere"; always check.
9. VOID POINTER: void *v can hold any address; cast before use.
10. DYNAMIC:     malloc(sizeof(int)*n) gets memory at runtime;
                 free() returns it.

DIAGRAM (pointer arithmetic walks elements, not bytes):
  arr: [10] [20] [30]     p = arr
        ^p   ^p+1  ^p+2
  arr + i  ==  &arr[i];   *(arr + i)  ==  arr[i]
  swap(&a, &b): the writes go THROUGH the addresses, so the
  caller's variables really change.

Pseudo code:
START
READ x; p = &x; PRINT p, *p
pp = &p; PRINT *pp, **pp
READ n; arr = malloc(...); fill; PRINT *(arr + i)
PRINT arr[1] - arr[0]        (pointer difference)
q = NULL; IF q == NULL: PRINT "null"
v = &x; PRINT *(int*)v
free(arr)
END

Sample input and dry run:
x = 7, p = &x          -> *p = 7
pp = &p                -> **pp = 7
n = 3, arr = {4 8 12}  -> *(arr+2) = 12, diff arr[2]-arr[0] = 2
a = 3, b = 9, swap     -> a = 9, b = 3

Main logic to understand:
Every pointer has the same skeleton - type, star, name - and the
same core operations: & to obtain, * to follow, +/- to move, and ==
to compare. Chaining stars gives multi-level indirection, and
malloc/free extend pointers to runtime-sized memory.
*/

#include <stdio.h>
#include <stdlib.h>

/*
 * swap - Exchanges two integers through their addresses.
 */
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x;
    int* p;
    int** pp;

    printf("Pointer Operations - Parts of a Pointer\n\n");

    /* PART: declaration + address-of + dereference on one variable */
    printf("Enter an integer x: ");
    scanf("%d", &x);

    p = &x;
    printf("\n1. Address-of and dereference:\n");
    printf("&x  = %p   (address of x)\n", (void*)&x);
    printf("p   = %p   (pointer stores the address)\n", (void*)p);
    printf("*p  = %d   (dereference reads x)\n", *p);

    /* PART: write through the pointer */
    printf("\nEnter a value to store via *p: ");
    scanf("%d", &x);
    printf("*p = %d now; x = %d as well (same memory)\n", *p, x);

    /* PART: pointer-to-pointer (double indirection) */
    pp = &p;
    printf("\n2. Pointer-to-pointer:\n");
    printf("p    = %p\n", (void*)p);
    printf("*pp  = %p   (= p)\n", (void*)(*pp));
    printf("**pp = %d   (= x)\n", **pp);

    /* PART: pointer arithmetic with user array (dynamic memory) */
    int n;
    printf("\nEnter how many elements to allocate dynamically (1-10): ");
    scanf("%d", &n);

    if (n < 1 || n > 10) {
        printf("Invalid count\n");
        return 1;
    }

    int* arr = (int*)malloc((size_t)n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("\n3. Pointer arithmetic:\n");
    for (int i = 0; i < n; i++) {
        printf("arr + %d = %p -> *(arr + %d) = %d\n",
               i, (void*)(arr + i), i, *(arr + i));
    }

    /* PART: pointer difference - number of elements between addresses */
    if (n >= 2) {
        int* first = &arr[0];
        int* last = &arr[n - 1];
        printf("\n4. Pointer difference:\n");
        printf("last - first = %td element(s)\n", (ptrdiff_t)(last - first));
        printf("first < last: %s\n", first < last ? "YES" : "NO");
    }

    /* PART: pointer comparison */
    printf("\n5. Pointer comparison:\n");
    printf("arr == &arr[0]: %s\n", (arr == &arr[0]) ? "YES" : "NO");

    /* PART: NULL pointer */
    int* q = NULL;
    printf("\n6. NULL pointer:\n");
    printf("q = %p, q == NULL: %s\n", (void*)q, q == NULL ? "YES" : "NO");
    printf("Safety check before dereference passed\n");

    /* PART: void pointer - generic address, cast before use */
    void* v = &x;
    printf("\n7. Void pointer:\n");
    printf("v holds the address of x; *(int*)v = %d\n", *(int*)v);

    /* PART: call by reference with user values */
    int a;
    int b;
    printf("\n8. Call by reference:\n");
    printf("Enter two integers a and b to swap: ");
    scanf("%d %d", &a, &b);
    swap(&a, &b);
    printf("After swap(&a, &b): a = %d, b = %d\n", a, b);

    /* PART: release dynamic memory */
    free(arr);
    printf("\n9. Dynamic memory freed (free(arr))\n");

    return 0;
}
