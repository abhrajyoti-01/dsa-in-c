/*
Backtracking Interview Problems - Permutations, Combination Sum, Letter
Combinations, Generate Parentheses (Intermediate -> Advanced)
--------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 46  - Permutations
  LeetCode 39  - Combination Sum (reuse allowed)
  LeetCode 17  - Letter Combinations of a Phone Number
  LeetCode 22  - Generate Parentheses (pruned DFS)
  LeetCode 78  - Subsets (full ladder: 13-backtracking/subsets-combinations-word-search.c)
  HackerRank "N-Queens" family recursion drills

PROBLEM DEFINITION:
Four enumeration drills that share the choose - explore - unchoose
skeleton, differing only in the pruning rule and the leaf definition.
Counting the leaves also proves the closed-form totals (n!, 2^n,
Catalan(n)).

REAL-LIFE EXAMPLE:
Generating all task orderings for a scheduler, all valid bracket
sequences for a parser test suite, and all phone-keypad words for a
dialler are these four searches.

DIAGRAM (generate parentheses, n = 3):
                     ""
              (                <- never start with ')'
            ((
        (((      (()
      (()) only valid leaves: ((())), (()()), (())(), ()(()),
            (())(                      ()()()      -> 5 = Catalan(3)
            ()()

PSEUDO CODE:
  Permutations(a, index):
      if index == n: emit a
      for i = index..n-1: swap(a[index], a[i]); recurse(index+1);
                          swap back
  CombinationSum(candidates, start, remaining, chosen):
      if remaining == 0: emit chosen; return
      for i = start..n-1:
          if candidates[i] > remaining: break   // sorted -> prune
          choose i; recurse(i, remaining - candidate); unchoose
  LetterCombinations(digits, position, prefix): one digit -> 3 or 4
      letters; recurse per letter
  GenerateParentheses(open, close, prefix):
      if prefix is full: emit
      if open < n: recurse with '('
      if close < open: recurse with ')'

SAMPLE INPUT & DRY RUN:
  3
  1 2 3
  3
  2 3 5
  7
  23
  3
  Permutations of 1 2 3 -> 6 (3! = 6).
  Combination sum 7 from {2,3,5} -> 2+2+3 and 2+5 -> 2 answers.
  "23" -> ad ae af bd be bf cd ce cf -> 9.
  Parentheses n=3 -> 5 sequences (diagram).

SPECIAL CASES:
  - Duplicate candidates: this program requires DISTINCT values for
    permutations; combination sum allows reuse of one value but the
    sorted order plus the break keeps the output duplicate-free for
    distinct candidates.
  - Combination sum with an unreachable target: zero answers, printed
    explicitly.
  - Letter combinations: '0' and '1' have no letters -> rejected rather
    than silently dropped. Digits are 2..9.
  - n = 0 permutations: exactly one leaf (the empty permutation).
  - Catalan growth is explosive: n <= 8 parentheses (1430 sequences)
    keeps the console readable; the count is printed either way.
  - candidates <= 20, target <= 40, digits <= 10.

COMPLEXITY:
  O(n * n!) permutations, O(2^target) style combination trees,
  O(4^digits) letter combinations, Catalan(n) parentheses; recursion
  depth is the input size, so each is O(n) or O(target) stack.

TIPS:
  Prune as EARLY as possible: sorting first turns "skip the remainder"
  into a simple break. Always undo the choice before the next iteration -
  the unchoose step is where most interview bugs live.
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 20
#define MAX_DIGITS 10

int values[MAX_N];
int chosen[MAX_N];
long long answerCount;

const char* keypad[10] = {
    "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
};

void printValues(const int a[], int n) {
    printf("  [");
    for (int i = 0; i < n; i++) {
        printf("%s%d", i ? ", " : "", a[i]);
    }
    printf("]\n");
}

void permutations(int a[], int n, int index) {
    if (index == n) {
        answerCount++;
        if (answerCount <= 20) printValues(a, n);
        return;
    }
    for (int i = index; i < n; i++) {
        int temp = a[index];
        a[index] = a[i];
        a[i] = temp;
        permutations(a, n, index + 1);
        temp = a[index];
        a[index] = a[i];
        a[i] = temp;
    }
}

void combinationSum(const int candidates[], int n, int start, int remaining,
                    int depth) {
    if (remaining == 0) {
        answerCount++;
        if (answerCount <= 20) printValues(chosen, depth);
        return;
    }
    for (int i = start; i < n; i++) {
        if (candidates[i] > remaining) break;   /* sorted, so stop */
        chosen[depth] = candidates[i];
        combinationSum(candidates, n, i, remaining - candidates[i],
                       depth + 1);
    }
}

