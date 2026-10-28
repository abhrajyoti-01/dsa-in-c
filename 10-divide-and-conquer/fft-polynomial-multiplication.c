/*
Fast Fourier Transform - Polynomial Multiplication in O(n log n) (Master)
------------------------------------------------------------------------
PROBLEM DEFINITION:
Multiply two polynomials A(x) and B(x) of degree up to 1000. Multiplying
coefficient lists directly costs O(n^2). The FFT evaluates both
polynomials at the n complex n-th roots of unity in O(n log n),
multiplies the values pointwise in O(n), and interpolates back with the
inverse FFT in O(n log n).

REAL-LIFE EXAMPLE:
Audio and image filters (convolution), big-integer multiplication, and
signal processing in every phone modem multiply huge "polynomials" -
sample sequences - using the FFT. A 1-second audio clip has 44100
samples; O(n^2) convolution would be far too slow.

DIAGRAM (the three-step pipeline):
   coefficients            point values               coefficients
  A: 1 2 3 0  --FFT-->  A(1) A(i) A(-1) A(-i) \
                                                x (pointwise) --IFFT--> C
  B: 4 5 0 0  --FFT-->  B(1) B(i) B(-1) B(-i) /
  Divide and conquer: A(x) = Aeven(x^2) + x * Aodd(x^2), and the
  squares of the n-th roots are the (n/2)-th roots, so each half is a
  size-n/2 FFT. With w a root:  A(w) = E + w*O,  A(-w) = E - w*O.

PSEUDO CODE:
  FFT(a, n, invert):
      if n == 1: return
      even = a[0], a[2], ...;  odd = a[1], a[3], ...
      FFT(even, n/2, invert);  FFT(odd, n/2, invert)
      for k = 0 .. n/2 - 1:
          w = exp(+-2*pi*i*k / n)          (minus sign when inverting)
          a[k]       = even[k] + w * odd[k]
          a[k + n/2] = even[k] - w * odd[k]
  Multiply(A, B):
      n = power of two >= deg A + deg B + 1; pad with zeros
      FFT(A), FFT(B); C[j] = A[j] * B[j]; FFT(C, invert); divide by n
      round every real part to the nearest integer

SAMPLE INPUT & DRY RUN:
  2
  1 2 3
  1
  4 5
  A(x) = 1 + 2x + 3x^2,  B(x) = 4 + 5x,  n = 4, roots 1, i, -1, -i
  A at the roots:  6,  -2+2i,  2,  -2-2i
  B at the roots:  9,   4+5i, -1,   4-5i
  products:       54, -18-2i, -2, -18+2i
  inverse FFT, e.g. c0 = (54 + (-18-2i) + (-2) + (-18+2i)) / 4 = 4
  C(x) = 4 + 13x + 22x^2 + 15x^3   (matches the school method)

SPECIAL CASES:
  - Degree-0 polynomials (constants) give n = 1 and no recursion.
  - Negative coefficients are allowed; rounding uses floor(v + 0.5).
  - Floating-point error grows with n and coefficient size; the limits
    (degree <= 1000, |coefficient| <= 1000) keep every product below
    about 10^9, far inside double precision. The program re-checks the
    result with the O(n^2) method.
  - Leading zero coefficients are kept as entered.

COMPLEXITY:
  O(n log n) time with n the padded size, O(n log n) temporary memory
  in this recursive version (an iterative bit-reversal FFT uses O(n)).

TIPS:
  Link with the math library on Linux: gcc ... fft-polynomial-
  multiplication.c -lm. For exact integer results modulo a prime, use
  the Number Theoretic Transform (same code, roots of unity mod p).
  Compare karatsuba (O(n^1.585)) and strassen.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DEGREE 1000
#define COEFFICIENT_LIMIT 1000
#define TRACE_LIMIT 8
#define PI 3.14159265358979323846

typedef struct {
    double re;
    double im;
} Complex;

Complex multiplyComplex(Complex a, Complex b) {
    Complex c = {a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re};
    return c;
}

void fft(Complex *a, int n, int invert) {
    if (n == 1) return;
    int half = n / 2;
    Complex *even = malloc(half * sizeof(Complex));
    Complex *odd = malloc(half * sizeof(Complex));
    if (even == NULL || odd == NULL) {
        printf("Out of memory\n");
        exit(1);
    }
    for (int i = 0; i < half; i++) {
        even[i] = a[2 * i];
        odd[i] = a[2 * i + 1];
    }
    fft(even, half, invert);
    fft(odd, half, invert);
    double angle = 2 * PI / n * (invert ? -1 : 1);
    for (int k = 0; k < half; k++) {
        Complex w = {cos(angle * k), sin(angle * k)};
        Complex t = multiplyComplex(w, odd[k]);
        a[k].re = even[k].re + t.re;
        a[k].im = even[k].im + t.im;
        a[k + half].re = even[k].re - t.re;
        a[k + half].im = even[k].im - t.im;
    }
    free(even);
    free(odd);
}

/* Prints a + bi rounded to 2 decimals, without "-0.00" noise. */
void printComplex(Complex c) {
    double re = fabs(c.re) < 0.005 ? 0.0 : c.re;
    double im = fabs(c.im) < 0.005 ? 0.0 : c.im;
    if (im == 0.0) printf("%10.2f        ", re);
    else printf("%10.2f%+7.2fi", re, im);
}

