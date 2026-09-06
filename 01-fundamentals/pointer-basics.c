/*
Pointers in C - Basics
----------------------
What this program solves:
Input-based demonstration of pointer fundamentals: taking an
address with &, dereferencing with *, changing a variable through
its pointer, pointer arithmetic on a user-entered array, proving
arr[i] == *(arr + i), and pass-by-reference functions (swap, add).

Real life example:
A pointer is like a house address written on a paper. The paper is
not the house, but anyone holding the paper can go to that exact
house and change things inside it. When you give a courier (function)
the address instead of a copy of the house, the courier can actually
deliver (modify) at the real location.

DIAGRAM (a pointer stores an ADDRESS, * dereferences it):
  address:   1000    1004    1008
  value:    [ 42 ]  [     ]  [1000]
               ^               |
               x               p  (p = &x)
  p = &x  -> p holds 1000.  *p -> reads 42 through the address.

How the process works:
1. int x = 10; reserves 4 bytes somewhere in memory.
2. int *p = &x; stores the ADDRESS of x in p (& = address-of).
3. *p reads or writes through the address (* = dereference).
   *p = 50 changes x itself.
4. Pointer arithmetic moves by the SIZE of the type:
   p + 1 advances by sizeof(int) bytes, not 1 byte.
5. For arrays, arr[i] is exactly *(arr + i); the array name acts
   like a pointer to its first element.
6. Passing &x to a function lets that function change x (pass by
   reference), which plain arguments cannot do.

Pseudo code:
START
READ x
p = &x
PRINT p, *p
READ new value; *p = newValue    (changes x)
READ n and n array elements
FOR i = 0..n-1: PRINT *(arr + i)
READ a, b; swap(&a, &b); PRINT a, b
END

Sample input and dry run:
x = 10, p = &x, *p = 50      -> x is now 50
n = 5, arr = {5, 10, 15, 20, 25}
*(arr + 2) = 15
a = 3, b = 9; swap(&a, &b)   -> a = 9, b = 3

Main logic to understand:
A pointer stores an address. & gives an address, * follows the
address. Pointer arithmetic scales by sizeof(type), and arrays and
pointers are intimately connected. Pointers enable pass-by-reference,
dynamic memory, and efficient data structures.
*/

#include <stdio.h>

#define MAX_SIZE 10

/*
 * swap - Exchanges two integers through their addresses.
 */
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * addFive - Adds 5 to the caller's variable via its address.
 */
void addFive(int* n) {
    *n = *n + 5;
}

int main() {
    int x;
    int* p;

    printf("Pointers in C - Basics\n\n");

    /* STEP: declare a pointer, take the address of a user value */
    printf("Enter an integer x: ");
    scanf("%d", &x);
    p = &x;

    printf("\nx   = %d   (the value)\n", x);
    printf("&x  = %p   (address of x)\n", (void*)&x);
    printf("p   = %p   (pointer holding that address)\n", (void*)p);
    printf("*p  = %d   (dereference: value at that address)\n\n", *p);

    /* STEP: dereference to WRITE - change x through the pointer */
    int newValue;
    printf("Enter a new value to write via *p: ");
    scanf("%d", &newValue);
    *p = newValue;
    printf("After *p = %d: x = %d   (x itself changed!)\n\n", newValue, x);

    /* STEP: re-assign the pointer to another variable */
    int y;
    printf("Enter another integer y: ");
    scanf("%d", &y);
    p = &y;
    printf("Re-pointed p to y: *p = %d\n\n", *p);

    /* STEP: pointer arithmetic on a user-entered array */
    int arr[MAX_SIZE];
    int n;

    printf("How many array elements (1-%d)? ", MAX_SIZE);
    scanf("%d", &n);

    if (n < 1 || n > MAX_SIZE) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d integers separated by space: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int* q = arr;
    printf("\nPointer arithmetic (q + i moves by %zu bytes each step):\n",
           sizeof(int));
    for (int i = 0; i < n; i++) {
        printf("q + %d = %p -> *(q + %d) = %d\n",
               i, (void*)(q + i), i, *(q + i));
    }

    /* STEP: prove arr[i] and *(arr + i) are the same thing */
    printf("\narr[i] == *(arr + i) check:\n");
    for (int i = 0; i < n; i++) {
        printf("arr[%d] = %d   *(arr + %d) = %d\n",
               i, arr[i], i, *(arr + i));
    }

    /* STEP: walk the array by moving the pointer itself */
    printf("\nWalking with q++:\n");
    for (q = arr; q < arr + n; q++) {
        printf("*q = %d  (address %p)\n", *q, (void*)q);
    }

    /* STEP: pass by reference - functions change caller's data */
    int a;
    int b;
    printf("\nEnter two integers a and b to swap: ");
    scanf("%d %d", &a, &b);

    printf("swap before: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("swap(&a, &b) after: a = %d, b = %d\n", a, b);

    addFive(&a);
    printf("addFive(&a) -> a = %d\n", a);

    return 0;
}
