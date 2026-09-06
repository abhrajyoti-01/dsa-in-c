/*
String Permutations (Recursion Master Problem)
----------------------------------------------
What this program solves:
Prints ALL permutations of a string's characters - "abc" gives
abc, acb, bac, bca, cab, cba. THE canonical recursion-tree problem
and the gateway to backtracking (folder 13).

Real life example:
Anagram generators, test-case enumerators and puzzle solvers all
enumerate orderings with swap-permute-swap-back backtracking.

DIAGRAM (permutations of "abc" - fix one slot, recurse):
  fix a -> permute bc -> abc, acb
  fix b -> permute ac -> bac, bca
  fix c -> permute ab -> cab, cba
  swap into place, recurse, swap BACK (undo) before the next choice.

BEGINNER EXPLANATION:
A permutation of "abc": pick WHICH character goes first (3 ways),
then permute the remaining two. Formally: for each position i,
swap s[0] with s[i], permute s[1..end], then swap BACK (undo).
The undo is what makes it backtracking - the string must be
restored between sibling choices.

SUPER SIMPLE EXAMPLE ("ab"):
fix a: permute "b" -> ab
fix b: swap a,b -> "ba", permute "a" -> ba
Permutations: ab, ba. Count = 2! = 2.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- DUPLICATE characters ("aab"): this simple swap method prints
  duplicates (6 raw orderings, 3 distinct). Deduping needs either
  a sorted-then-skip-same-char rule or a set - noted in tips.
- Single character: one permutation, itself.
- Count sanity: n distinct chars -> n! permutations; "abc" gives
  6. If you see more/fewer, check the swap-undo pairing.
- Recursion depth = n (fine for lab sizes; n > 10 output explodes
  as n! - 10! = 3.6 million lines).
- The swap-back is NOT optional: without undo, later branches see
  a mutated string and the output order/content corrupts.

DRY RUN ("abc", level 0):
swap(0,0): a?? -> permute level1:
  swap(1,1): abc -> permute level2: print abc
  swap(1,2): acb -> print acb; swap back -> abc
swap(0,1): bac -> level1: print bac, bca; swap back
swap(0,2): cba -> level1: print cba, cab; swap back
Output: abc acb bac bca cba cab - all 6, lexicographic here by luck.

TIPS:
- This is backtracking's hello-world: choose -> explore -> UNDO.
  Every folder-13 program (N-Queens, Sudoku) repeats this shape.
- Handling duplicates: sort first, and at each level skip
  s[j] == s[i] where j was already used at this position - try
  implementing after this program.
- Permutations relate to next_permutation (the dictionary-order
  generator): find the rightmost ascent, swap with the smallest
  larger suffix element, reverse the suffix - O(n) per step.
*/

#include <stdio.h>
#include <string.h>

long long printed = 0;

/*
 * permute - Swap-based recursion; depth = fixed prefix length.
 */
void permute(char s[], int depth, int n) {
    if (depth == n) {
        printf("%s\n", s);
        printed = printed + 1;
        return;
    }
    for (int i = depth; i < n; i++) {
        char t = s[depth];
        s[depth] = s[i];
        s[i] = t;

        permute(s, depth + 1, n);

        t = s[depth];
        s[depth] = s[i];
        s[i] = t;
    }
}

int main() {
    char str[12];

    printf("String Permutations (backtracking hello-world)\n\n");

    printf("Enter a string (max 10 chars): ");
    scanf("%10s", str);

    int n = (int)strlen(str);
    if (n > 10) {
        printf("10! = 3.6 million lines - keep it small\n");
        return 1;
    }

    printf("\nAll %d! expected = %lld permutations:\n", n,
           n <= 12 ? (1LL << 40) : 0);
    permute(str, 0, n);

    printf("\nTotal printed: %lld (duplicates appear if chars repeat)\n",
           printed);

    return 0;
}