void printPolynomial(const long long *c, int degree) {
    for (int i = 0; i <= degree; i++) {
        if (i > 0) printf(c[i] < 0 ? " - " : " + ");
        long long magnitude = i > 0 && c[i] < 0 ? -c[i] : c[i];
        if (i == 0) printf("%lld", c[i]);
        else if (i == 1) printf("%lldx", magnitude);
        else printf("%lldx^%d", magnitude, i);
    }
    printf("\n");
}

int readPolynomial(const char *name, long long *c, int *degree) {
    printf("Degree of %s (0-%d): ", name, MAX_DEGREE);
    if (scanf("%d", degree) != 1 || *degree < 0 || *degree > MAX_DEGREE) {
        return 0;
    }
    printf("Coefficients of %s from x^0 up to x^%d: ", name, *degree);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lld", &c[i]) != 1 || c[i] < -COEFFICIENT_LIMIT ||
            c[i] > COEFFICIENT_LIMIT) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    static long long a[MAX_DEGREE + 1];
    static long long b[MAX_DEGREE + 1];
    static long long product[2 * MAX_DEGREE + 1];
    static long long school[2 * MAX_DEGREE + 1];
    static Complex fa[4096];
    static Complex fb[4096];
    int degreeA;
    int degreeB;

    printf("FFT - Polynomial Multiplication\n\n");
    if (!readPolynomial("A", a, &degreeA) || !readPolynomial("B", b, &degreeB)) {
        printf("Invalid polynomial\n");
        return 1;
    }

    int degreeC = degreeA + degreeB;
    int n = 1;
    while (n < degreeC + 1) n *= 2;
    for (int i = 0; i < n; i++) {
        fa[i].re = i <= degreeA ? (double)a[i] : 0.0;
        fb[i].re = i <= degreeB ? (double)b[i] : 0.0;
        fa[i].im = 0.0;
        fb[i].im = 0.0;
    }

    fft(fa, n, 0);
    fft(fb, n, 0);
    if (n <= TRACE_LIMIT) {
        printf("\nn = %d; values at the roots w^j = exp(2*pi*i*j/%d):\n", n, n);
        printf("   j            A(w^j)             B(w^j)       A*B(w^j)\n");
    }
    for (int j = 0; j < n; j++) {
        Complex c = multiplyComplex(fa[j], fb[j]);
        if (n <= TRACE_LIMIT) {
            printf("  %2d ", j);
            printComplex(fa[j]);
            printComplex(fb[j]);
            printComplex(c);
            printf("\n");
        }
        fa[j] = c;
    }
    fft(fa, n, 1);
    for (int i = 0; i <= degreeC; i++) {
        product[i] = (long long)floor(fa[i].re / n + 0.5);
    }

    for (int i = 0; i <= degreeA; i++) {
        for (int j = 0; j <= degreeB; j++) school[i + j] += a[i] * b[j];
    }
    int same = 1;
    for (int i = 0; i <= degreeC; i++) {
        if (product[i] != school[i]) same = 0;
    }

    printf("\nA(x) = ");
    printPolynomial(a, degreeA);
    printf("B(x) = ");
    printPolynomial(b, degreeB);
    printf("C(x) = ");
    printPolynomial(product, degreeC);
    printf("School-method check: %s\n", same ? "same" : "MISMATCH");
    return 0;
}
