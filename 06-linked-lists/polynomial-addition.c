/*
Polynomial Addition & Multiplication (Linked List Classic)
----------------------------------------------------------
What this program solves:
Represents polynomials as linked lists of (coefficient, exponent)
nodes in DECREASING exponent order; adds and multiplies two of
them. THE classic DAA lab problem on linked lists.

Real life example:
Computer algebra systems and signal-processing libraries store
sparse polynomials as term lists and add them by comparing
exponents - exactly this program.

DIAGRAM (add 3x^2+5x+1 and 4x^2+2 - compare exponents):
  3x^2 vs 4x^2 -> 7x^2
  5x^1 vs none -> 5x^1
  none vs 2x^0 -> 2x^0
  result: 7x^2 + 5x + 2   (one linked-list node per term)

BEGINNER EXPLANATION:
A polynomial 4x^3 + 2x + 1 is a list of term nodes:
(4,3) -> (2,1) -> (1,0). Addition merges like exponents (sum
coefficients); multiplication generates every pair product
(exp sums, coef products) then COMBINES equal exponents.

SUPER SIMPLE EXAMPLE:
A = 2x^2 + 1  -> (2,2) -> (1,0)
B = 3x + 4    -> (3,1) -> (4,0)
A + B: exponents 2,1,0 all present -> (2,2) -> (3,1) -> (5,0)
  = 2x^2 + 3x + 5.
A x B: (2,2)*(3,1)=(6,3); (2,2)*(4,0)=(8,2); (1,0)*(3,1)=(3,1);
  (1,0)*(4,0)=(4,0) -> 6x^3 + 8x^2 + 3x + 4.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Cancellation: A = x^2 + x, B = -x^2 + 5 -> sum has x^2 with
  coefficient 0 -> the zero term must be DROPPED (or you print
  ugly "+ 0x^2"). This file drops zero-coefficient terms.
- Empty polynomial (all cancelled): print "0".
- Duplicate exponents in input: combine on insertion - input
  normalization step.
- Multiplication result may need sorting: pair products appear
  out of order -> insert each product into the result list in
  sorted position (insertion into sorted list - a linked list
  skill in itself).

DRY RUN (add A=(2,2)->(1,0), B=(3,1)->(4,0)):
compare (2,2) vs (3,1): 2 > 1 -> take (2,2). result: (2,2)
compare (1,0) vs (3,1): 1 < 1 -> take (3,1). result: (2,2)->(3,1)
compare (1,0) vs (4,0): equal exponents -> sum: (1+4, 0) = (5,0)
result: 2x^2 + 3x + 5.

TIPS:
- Keeping DECREASING exponent order makes addition a linear merge
  (two pointers) - same pattern as merging sorted arrays.
- Multiplication is O(m*n) pairs; use a dummy head node to avoid
  empty-result special cases.
- Extend to subtraction (negate + add) and evaluation (Horner's
  method on the list) - natural next steps.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Term {
    int coef;
    int exp;
    struct Term* next;
} Term;

/*
 * makeTerm - Allocates one polynomial term.
 */
Term* makeTerm(int coef, int exp) {
    Term* t = (Term*)malloc(sizeof(Term));
    t->coef = coef;
    t->exp = exp;
    t->next = NULL;
    return t;
}

/*
 * readPoly - Reads terms; keeps descending exponent order.
 */
Term* readPoly(const char* name) {
    int count;
    printf("How many terms in %s? ", name);
    scanf("%d", &count);

    Term* head = NULL;
    Term* tail = NULL;
    printf("Enter coef exponent for each term (descending exponents):\n");
    for (int i = 0; i < count; i++) {
        int c;
        int e;
        printf("  Term %d: ", i + 1);
        scanf("%d %d", &c, &e);
        if (c == 0) {
            continue;
        }
        Term* t = makeTerm(c, e);
        if (head == NULL) {
            head = t;
        } else {
            tail->next = t;
        }
        tail = t;
    }
    return head;
}

/*
 * printPoly - Human-readable output.
 */
void printPoly(Term* p) {
    if (p == NULL) {
        printf("0\n");
        return;
    }
    int first = 1;
    while (p != NULL) {
        if (!first) {
            printf(p->coef < 0 ? " - " : " + ");
        } else if (p->coef < 0) {
            printf("-");
        }
        int c = p->coef < 0 ? -p->coef : p->coef;
        printf("%dx^%d", c, p->exp);
        first = 0;
        p = p->next;
    }
    printf("\n");
}

/*
 * addPoly - Linear merge of two sorted lists.
 */
Term* addPoly(Term* a, Term* b) {
    Term dummy;
    Term* tail = &dummy;
    dummy.next = NULL;

    while (a != NULL && b != NULL) {
        if (a->exp > b->exp) {
            tail->next = makeTerm(a->coef, a->exp);
            tail = tail->next;
            a = a->next;
        } else if (a->exp < b->exp) {
            tail->next = makeTerm(b->coef, b->exp);
            tail = tail->next;
            b = b->next;
        } else {
            int sum = a->coef + b->coef;
            if (sum != 0) {
                tail->next = makeTerm(sum, a->exp);
                tail = tail->next;
            }
            a = a->next;
            b = b->next;
        }
    }
    while (a != NULL) {
        tail->next = makeTerm(a->coef, a->exp);
        tail = tail->next;
        a = a->next;
    }
    while (b != NULL) {
        tail->next = makeTerm(b->coef, b->exp);
        tail = tail->next;
        b = b->next;
    }
    tail->next = NULL;
    return dummy.next;
}

/*
 * insertSorted - Inserts a term keeping descending exponent order;
 * merges equal exponents (drops zero sums).
 */
void insertSorted(Term** head, int coef, int exp) {
    if (coef == 0) {
        return;
    }
    Term* prev = NULL;
    Term* curr = *head;
    while (curr != NULL && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL && curr->exp == exp) {
        curr->coef = curr->coef + coef;
        if (curr->coef == 0) {
            if (prev == NULL) {
                *head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
        }
        return;
    }
    Term* t = makeTerm(coef, exp);
    t->next = curr;
    if (prev == NULL) {
        *head = t;
    } else {
        prev->next = t;
    }
}

/*
 * multiplyPoly - All pairs, inserted in sorted order.
 */
Term* multiplyPoly(Term* a, Term* b) {
    Term* result = NULL;
    for (Term* pa = a; pa != NULL; pa = pa->next) {
        for (Term* pb = b; pb != NULL; pb = pb->next) {
            insertSorted(&result, pa->coef * pb->coef,
                         pa->exp + pb->exp);
        }
    }
    return result;
}

void freePoly(Term* p) {
    while (p != NULL) {
        Term* next = p->next;
        free(p);
        p = next;
    }
}

int main() {
    printf("Polynomial Addition & Multiplication (linked lists)\n\n");

    Term* a = readPoly("A");
    Term* b = readPoly("B");

    printf("\nA(x) = ");
    printPoly(a);
    printf("B(x) = ");
    printPoly(b);

    Term* sum = addPoly(a, b);
    printf("A+B  = ");
    printPoly(sum);

    Term* product = multiplyPoly(a, b);
    printf("A*B  = ");
    printPoly(product);

    freePoly(a);
    freePoly(b);
    freePoly(sum);
    freePoly(product);

    return 0;
}
