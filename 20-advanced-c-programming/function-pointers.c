/*
Function Pointers and Callbacks - Master Level C
------------------------------------------------
What this program solves:
The highest-level C technique: passing FUNCTIONS as data.
Demonstrates function pointers, callback functions, dispatch
tables (replacing switch with an array of functions), qsort with
custom comparators, and closures-by-struct (function + context).

Real life example:
qsort callbacks, button handlers in GUI toolkits and interrupt
vectors in embedded firmware are dispatch tables of function
pointers.

DIAGRAM (a dispatch table = array of function pointers):
  int add(int,int), sub(int,int)...
  op[0] = add; op[1] = sub;
  result = op[choice](a, b);   <- called THROUGH the pointer

BEGINNER EXPLANATION (the simplest way to think):
A function lives at an address in memory, just like data. A
FUNCTION POINTER stores that address. You can store it in a
variable, pass it to another function, call it through the
pointer - the called code decides AT RUNTIME which function runs.
"Callback" = handing your function to library code so IT calls
YOU back at the right moment.

SUPER SIMPLE EXAMPLE:
int add(int a, int b) { return a + b; }
int (*op)(int, int) = add;   // pointer to function
int r = op(2, 3);            // calls add -> r = 5
Swap: op = subtract; same call site now computes 2-3. The caller
did not change - the BEHAVIOR did.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Signature mismatch: assigning a function with different
  parameter types to the pointer compiles with a warning in C but
  calls are undefined behavior - the compiler cannot check this
  for you. Match the types EXACTLY.
- NULL function pointer: calling through NULL crashes - always
  check before dispatch.
- qsort comparator order: returning negative means "first arg
  comes first". Returning a-b vs b-a flips the sort direction -
  the single most common callback bug.
- Dispatch table indexes: out-of-range menu choice indexes past
  the array - bounds-check before calling.

DRY RUN (dispatch table):
ops[] = {add, subtract, multiply};
menu choice 2 -> ops[2](6, 7) -> multiply(6,7) = 42.
Same program, three behaviors, ONE call site:
result = ops[i](a, b);
That indirection is the essence: data (the index) selects code.

TIPS:
- Read the type aloud: int (*op)(int, int) = "pointer to function
  taking (int,int) returning int". Parentheses around (*op) are
  mandatory - int *op(int,int) would declare a FUNCTION returning
  int*.
- qsort comparator pattern: compare structs by multiple keys by
  comparing key1, if equal key2, etc. This file sorts a struct
  both ways.
- Real uses: interrupt vectors, plugin systems, state machines,
  thread entry points (pthread_create), event handlers (GUI),
  qsort/bsearch callbacks. Master this and C stops feeling low-level.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Arithmetic ops with identical signatures - interchangeable.
 */
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

/*
 * applyOperation - The CALLER receives a callback and invokes it.
 */
int applyOperation(int a, int b, int (*operation)(int, int)) {
    if (operation == NULL) {
        printf("No operation provided (NULL callback)\n");
        return 0;
    }
    return operation(a, b);
}

/*
 * Student - Struct for qsort comparator demos.
 */
typedef struct {
    char name[20];
    int marks;
    int roll;
} Student;

/*
 * compareByMarksDesc - qsort callback: marks descending.
 */
int compareByMarksDesc(const void* a, const void* b) {
    const Student* x = (const Student*)a;
    const Student* y = (const Student*)b;
    return y->marks - x->marks;
}

/*
 * compareByRollAsc - qsort callback: roll ascending.
 */
int compareByRollAsc(const void* a, const void* b) {
    const Student* x = (const Student*)a;
    const Student* y = (const Student*)b;
    return x->roll - y->roll;
}

int main() {
    int a;
    int b;
    int choice;

    printf("Function Pointers and Callbacks\n\n");

    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);

    /* PART 1: direct function pointer variable */
    int (*op)(int, int);

    op = add;
    printf("\nDirect pointer: add(%d, %d) = %d\n", a, b, op(a, b));
    op = subtract;
    printf("Re-pointed:     %d - %d = %d\n", a, b, op(a, b));

    /* PART 2: callback - function receives a function */
    printf("\nCallback via applyOperation:\n");
    printf("apply(%d, %d, add)      = %d\n", a, b,
           applyOperation(a, b, add));
    printf("apply(%d, %d, multiply) = %d\n", a, b,
           applyOperation(a, b, multiply));
    printf("apply(%d, %d, NULL)     = %d\n", a, b,
           applyOperation(a, b, NULL));

    /* PART 3: dispatch table - array of function pointers */
    int (*ops[3])(int, int) = {add, subtract, multiply};
    const char* names[3] = {"add", "subtract", "multiply"};

    printf("\nDispatch table (0=add, 1=subtract, 2=multiply): ");
    scanf("%d", &choice);

    if (choice >= 0 && choice < 3) {
        printf("ops[%d] is %s -> %d %s %d = %d\n",
               choice, names[choice], a, names[choice], b,
               ops[choice](a, b));
    } else {
        printf("Out of range - bounds check saved us\n");
    }

    /* PART 4: qsort with custom comparators */
    Student students[5] = {
        {"Riya", 88, 3},
        {"Arjun", 92, 1},
        {"Diya", 75, 2},
        {"Kabir", 88, 5},
        {"Meera", 81, 4}
    };

    printf("\nqsort by marks DESC:\n");
    qsort(students, 5, sizeof(Student), compareByMarksDesc);
    for (int i = 0; i < 5; i++) {
        printf("  %-10s marks %3d roll %d\n",
               students[i].name, students[i].marks, students[i].roll);
    }

    printf("qsort by roll ASC:\n");
    qsort(students, 5, sizeof(Student), compareByRollAsc);
    for (int i = 0; i < 5; i++) {
        printf("  %-10s marks %3d roll %d\n",
               students[i].name, students[i].marks, students[i].roll);
    }

    /* PART 5: array of function pointers taking no args (state machine feel) */
    void (*greeter)(void);
    extern void greetHello(void);
    greeter = greetHello;
    greeter();

    return 0;
}

/*
 * greetHello - Simple void(void) callback target.
 */
void greetHello(void) {
    printf("\nHello from a function called through a pointer!\n");
}
