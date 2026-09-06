/*
Recursion Ladder (Basic -> Hard)
--------------------------------
Real life example:
Nested folders: to list a drive you recurse into each subfolder -
one-call recursion. Tree-shaped decisions (like flood fill) use
two-call recursion. Both patterns appear in every file explorer.

DIAGRAM (one-call vs two-call recursion for fib(4)):
  one call: fact(4) -> fact(3) -> ... a straight chain
  two calls:              fib(4)
                          /    \
                     fib(3)    fib(2)
                     /    \     /   \
                 fib(2) fib(1) fib(1) fib(0)
  the call TREE branches; leaves are the base cases.

LADDER (easy -> hard inside this file):
  1. Sum of 1..n             (one branch)
  2. Factorial               (seen in factorial.c - here quick)
  3. a^b (power, halving)    (divide & conquer seed)
  4. Reverse a string        (two-index recursion)
  5. Palindrome check        (compare ends inward)
  6. Fibonacci               (TWO branches - exponential tree)
  7. Digital root            (recursion on results)
  8. Tower of Hanoi lite     (3 moves for n=2 traced)

BEGINNER EXPLANATION:
Each rung adds ONE new recursion idea:
- 1-2: a single self-call per level (a straight line).
- 3: HALVING - the call chain gets shorter, not n-1 shorter. This
  is the seed of fast_power and divide & conquer.
- 4-5: recursion on TWO indices moving toward each other.
- 6: TWO self-calls per level = a tree of calls = exponential
  without memoization (why folder 11 exists).
- 7: recursion on the RESULT of a function call.

SUPER SIMPLE EXAMPLE (sum(4)):
sum(4) = 4 + sum(3)
       = 4 + 3 + sum(2)
       = 4 + 3 + 2 + sum(1)
       = 4 + 3 + 2 + 1 = 10
One branch, base case at 1, answer assembled on the way back.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- sum(0) = 0, not an error: empty sum convention.
- Palindrome with ONE character: trivially true (lo meets hi).
- Palindrome with TWO equal middle chars: "abba" - lo and hi pass
  each other mid-check; the loop/recursion naturally stops.
- fib(50) naive: 2^50 calls - the program caps n so the demo
  stays instant; memoization lives in 11-dynamic-programming.
- Digital root: dr(0)=0; for n>0 it is 1 + (n-1) % 9 - the closed
  form avoids recursion entirely (a fun verification).
- Power 0: a^0 = 1 for any a (including 0^0 = 1 by convention
  here). Negative exponent: rejected - fractions need a different
  representation.

DRY RUN (reverse "abc" by recursion):
rev(lo=0, hi=2): swap a,c -> "cba"; recurse (1,1) -> base
rev returns; string already reversed in place.
Call depth = length/2 - shallow and safe.

TIPS:
- Ask of every recursion: "what SHRINKS and what is the BASE?"
  If either answer is unclear, the code will stack-overflow.
- One-call recursion = a loop in disguise (rewrite iteratively).
  Two-call recursion = tree traversal - the real power case.
- Digital root appears in checksum designs (mod-9 casting out
  nines) - small pattern, big idea.
*/

#include <stdio.h>
#include <string.h>

int sumToN(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sumToN(n - 1);
}

long long powerHalf(long long a, int b) {
    if (b == 0) {
        return 1;
    }
    long long half = powerHalf(a, b / 2);
    if (b % 2 == 0) {
        return half * half;
    }
    return half * half * a;
}

void reverseString(char s[], int lo, int hi) {
    if (lo >= hi) {
        return;
    }
    char t = s[lo];
    s[lo] = s[hi];
    s[hi] = t;
    reverseString(s, lo + 1, hi - 1);
}

int isPalindromeRec(const char s[], int lo, int hi) {
    if (lo >= hi) {
        return 1;
    }
    if (s[lo] != s[hi]) {
        return 0;
    }
    return isPalindromeRec(s, lo + 1, hi - 1);
}

long long fibNaive(int n) {
    if (n <= 1) {
        return n;
    }
    return fibNaive(n - 1) + fibNaive(n - 2);
}

int digitalRoot(int n) {
    if (n < 10) {
        return n;
    }
    int sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return digitalRoot(sum);
}

int hanoiMoves(int n) {
    if (n == 1) {
        return 1;
    }
    return 2 * hanoiMoves(n - 1) + 1;
}

int main() {
    int n;
    int choice;
    char str[100];

    printf("Recursion Ladder (easy -> hard)\n\n");

    while (1) {
        printf("\n1. Sum of 1..n\n");
        printf("2. a^b by halving\n");
        printf("3. Reverse a string\n");
        printf("4. Palindrome check (recursive)\n");
        printf("5. Fibonacci naive (show exponential cost)\n");
        printf("6. Digital root\n");
        printf("7. Hanoi move count\n");
        printf("8. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("n: ");
            scanf("%d", &n);
            if (n < 0 || n > 100000) {
                printf("Keep n in 0..100000 (stack!)\n");
            } else {
                printf("sum(1..%d) = %d\n", n, sumToN(n));
            }
        } else if (choice == 2) {
            printf("base and exponent: ");
            scanf("%d %d", &n, &choice);
            if (choice < 0) {
                printf("Exponent must be >= 0\n");
            } else {
                printf("%d^%d = %lld\n", n, choice,
                       powerHalf(n, choice));
            }
        } else if (choice == 3) {
            printf("Enter string: ");
            scanf("%99s", str);
            reverseString(str, 0, (int)strlen(str) - 1);
            printf("Reversed: %s\n", str);
        } else if (choice == 4) {
            printf("Enter string: ");
            scanf("%99s", str);
            printf("\"%s\" palindrome: %s\n", str,
                   isPalindromeRec(str, 0, (int)strlen(str) - 1)
                       ? "YES" : "NO");
        } else if (choice == 5) {
            printf("n (0-25): ");
            scanf("%d", &n);
            if (n < 0 || n > 25) {
                printf("Naive fib is exponential - keep n <= 25\n");
            } else {
                printf("fib(%d) = %lld (calls explode as n grows!)\n",
                       n, fibNaive(n));
            }
        } else if (choice == 6) {
            printf("n: ");
            scanf("%d", &n);
            printf("Digital root of %d = %d\n", n, digitalRoot(n));
        } else if (choice == 7) {
            printf("disks (1-30): ");
            scanf("%d", &n);
            printf("Minimum moves = %d (formula 2^n - 1: %d)\n",
                   hanoiMoves(n), (1 << n) - 1);
        } else if (choice == 8) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
