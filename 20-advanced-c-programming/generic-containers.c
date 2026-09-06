/*
Generic Containers with void* - Master Level C
----------------------------------------------
What this program solves:
Builds a TYPE-GENERIC dynamic vector using void* and element
sizes (memcpy) - the technique behind real libraries' container
implementations. Stores ANY data type: int, double, struct, even
strings - decided at runtime by element size.

Real life example:
Library containers (vector in C++, GLib's GArray) store ANY type
by copying raw bytes - void* + memcpy is the C mechanism behind
generic data structures.

DIAGRAM (a void* vector stores raw bytes of any element size):
  data: [ e0 ][ e1 ][ e2 ] ...   element_size = 16 bytes
  index i -> data + i * element_size  (byte arithmetic)
  push: memcpy(data + n*size, &item, size) after growth check.

BEGINNER EXPLANATION (the simplest way to think):
A vector normally stores ints because it says int*. Make it store
unsigned char BYTES instead, remember how many bytes one element
takes (elemSize), and copy whole elements with memcpy. Now the
same container code works for int (4B), double (8B), or a 40-byte
struct - the container never needs to know what the data IS.
That is exactly how C standard qsort works, and how C++ vector
would work without templates.

SUPER SIMPLE EXAMPLE:
push(vec, &x) with elemSize = sizeof(int):
memcpy(vec->data + vec->count * 4, &x, 4);
The pointer arithmetic is in BYTES; one int = 4 bytes so slot i
starts at data + i*4. Swap elemSize to 8 and suddenly it stores
doubles with zero code change.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Passing a VALUE instead of an ADDRESS: push(vec, 5) is wrong -
  you must push(vec, &x). The container copies from the pointer
  you give it; forgetting & is the classic bug (compiler warns).
- Element larger than expected: elemSize mismatch = silent memory
  corruption. sizeof(type) at the call site keeps them in sync.
- Reading back: *(int*)vecGet(&v, i) - cast the void* to the right
  type. Wrong cast = garbage values (not a crash necessarily -
  worse: silently wrong data).
- Capacity growth: doubling on full - amortized O(1) push (the
  same amortized analysis idea used in dynamic arrays everywhere).

DRY RUN (int vector, initial capacity 2):
push 10 -> data[0..3] = 10, count=1
push 20 -> count=2 (capacity full)
push 30 -> grow: realloc to 4 slots, memcpy moves old bytes, push
           -> count=3
vecGet(&v, 1) -> *(int*)(data + 1*4) = 20.
Same vector re-typed for doubles: push 2.5, 3.5, 4.5 - identical
code path, 8-byte elements.

TIPS:
- The three ingredients of generic C: void* for storage, size_t
  elemSize for stride, memcpy/memmove for element transfer.
- memcpy when regions don't overlap; memmove when they might.
- Alignment: malloc returns suitably aligned memory; casting the
  void* back to the right pointer type keeps alignment correct.
- Follow-up mastery: implement bsearch on your vector, then an
  insert-sorted vector - the container becomes a sorted set.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Vector - Generic dynamic array of fixed-size elements.
 */
typedef struct VectorImpl {
    unsigned char* data;
    size_t count;
    size_t capacity;
    size_t elemSize;
} Vector;

/*
 * vecInit - Initializes an empty generic vector.
 */
void vecInit(Vector* v, size_t elemSize, size_t initialCapacity) {
    v->elemSize = elemSize;
    v->count = 0;
    v->capacity = initialCapacity > 0 ? initialCapacity : 4;
    v->data = (unsigned char*)malloc(v->capacity * elemSize);
    if (v->data == NULL) {
        printf("Allocation failed\n");
        exit(1);
    }
}

/*
 * vecPush - Copies one element in; grows by doubling when full.
 */
void vecPush(Vector* v, const void* element) {
    if (v->count == v->capacity) {
        size_t newCap = v->capacity * 2;
        unsigned char* bigger =
            (unsigned char*)realloc(v->data, newCap * v->elemSize);
        if (bigger == NULL) {
            printf("realloc failed\n");
            exit(1);
        }
        v->data = bigger;
        v->capacity = newCap;
        printf("  (grew capacity to %zu)\n", newCap);
    }
    memcpy(v->data + v->count * v->elemSize, element, v->elemSize);
    v->count = v->count + 1;
}

/*
 * vecGet - Returns pointer to element i (caller casts).
 */
void* vecGet(Vector* v, size_t index) {
    if (index >= v->count) {
        return NULL;
    }
    return v->data + index * v->elemSize;
}

/*
 * vecFree - Releases the buffer.
 */
void vecFree(Vector* v) {
    free(v->data);
    v->data = NULL;
    v->count = 0;
    v->capacity = 0;
}

/*
 * Point - Sample struct to prove generics.
 */
typedef struct {
    int x;
    int y;
} Point;

int main() {
    printf("Generic Vector with void* (works for ANY type)\n\n");

    /* PART 1: vector of int */
    Vector vi;
    vecInit(&vi, sizeof(int), 2);

    printf("Int vector - pushing 10, 20, 30:\n");
    int x = 10;
    vecPush(&vi, &x);
    x = 20;
    vecPush(&vi, &x);
    x = 30;
    vecPush(&vi, &x);

    printf("Contents: ");
    for (size_t i = 0; i < vi.count; i++) {
        printf("%d ", *(int*)vecGet(&vi, i));
    }
    printf("\n\n");

    vecFree(&vi);

    /* PART 2: SAME container code for double */
    Vector vd;
    vecInit(&vd, sizeof(double), 2);

    printf("Double vector - pushing 2.5, 3.5, 4.5:\n");
    double d = 2.5;
    vecPush(&vd, &d);
    d = 3.5;
    vecPush(&vd, &d);
    d = 4.5;
    vecPush(&vd, &d);

    printf("Contents: ");
    for (size_t i = 0; i < vd.count; i++) {
        printf("%.1f ", *(double*)vecGet(&vd, i));
    }
    printf("\n\n");

    vecFree(&vd);

    /* PART 3: SAME container code for a struct */
    Vector vp;
    vecInit(&vp, sizeof(Point), 2);

    printf("Point struct vector:\n");
    Point p1 = {1, 2};
    Point p2 = {3, 4};
    vecPush(&vp, &p1);
    vecPush(&vp, &p2);

    for (size_t i = 0; i < vp.count; i++) {
        Point* p = (Point*)vecGet(&vp, i);
        printf("  Point(%d, %d)\n", p->x, p->y);
    }

    vecFree(&vp);

    printf("\nOne container, three types - only elemSize changed.\n");

    return 0;
}
