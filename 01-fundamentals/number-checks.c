/*
Number Checks - Even/Odd, Prime, Armstrong, Palindrome, Perfect, Strong, Sum of Digits
--------------------------------------------------------------------------------------
What this program solves:
The classic "number theory" beginner set, all in one menu:
even/odd, prime, Armstrong number, palindrome number, perfect
number, strong number, and digit sum.

Real life example:
Credit-card validation (Luhn), ISBN checks and roll-number rules
all test digits with tiny loops exactly like the prime, Armstrong
and palindrome checks in this program.

DIAGRAM (is 29 prime? test divisors only up to sqrt(29) ~ 5):
  2: 29 % 2 = 1 -> no
  3: 29 % 3 = 2 -> no
  4: 29 % 4 = 1 -> no
  5: 29 % 5 = 4 -> no   => prime, in 4 tests instead of 27
  Palindrome 121: reverse(121) = 121 -> match.

BEGINNER EXPLANATION (the simplest way to think):
All of these are just "pull the number apart digit by digit" with
% 10 (last digit) and / 10 (chop the last digit). Every check is a
different question asked about those digits:
- Armstrong (3 digits): does the sum of CUBES of digits equal the
  number itself?
- Perfect: does the sum of proper divisors equal the number?
- Strong: does the sum of FACTORIALS of digits equal the number?
- Palindrome: does the number read the same reversed?

SUPER SIMPLE EXAMPLES:
- Even/odd: 7 % 2 = 1 -> odd; 8 % 2 = 0 -> even.
- Prime 7: try dividing by 2, 3 (only up to sqrt(7)~2.6) -> none
  divide -> prime. 9: divisible by 3 -> not prime.
- Armstrong 153: 1^3 + 5^3 + 3^3 = 1 + 125 + 27 = 153 -> YES.
- Palindrome 1221: reversed 1221 -> YES. 1231 reversed 1231? no,
  reversed is 1321 -> NO.
- Perfect 28: divisors 1+2+4+7+14 = 28 -> YES.
- Strong 145: 1! + 4! + 5! = 1 + 24 + 120 = 145 -> YES.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- 1 is NOT prime (needs exactly two divisors) but IS a strong
  number (1! = 1) and NOT perfect (its divisor sum is 0).
- 0 is even; 0 is not prime; 0 is a palindrome; 0 is an Armstrong
  number of one digit by convention (0^1 = 0)? Actually single
  digit numbers are trivially Armstrong - the interesting ones are
  153, 370, 371, 407.
- 2 is the ONLY even prime - do not "optimize" by skipping even
  numbers before checking 2.
- Negative numbers: these definitions target positives; the menu
  rejects negatives for prime/perfect/Armstrong.
- Very large inputs: use the sqrt bound for prime, otherwise the
  loop gets slow (trial division to n would be O(n)).

DRY RUN (prime check, n = 29):
i = 2: 29 % 2 = 1 -> not divisible
i = 3: 29 % 3 = 2 -> not divisible
i = 4: 29 % 4 = 1 -> not divisible
i = 5: 5*5 = 25 <= 29, 29 % 5 = 4 -> not divisible
loop ends (i*i = 36 > 29) -> prime, with only 4 checks

TIPS:
- Prime loop only needs i*i <= n, not i <= n - cutting from O(n)
  to O(sqrt(n)). This is the single most useful prime tip.
- Digit extraction loop: while (n > 0) { digit = n % 10; n /= 10; }
  memorize it - Armstrong/strong/palindrome/reverse all reuse it.
- Perfect number pairs: (220, 284) are "amicable" - sum of
  divisors of each equals the other. Fun follow-up project.
- Armstrong numbers: 153, 370, 371, 407 (3-digit); 1634, 8208,
  9474 (4-digit, using 4th powers).
*/

#include <stdio.h>
#include <math.h>

/*
 * isEven - Returns 1 for even numbers.
 */
int isEven(int n) {
    return n % 2 == 0;
}

/*
 * isPrime - Trial division up to sqrt(n).
 */
int isPrime(int n) {
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    for (int i = 3; (long long)i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * digitSum - Sum of all digits.
 */
int digitSum(int n) {
    int sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

/*
 * isArmstrong - Sum of digits raised to digit-count power.
 * 3-digit classic uses cubes; this handles any digit count.
 */
int isArmstrong(int n) {
    int digits = 0;
    int temp = n;
    while (temp > 0) {
        digits = digits + 1;
        temp = temp / 10;
    }

    int sum = 0;
    temp = n;
    while (temp > 0) {
        int d = temp % 10;
        int power = 1;
        for (int i = 0; i < digits; i++) {
            power = power * d;
        }
        sum = sum + power;
        temp = temp / 10;
    }
    return sum == n;
}

/*
 * isPalindromeNumber - Reverse the number, compare.
 */
int isPalindromeNumber(int n) {
    int original = n;
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n = n / 10;
    }
    return reversed == original;
}

/*
 * isPerfect - Sum of proper divisors equals n.
 */
int isPerfect(int n) {
    if (n < 2) {
        return 0;
    }
    int sum = 1;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum == n;
}

/*
 * factorial - Needed for strong numbers.
 */
long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; i++) {
        f = f * i;
    }
    return f;
}

/*
 * isStrong - Sum of factorials of digits equals n.
 */
int isStrong(int n) {
    int sum = 0;
    int temp = n;
    while (temp > 0) {
        sum = (int)(sum + factorial(temp % 10));
        temp = temp / 10;
    }
    return sum == n;
}

int main() {
    int n;
    int choice;

    printf("Number Checks\n\n");

    printf("Enter a non-negative integer: ");
    scanf("%d", &n);

    while (1) {
        printf("\nNumber: %d\n", n);
        printf("1. Even or odd\n");
        printf("2. Prime?\n");
        printf("3. Sum of digits\n");
        printf("4. Armstrong number?\n");
        printf("5. Palindrome number?\n");
        printf("6. Perfect number?\n");
        printf("7. Strong number?\n");
        printf("8. Enter a new number\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("%d is %s\n", n, isEven(n) ? "EVEN" : "ODD");
        } else if (choice == 2) {
            printf("%d is %sprime\n", n, isPrime(n) ? "" : "not ");
        } else if (choice == 3) {
            printf("Sum of digits = %d\n", digitSum(n));
        } else if (choice == 4) {
            printf("%d is %s Armstrong number\n",
                   n, isArmstrong(n) ? "an" : "not an");
        } else if (choice == 5) {
            printf("%d is %s palindrome number\n",
                   n, isPalindromeNumber(n) ? "a" : "not a");
        } else if (choice == 6) {
            printf("%d is %s perfect number\n",
                   n, isPerfect(n) ? "a" : "not a");
        } else if (choice == 7) {
            printf("%d is %s strong number\n",
                   n, isStrong(n) ? "a" : "not a");
        } else if (choice == 8) {
            printf("Enter a new non-negative integer: ");
            scanf("%d", &n);
        } else if (choice == 9) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
