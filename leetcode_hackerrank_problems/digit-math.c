/*
Digit Math - Digits, Sum of Digits, Palindromes, Armstrong (Basic)
------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 9   - Palindrome Number
  LeetCode 258 - Add Digits (digital root)
  LeetCode 7   - Reverse Integer (overflow-aware variant)
  HackerRank "Recursive Digit Sum"
  Classic drills: sum of digits, count of digits, Armstrong numbers

PROBLEM DEFINITION:
Peel decimal digits one at a time and use them to answer four
questions about a number: how many digits, their sum, its digital
root, whether it reads the same backwards, and whether it is an
Armstrong (narcissistic) number.

REAL-LIFE EXAMPLE:
Checksum digits on invoices and ID numbers: a single digit derived
from all the others catches keystroke errors. The digital root is the
simplest such checksum.

DIAGRAM (1729):
  1729 / 10 = 172 rem 9   -> digit 9
   172 / 10 =  17 rem 2   -> digit 2
    17 / 10 =   1 rem 7   -> digit 7
     1 / 10 =   0 rem 1   -> digit 1
  digits = 4, sum = 19, digital root = 1 (1+9) = 1
  reversed = 9271 != 1729 -> not a palindrome number

PSEUDO CODE:
  Reverse(n): rev = 0
      while n > 0: rev = rev*10 + n%10; n /= 10
  SumOfDigits(n): total = 0
      while n > 0: total += n%10; n /= 10
  DigitalRoot(n): while n >= 10: n = SumOfDigits(n)
  IsArmstrong(n): total = sum of digit^digitCount; total == n

SAMPLE INPUT & DRY RUN:
  3
  1729
  1634
  9474
  1729 -> 4 digits, sum 19, root 1, not a palindrome, not Armstrong.
  1634 -> 1^4+6^4+3^4+4^4 = 1+1296+81+256 = 1634 -> Armstrong.
  9474 -> 9^4+4^4+7^4+4^4 = 6561+256+2401+256 = 9474 -> Armstrong.

SPECIAL CASES:
  - Only n >= 0 is accepted; negatives are rejected instead of guessed.
  - n = 0 has one digit, sum 0, root 0, and IS a palindrome.
  - Numbers ending in 0 lose that digit when reversed (120 -> 21):
    normal for integer reversal, so 120 is not a palindrome number.
  - Digital root shortcut: n % 9, with 9 instead of 0 for multiples.
  - Reverse Integer compares against the signed 32-bit range.
  - value <= 10^9, count <= 1000, so long long holds every product.

COMPLEXITY:
  O(digits) per number, i.e. O(log n); O(1) extra space.
  The digital-root shortcut is O(1) - use it in contests.

TIPS:
  `while (n > 0) { digit = n % 10; n /= 10; }` is THE digit loop;
  keep it as muscle memory. Weighted digit problems (Luhn, ISBN) add
  a position counter inside the same loop.
*/

#include <stdio.h>

#define MAX_NUMBERS 1000
#define VALUE_LIMIT 1000000000LL

long long reverseNumber(long long n) {
    long long reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return reversed;
}

int countDigits(long long n) {
    int count = 0;
    do {
        count++;
        n /= 10;
    } while (n > 0);
    return count;
}

long long sumOfDigits(long long n) {
    long long total = 0;
    while (n > 0) {
        total += n % 10;
        n /= 10;
    }
    return total;
}

long long digitalRoot(long long n) {
    while (n >= 10) n = sumOfDigits(n);
    return n;
}

int isPalindromeNumber(long long n) {
    return n == reverseNumber(n);
}

int isArmstrong(long long n) {
    int digits = countDigits(n);
    long long total = 0;
    long long rest = n;
    while (rest > 0) {
        long long power = 1;
        long long digit = rest % 10;
        for (int i = 0; i < digits; i++) power *= digit;
        total += power;
        rest /= 10;
    }
    return total == n;
}

int main(void) {
    int n;

    printf("Digit Math - Digits, Digital Root, Palindrome, Armstrong\n\n");
    printf("How many numbers (1-%d)? ", MAX_NUMBERS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NUMBERS) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d non-negative integers (0..10^9):\n", n);
    for (int i = 0; i < n; i++) {
        long long value;
        if (scanf("%lld", &value) != 1 || value < 0 || value > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        long long reversed = reverseNumber(value);
        printf("\n%d: digits = %d, digit sum = %lld, digital root = %lld\n",
               (int)value, countDigits(value), sumOfDigits(value),
               digitalRoot(value));
        printf("   reversed = %lld, palindrome number = %s, Armstrong = %s\n",
               reversed, isPalindromeNumber(value) ? "yes" : "no",
               isArmstrong(value) ? "yes" : "no");
    }
    return 0;
}
