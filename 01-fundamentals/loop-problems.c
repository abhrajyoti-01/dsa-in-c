/*
Loop Problems - summation, factorial, table, Fibonacci, digits, pattern
----------------------------------------------------------------------
What this program solves:
A small menu of common beginner problems that use loops:
1. Sum integers from 1 to n.
2. Find n! (factorial).
3. Print a multiplication table.
4. Print the first n Fibonacci numbers.
5. Count digits, sum digits, and reverse a non-negative integer.
6. Print a right-triangle star pattern using nested loops.

LOOP IDEAS USED:
- Accumulator: update a running result on every pass (sum/factorial).
- State update: carry the previous values forward (Fibonacci).
- Digit processing: take the last digit with % 10, then remove it
  with / 10.
- Nested loops: the inner loop prints one row of the pattern.

Input limits keep the results representable:
- Summation: 0 through 1,000,000 (keeps the demonstration responsive).
- Factorial: 0 through 20 (0! is 1).
- Fibonacci: 1 through 93 terms (F0 through F92 fit unsigned long long).
- Digit and pattern inputs: non-negative number / 1 through 20 rows.
*/

#include <stdio.h>

void printMenu() {
    printf("\nLoop Problems\n");
    printf("1. Sum from 1 to n\n");
    printf("2. Factorial (n!)\n");
    printf("3. Multiplication table\n");
    printf("4. First n Fibonacci numbers\n");
    printf("5. Count, sum, and reverse digits\n");
    printf("6. Right-triangle star pattern\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;

    printf("Loop Problems\n");

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input; exiting.\n");
            break;
        }

        if (choice == 1) {
            int n;
            printf("Enter n (0-1000000): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (n < 0 || n > 1000000) {
                printf("n must be between 0 and 1000000.\\n");
                continue;
            }

            long long sum = 0;
            for (int i = 1; i <= n; i++) {
                sum += i;
            }
            printf("Sum from 1 to %d = %lld\n", n, sum);
        } else if (choice == 2) {
            int n;
            printf("Enter n (0-20): ");
            if (scanf("%d", &n) != 1) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (n < 0 || n > 20) {
                printf("n must be between 0 and 20.\n");
                continue;
            }

            unsigned long long factorial = 1;
            for (int i = 2; i <= n; i++) {
                factorial *= (unsigned long long)i;
            }
            printf("%d! = %llu\n", n, factorial);
        } else if (choice == 3) {
            int number;
            int limit;
            printf("Enter a number and table limit (1-20): ");
            if (scanf("%d %d", &number, &limit) != 2) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (limit < 1 || limit > 20) {
                printf("The table limit must be between 1 and 20.\n");
                continue;
            }

            for (int i = 1; i <= limit; i++) {
                printf("%d x %d = %lld\n", number, i,
                       (long long)number * i);
            }
        } else if (choice == 4) {
            int terms;
            printf("How many terms (1-93)? ");
            if (scanf("%d", &terms) != 1) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (terms < 1 || terms > 93) {
                printf("Term count must be between 1 and 93.\n");
                continue;
            }

            unsigned long long first = 0;
            unsigned long long second = 1;
            printf("Fibonacci: ");
            for (int i = 0; i < terms; i++) {
                printf("%llu", first);
                if (i + 1 < terms) {
                    printf(" ");
                }
                if (i + 1 < terms) {
                    unsigned long long next = first + second;
                    first = second;
                    second = next;
                }
            }
            printf("\n");
        } else if (choice == 5) {
            long long number;
            printf("Enter a non-negative integer: ");
            if (scanf("%lld", &number) != 1) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (number < 0 || number > 999999999999999999LL) {
                printf("Enter a number from 0 through 999999999999999999.\\n");
                continue;
            }

            long long remaining = number;
            long long reversed = 0;
            int digitCount = 0;
            int digitSum = 0;
            do {
                int digit = (int)(remaining % 10);
                digitSum += digit;
                digitCount++;
                reversed = reversed * 10 + digit;
                remaining /= 10;
            } while (remaining > 0);

            printf("Digit count = %d\n", digitCount);
            printf("Digit sum = %d\n", digitSum);
            printf("Reversed = %lld\n", reversed);
        } else if (choice == 6) {
            int rows;
            printf("Enter number of rows (1-20): ");
            if (scanf("%d", &rows) != 1) {
                printf("Invalid input; exiting.\n");
                break;
            }
            if (rows < 1 || rows > 20) {
                printf("Rows must be between 1 and 20.\n");
                continue;
            }

            for (int row = 1; row <= rows; row++) {
                for (int column = 1; column <= row; column++) {
                    printf("* ");
                }
                printf("\n");
            }
        } else if (choice == 7) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Choose 1-7.\n");
        }
    }

    return 0;
}