void letterCombinations(const char* digits, int length, int position,
                        char* prefix) {
    if (position == length) {
        answerCount++;
        if (answerCount <= 20) printf("  %s\n", prefix);
        return;
    }
    const char* letters = keypad[digits[position] - '0'];
    for (int i = 0; letters[i] != '\0'; i++) {
        prefix[position] = letters[i];
        letterCombinations(digits, length, position + 1, prefix);
    }
    prefix[position] = '\0';
}

void generateParentheses(int n, int open, int close, char* buffer,
                         int position) {
    if (position == 2 * n) {
        buffer[position] = '\0';
        answerCount++;
        if (answerCount <= 20) printf("  %s\n", buffer);
        return;
    }
    if (open < n) {
        buffer[position] = '(';
        generateParentheses(n, open + 1, close, buffer, position + 1);
    }
    if (close < open) {
        buffer[position] = ')';
        generateParentheses(n, open, close + 1, buffer, position + 1);
    }
}

int main(void) {
    int n;

    printf("Backtracking Interview Problems\n\n");
    printf("== Permutations. How many DISTINCT integers (0-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 0 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &values[i]) != 1) {
            printf("Invalid value\n");
            return 1;
        }
        for (int j = 0; j < i; j++) {
            if (values[j] == values[i]) {
                printf("Invalid input: values must be distinct\n");
                return 1;
            }
        }
    }
    answerCount = 0;
    permutations(values, n, 0);
    printf("Total permutations = %lld\n", answerCount);

    printf("\n== Combination sum. How many candidates (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d POSITIVE candidates (1..1000), duplicates allowed:\n",
           n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &values[i]) != 1 || values[i] < 1 ||
            values[i] > 1000) {
            printf("Invalid candidate\n");
            return 1;
        }
    }
    for (int i = 1; i < n; i++) {
        int key = values[i];
        int j = i - 1;
        while (j >= 0 && values[j] > key) {
            values[j + 1] = values[j];
            j--;
        }
        values[j + 1] = key;
    }
    int target;
    printf("Target sum (1..1000): ");
    if (scanf("%d", &target) != 1 || target < 1 || target > 1000) {
        printf("Invalid target\n");
        return 1;
    }
    answerCount = 0;
    combinationSum(values, n, 0, target, 0);
    printf("Total combinations = %lld\n", answerCount);

    printf("\n== Letter combinations. Enter 1..%d digits from 2..9: ",
           MAX_DIGITS);
    char digits[MAX_DIGITS + 1];
    char prefix[MAX_DIGITS + 1];
    if (scanf("%10s", digits) != 1) {
        printf("Invalid digits\n");
        return 1;
    }
    n = (int)strlen(digits);
    if (n < 1 || n > MAX_DIGITS) {
        printf("Invalid digit count\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (digits[i] < '2' || digits[i] > '9') {
            printf("Invalid digit %c: only 2..9 map to letters\n",
                   digits[i]);
            return 1;
        }
    }
    answerCount = 0;
    prefix[0] = '\0';
    letterCombinations(digits, n, 0, prefix);
    printf("Total letter combinations = %lld\n", answerCount);

    printf("\n== Generate parentheses. How many pairs (0-8)? ");
    if (scanf("%d", &n) != 1 || n < 0 || n > 8) {
        printf("Invalid pair count\n");
        return 1;
    }
    char buffer[2 * 8 + 1];
    answerCount = 0;
    generateParentheses(n, 0, 0, buffer, 0);
    if (n == 0) printf("  (the empty sequence)\n");
    printf("Total valid sequences = %lld\n", answerCount);
    return 0;
}
