/*
Swap, Conversions and Number Puzzles (Basic -> Hard Ladder)
-----------------------------------------------------------
Real life example:
Unit and currency converters run exactly these divide-and-read-
remainder loops; XOR swap is a classic low-memory embedded trick.

DIAGRAM (swap with a temp variable; decimal to binary):
  temp = a;  a = b;  b = temp          (a and b exchanged)
  13 -> binary:  13/2=6 r1, 6/2=3 r0, 3/2=1 r1, 1/2=0 r1
  read remainders BOTTOM-UP: 1101

LADDER (easy -> hard inside this file):
  1. Swap with temp          (easiest)
  2. Swap without temp       (arithmetic trick)
  3. Swap with XOR           (bit trick)
  4. Decimal -> Binary       (repeated division)
  5. Binary -> Decimal       (positional weights)
  6. Decimal -> Any base     (generalization)
  7. Number reverse + palindrome (digit loop combo)

BEGINNER EXPLANATION:
Swapping = exchanging two box contents. With a temp box it is
trivial; without one you must be clever: use SUMS or XOR to carry
the information. Number base conversion = either divide down
(decimal->binary) or weight up (binary->decimal).

SUPER SIMPLE EXAMPLE:
Swap 3, 7 without temp: a = a+b = 10; b = a-b = 3; a = a-b = 7.
Decimal 13 -> binary: 13/2=6 r1, 6/2=3 r0, 3/2=1 r1, 1/2=0 r1
-> read remainders UP: 1101.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- XOR swap with EQUAL values: a^=a makes a=0 FIRST -> both become
  0! Guard with if (a != b). Classic interview trap.
- Arithmetic swap overflow: a+b can overflow for huge values -
  XOR swap does not (but has the equal-value trap instead).
- Decimal -> base 1? Illegal. Base must be 2..16; digits above 9
  print as letters A-F.
- Reverse of 100 -> 001 = 1: leading zeros vanish by definition -
  palindrome check must use the digit count, not string length
  assumptions.

DRY RUN (13 -> binary, remainders collected):
13 % 2 = 1, 13/2 = 6
6 % 2 = 0, 6/2 = 3
3 % 2 = 1, 3/2 = 1
1 % 2 = 1, 1/2 = 0
Remainders bottom-up: 1101 -> verify 8+4+0+1 = 13.

TIPS:
- Know all three swaps - interviewers ask each in turn and expect
  the special-case analysis above.
- Base conversion appears inside radix sort, bit manipulation and
  hashing - master it here, reuse it everywhere.
*/

#include <stdio.h>

void swapTemp(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void swapArithmetic(int* a, int* b) {
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void swapXor(int* a, int* b) {
    if (a == b) {
        return;
    }
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void printBinary(int n) {
    if (n == 0) {
        printf("0");
        return;
    }
    int bits[32];
    int count = 0;
    while (n > 0) {
        bits[count] = n % 2;
        n = n / 2;
        count = count + 1;
    }
    for (int i = count - 1; i >= 0; i--) {
        printf("%d", bits[i]);
    }
}

void decimalToBase(int n, int base) {
    if (n == 0) {
        printf("0");
        return;
    }
    char digits[64];
    int count = 0;
    while (n > 0) {
        int d = n % base;
        digits[count] = (d < 10) ? (char)('0' + d) : (char)('A' + d - 10);
        count = count + 1;
        n = n / base;
    }
    for (int i = count - 1; i >= 0; i--) {
        printf("%c", digits[i]);
    }
}

long long binaryToDecimal(long long binary) {
    long long result = 0;
    long long weight = 1;
    while (binary > 0) {
        result = result + (binary % 10) * weight;
        weight = weight * 2;
        binary = binary / 10;
    }
    return result;
}

int reverseNumber(int n) {
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n = n / 10;
    }
    return reversed;
}

int main() {
    int choice;
    int a;
    int b;

    printf("Swap & Conversion Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. Swap with temp\n");
        printf("2. Swap without temp (arithmetic)\n");
        printf("3. Swap with XOR\n");
        printf("4. Decimal -> Binary\n");
        printf("5. Binary -> Decimal\n");
        printf("6. Decimal -> any base (2-16)\n");
        printf("7. Reverse a number\n");
        printf("8. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice >= 1 && choice <= 3) {
            printf("Enter two values a b: ");
            scanf("%d %d", &a, &b);
            if (choice == 1) {
                swapTemp(&a, &b);
            } else if (choice == 2) {
                swapArithmetic(&a, &b);
            } else {
                swapXor(&a, &b);
            }
            printf("After swap: a = %d, b = %d\n", a, b);
        } else if (choice == 4) {
            printf("Enter decimal (0-2 billion): ");
            scanf("%d", &a);
            printf("Binary: ");
            printBinary(a);
            printf("\n");
        } else if (choice == 5) {
            printf("Enter binary (only 0s and 1s): ");
            scanf("%lld", &b);
            printf("Decimal: %lld\n", binaryToDecimal(b));
        } else if (choice == 6) {
            printf("Enter decimal and base (2-16): ");
            scanf("%d %d", &a, &b);
            if (b < 2 || b > 16) {
                printf("Base must be 2..16\n");
            } else {
                printf("Base-%d: ", b);
                decimalToBase(a, b);
                printf("\n");
            }
        } else if (choice == 7) {
            printf("Enter number: ");
            scanf("%d", &a);
            int rev = reverseNumber(a);
            printf("Reversed: %d (palindrome: %s)\n",
                   rev, rev == a ? "YES" : "NO");
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
