/*
Karatsuba Multiplication - Big Integers with 3 Products Instead of 4 (Advanced)
-------------------------------------------------------------------------------
PROBLEM DEFINITION:
Multiply two non-negative integers with up to 1000 digits each. The
school method multiplies every digit by every digit: O(n^2). Karatsuba
splits each number into a high and a low half and needs only THREE
half-size products instead of four, giving O(n^log2(3)) = O(n^1.585).
The program also runs the school method and checks both answers agree.

REAL-LIFE EXAMPLE:
RSA keys and cryptographic signatures multiply numbers with hundreds of
digits millions of times a day. Libraries such as GMP and Java's
BigInteger switch to Karatsuba once numbers grow past a few dozen words.

DIAGRAM (x = 1234, y = 5678, split into halves of 2 digits):
  x = 12 | 34      a1 = 12, a0 = 34
  y = 56 | 78      b1 = 56, b0 = 78
  z2 = a1*b1             = 12*56          =  672
  z0 = a0*b0             = 34*78          = 2652
  z1 = (a1+a0)(b1+b0) - z2 - z0 = 46*134 - 672 - 2652 = 2840
  x*y = z2*10^4 + z1*10^2 + z0 = 6720000 + 284000 + 2652 = 7006652

PSEUDO CODE:
  Karatsuba(a, b, n):              (a, b are n-digit, n a power of 2)
      if n == 1: return a[0] * b[0]
      h = n / 2;  split a = a1*B^h + a0,  b = b1*B^h + b0
      z0 = Karatsuba(a0, b0, h)
      z2 = Karatsuba(a1, b1, h)
      z1 = Karatsuba(a0 + a1, b0 + b1, h) - z0 - z2
      return z2*B^(2h) + z1*B^h + z0
  Digits are kept as unnormalized "coefficients" during recursion and
  carries are propagated once at the end.

SAMPLE INPUT & DRY RUN:
  1234
  5678
  Both padded to n = 4 digits. Top level: three products of 2-digit
  halves (shown above); each of those makes three 1-digit products.
  Product: 7006652
  Single-digit multiplications: Karatsuba 9 (= 3^2), school 16 (= 4^2)

SPECIAL CASES:
  - Zero and leading zeros ("000123") are handled; the product of
    anything with 0 prints a single 0.
  - Different lengths are padded with zeros to the next power of two.
  - The sums a0 + a1 may have "digits" above 9; that is fine because
    carries are resolved only at the very end.
  - Up to 1000 digits per number, digits only (no sign).

COMPLEXITY:
  T(n) = 3T(n/2) + O(n)  ->  O(n^1.585) time; O(n) extra space per level.
  School method: O(n^2). For 1000 digits: about 59 thousand single-digit
  products instead of a million.

TIPS:
  Real implementations stop recursing below ~32 digits and use the
  school method there (less overhead). The same "save one product"
  idea gives Strassen for matrices; FFT pushes it to O(n log n).
  See master-theorem for solving T(n) = 3T(n/2) + n.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 1000

long long karatsubaProducts;

void *allocate(size_t count) {
    void *memory = calloc(count, sizeof(long long));
    if (memory == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    return memory;
}

/* result (length 2n) = a * b, where a and b have n coefficients each. */
void karatsuba(const long long *a, const long long *b, int n,
               long long *result) {
    for (int i = 0; i < 2 * n; i++) result[i] = 0;
    if (n == 1) {
        result[0] = a[0] * b[0];
        karatsubaProducts++;
        return;
    }
    int half = n / 2;
    long long *buffer = allocate(4 * (size_t)n);
    long long *low = buffer;              /* z0: n entries */
    long long *high = buffer + n;         /* z2: n entries */
    long long *middle = buffer + 2 * n;   /* z1: n entries */
    long long *sumA = buffer + 3 * n;     /* half entries */
    long long *sumB = sumA + half;        /* half entries */

    karatsuba(a, b, half, low);
    karatsuba(a + half, b + half, half, high);
    for (int i = 0; i < half; i++) {
        sumA[i] = a[i] + a[i + half];
        sumB[i] = b[i] + b[i + half];
    }
    karatsuba(sumA, sumB, half, middle);
    for (int i = 0; i < n; i++) middle[i] -= low[i] + high[i];
    for (int i = 0; i < n; i++) {
        result[i] += low[i];
        result[i + half] += middle[i];
        result[i + n] += high[i];
    }
    free(buffer);
}

/* Propagates base-10 carries; returns the number of significant digits. */
int normalize(long long *digits, int length) {
    long long carry = 0;
    for (int i = 0; i < length; i++) {
        long long value = digits[i] + carry;
        digits[i] = value % 10;
        carry = value / 10;
    }
    int top = length - 1;
    while (top > 0 && digits[top] == 0) top--;
    return top + 1;
}

/* Reads a decimal string, strips leading zeros, stores little-endian. */
int readNumber(long long *digits, int *length) {
    char text[MAX_DIGITS + 2];
    if (scanf("%1001s", text) != 1) return 0;
    int len = (int)strlen(text);
    if (len > MAX_DIGITS) return 0;
    for (int i = 0; i < len; i++) {
        if (text[i] < '0' || text[i] > '9') return 0;
    }
    int start = 0;
    while (start < len - 1 && text[start] == '0') start++;
    *length = len - start;
    for (int i = 0; i < *length; i++) digits[i] = text[len - 1 - i] - '0';
    return 1;
}

void printNumber(const long long *digits, int length) {
    for (int i = length - 1; i >= 0; i--) printf("%lld", digits[i]);
    printf("\n");
}

int main(void) {
    static long long a[MAX_DIGITS * 2];
    static long long b[MAX_DIGITS * 2];
    static long long fast[MAX_DIGITS * 4];
    static long long school[MAX_DIGITS * 2];
    int lengthA;
    int lengthB;

    printf("Karatsuba Multiplication\n\n");
    printf("First number (up to %d digits): ", MAX_DIGITS);
    if (!readNumber(a, &lengthA)) {
        printf("Invalid number\n");
        return 1;
    }
    printf("Second number (up to %d digits): ", MAX_DIGITS);
    if (!readNumber(b, &lengthB)) {
        printf("Invalid number\n");
        return 1;
    }

    int n = 1;
    while (n < lengthA || n < lengthB) n *= 2;
    karatsuba(a, b, n, fast);
    int fastLength = normalize(fast, 2 * n);

    long long schoolProducts = 0;
    for (int i = 0; i < lengthA; i++) {
        for (int j = 0; j < lengthB; j++) {
            school[i + j] += a[i] * b[j];
            schoolProducts++;
        }
    }
    int schoolLength = normalize(school, lengthA + lengthB);

    printf("\nPadded length n = %d\nProduct: ", n);
    printNumber(fast, fastLength);
    printf("Single-digit multiplications: Karatsuba %lld, school %lld\n",
           karatsubaProducts, schoolProducts);
    int same = fastLength == schoolLength;
    for (int i = 0; same && i < fastLength; i++) {
        if (fast[i] != school[i]) same = 0;
    }
    printf("School-method check: %s\n", same ? "same" : "MISMATCH");
    return 0;
}
