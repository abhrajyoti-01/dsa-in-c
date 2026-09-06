/*
Bit Manipulation Toolkit
------------------------
What this program solves:
Demonstrates the classic bitwise operations and tricks on a
user-entered number: binary display, set/clear/toggle/check bit,
count set bits, check power of two, reverse bits, swap two values
with XOR, and get/set/clear ranges.

Real life example:
Embedded systems and drivers: a single hardware register packs 32
flags; device drivers test and flip individual bits. Networks use
masks (IP & netmask); games pack entity states into bitfields.

DIAGRAM (n = 12 = 1100, bit i = 1, mask = 0010):
  set      1100 | 0010 -> 1110 (14)
  clear    1100 & ~0010 -> 1100 (unchanged, bit was 0)
  toggle   1100 ^ 0010 -> 1110 (14)
  check    (1100 >> 1) & 1 -> 0
  Kernighan trick: 1100 & 1011 -> 1000 (lowest set bit cleared)

How the process works:
1. Binary display: print bits from the highest to lowest using
   (n >> i) & 1.
2. SET bit i: n | (1 << i). CLEAR: n & ~(1 << i).
   TOGGLE: n ^ (1 << i). CHECK: (n >> i) & 1.
3. Count set bits (Kernighan): repeatedly n &= n-1 clears the
   lowest set bit - loops only that many times.
4. Power of two: n > 0 and (n & (n-1)) == 0.
5. XOR swap: a ^= b; b ^= a; a ^= b - no temporary needed.

Pseudo code:
START
READ n
PRINT binary: FOR i = high..0: PRINT (n >> i) & 1
MENU: set/check/toggle/count/reverse/xor-swap...
END

Sample input and dry run:
n = 13 (1101)
check bit 2 -> 1; toggle bit 1 -> 15; count set bits = 3;
power of two? no (3 set bits); n & (n-1) = 12.

Main logic to understand:
Everything is shifts and masks: O(1) ops that beat arithmetic.
Kernighan's trick n &= (n-1) isolates the lowest set bit - the
trick behind the Fenwick tree too. Bit tricks are constant-time
and cache-friendly.
*/

#include <stdio.h>

/*
 * printBinary - Shows all bits of an unsigned int.
 */
void printBinary(unsigned int n) {
    printf("Binary of %u: ", n);
    int started = 0;
    for (int i = 31; i >= 0; i--) {
        int bit = (int)((n >> i) & 1u);
        if (bit) {
            started = 1;
        }
        if (started || i == 0) {
            printf("%d", bit);
        }
    }
    printf("\n");
}

/*
 * countSetBits - Kernighan's trick: clear lowest set bit each loop.
 */
int countSetBits(unsigned int n) {
    int count = 0;
    while (n) {
        n = n & (n - 1u);
        count = count + 1;
    }
    return count;
}

/*
 * reverseBits - Mirrors all 32 bits.
 */
unsigned int reverseBits(unsigned int n) {
    unsigned int result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1u);
        n = n >> 1;
    }
    return result;
}

int main() {
    unsigned int n;
    int choice;
    int pos;

    printf("Bit Manipulation Toolkit\n\n");

    printf("Enter a non-negative integer: ");
    scanf("%u", &n);

    while (1) {
        printf("\nCurrent number: %u\n", n);
        printf("1. Show binary\n");
        printf("2. Set bit i\n");
        printf("3. Clear bit i\n");
        printf("4. Toggle bit i\n");
        printf("5. Check bit i\n");
        printf("6. Count set bits\n");
        printf("7. Power of two?\n");
        printf("8. Reverse all bits\n");
        printf("9. Multiply by 2 / divide by 2 (shifts)\n");
        printf("10. Show XOR trick (n ^ n-1)\n");
        printf("11. Enter a new number\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printBinary(n);
        } else if (choice == 2) {
            printf("Bit position (0-31): ");
            scanf("%d", &choice);
            n = n | (1u << choice);
            printf("Set bit %d -> %u\n", choice, n);
        } else if (choice == 3) {
            printf("Bit position (0-31): ");
            scanf("%d", &choice);
            n = n & ~(1u << choice);
            printf("Cleared bit %d -> %u\n", choice, n);
        } else if (choice == 4) {
            printf("Bit position (0-31): ");
            scanf("%d", &choice);
            n = n ^ (1u << choice);
            printf("Toggled bit %d -> %u\n", choice, n);
        } else if (choice == 5) {
            printf("Bit position (0-31): ");
            scanf("%d", &choice);
            printf("Bit %d = %d\n", choice, (int)((n >> choice) & 1u));
        } else if (choice == 6) {
            printf("Set bits: %d\n", countSetBits(n));
        } else if (choice == 7) {
            printf("%u is a power of two: %s\n",
                   n, (n > 0 && (n & (n - 1)) == 0) ? "YES" : "NO");
        } else if (choice == 8) {
            n = reverseBits(n);
            printf("Reversed bits -> %u\n", n);
        } else if (choice == 9) {
            printf("n << 1 = %u (times 2)\n", n << 1);
            printf("n >> 1 = %u (divided by 2)\n", n >> 1);
        } else if (choice == 10) {
            printf("n & (n-1) clears the lowest set bit: %u & %u = %u\n",
                   n, n - 1, n & (n - 1));
        } else if (choice == 11) {
            printf("Enter a new non-negative integer: ");
            scanf("%u", &n);
        } else if (choice == 12) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
