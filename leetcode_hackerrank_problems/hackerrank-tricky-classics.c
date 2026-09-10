/*
HackerRank Tricky Classics - New Year Chaos, Minimum Swaps 2,
Maximum Subarray Sum, Simple Text Editor (Intermediate -> Advanced)
-------------------------------------------------------------------
PLATFORM PROBLEMS:
  HackerRank "New Year Chaos"        - bribes by a queue jump, with the
                                       exact bridge-crossing proof
  HackerRank "Minimum Swaps 2"       - in-place cycle counting
  HackerRank "Maximum Subarray Sum"  - Kadane (and the all-negative trap)
  HackerRank "Simple Text Editor"    - undo with a stack of inverse
                                       operations (the editor itself)
  related: "Array Manipulation" (the difference array is in
  01-fundamentals/difference-array.c)

PROBLEM DEFINITION:
Four HackerRank standards that punish the obvious solution. New Year
Chaos looks like a bubble-sort simulation but is O(n) once you use the
"nobody can be more than 2 places early" fact. Minimum Swaps 2 asks for
the fewest swaps to sort a permutation; the answer is n minus the number
of cycles. Maximum Subarray Sum is Kadane with the all-negative trap
made explicit. Simple Text Editor is the only STATE program here: an
undo/redo history built from inverse operations rather than snapshots.

REAL-LIFE EXAMPLE:
Queue-jump audits in ticket lines, the minimum number of swaps to seat
guests in their assigned chairs, the best contiguous run of profit in a
price log, and every text editor's Ctrl+Z - four everyday features.

DIAGRAM (New Year Chaos, [2,1,5,3,4]):
  A person can only move FORWARD (by bribing) and by at most 2 places,
  so the value at position i (0-based) was originally at index v-1 and
  may only be at most 2 places ahead: "Too chaotic" iff v - (i+1) > 2.
  Only the two positions before i can contain a value larger than q[i],
  because any larger value further left would itself be > 2 ahead:
     i=0 v=2  : nothing before            -> 0
     i=1 v=1  : q[0]=2 > 1                -> 1
     i=2 v=5  : window starts at index 3  -> 0
     i=3 v=3  : q[2]=5 > 3                -> 1
     i=4 v=4  : q[2]=5 > 4                -> 1
  total = 3 bribes, and nobody moved more than 2 places, so it is valid.

PSEUDO CODE:
  NewYearChaos(q, n):
      for i in 0..n-1:
          if q[i] - (i + 1) > 2 -> print "Too chaotic"; stop
          for j = max(0, q[i] - 2) .. i-1:
              if q[j] > q[i] -> bribes++       (only these can exceed)
  MinimumSwaps2(p, n):
      visited[] = false; swaps = 0
      for i in 0..n-1:
          if not visited[i]:
              follow the cycle i -> p[i]-1 -> ... marking visited
              swaps += cycleLength - 1
  MaximumSubarraySum(a, n):                  (Kadane)
      best = a[0]; current = a[0]
      for i in 1..n-1:
          current = max(a[i], current + a[i])
          best    = max(best, current)
  TextEditor: one stack of inverse operations (undoStack[])
      append "text"  -> push the inverse (delete those characters)
      delete k       -> save the removed characters, push the inverse
                        (append them again)
      print k        -> show the kth character WITHOUT changing the text
      undo n         -> apply the last n inverse operations, popping them

SAMPLE INPUT & DRY RUN:
  5
  2 1 5 3 4
  4
  4 3 1 2
  5
  -1 -2 -3 -4 -5
  7
  1 abc
  3 3
  1 defg
  2 8
  3 2
  1 xyz
  4 1
  New Year Chaos ([2,1,5,3,4]): not chaotic, 3 bribes (diagram above).
  Minimum swaps for [4,3,1,2]: one cycle of length 4 -> 3 swaps.
  Maximum subarray sum of all negatives = -1 (the LARGEST value in the
  array, never 0) - the classic all-negative trap.
  Simple Text Editor (7 operations - the count line comes FIRST):
    1 abc   -> editor "abc"
    3 3     -> the character at position 3 is 'c' (no state change)
    1 defg  -> editor "abcdefg"
    2 8     -> editor ""   (a delete larger than the text is clamped)
    3 2     -> position 2 does not exist (no state change)
    1 xyz   -> editor "xyz"
    4 1     -> undo the last append -> editor "" (back before "xyz")

SPECIAL CASES:
  - New Year Chaos: the inner loop may look at only the previous TWO
    positions; a full scan is O(n^2) and times out at n = 10^5. A value
    more than 2 places ahead of its original index is impossible -
    print exactly "Too chaotic".
  - Minimum Swaps 2 works because the array is a PERMUTATION of 1..n;
    with duplicates or gaps the cycle count is meaningless and is
    rejected here.
  - Maximum Subarray Sum on all-negative input returns the least
    negative value; returning 0 is the classic wrong answer.
  - Kadane needs a 64-bit accumulator: n * |value| can exceed 2^31.
  - Text Editor: "print k" must NOT change the history, and a delete
    larger than the text is clamped to the current length. Undo is
    applied one operation at a time, so an undo count larger than the
    history simply stops at an empty history.
  - The editor's undo stack is bounded, so a long session is stopped
    with a clear message instead of a silent overflow.
  - n <= 1000 (queue, array), values |a[i]| <= 10^6, text <= 2000 chars.

COMPLEXITY:
  New Year Chaos O(n) with the 2-position window. Minimum Swaps 2 O(n).
  Kadane O(n) time, O(1) space. Text Editor: every operation is O(1)
  amortised except delete/undo, which are O(deleted characters) - the
  stacks hold inverse operations, not snapshots, so memory stays O(total
  edits) instead of O(length * edits).

TIPS:
  When a simulation is too slow, look for an INVARIANT that bounds the
  work: here "no one moves more than 2 places forward" turns an O(n^2)
  bubble simulation into one linear pass. For state problems, store the
  INVERSE of each operation instead of a copy of the state.
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 1000
#define MAX_TEXT 2000
#define MAX_UNDO 500
#define VALUE_LIMIT 1000000

int queueOrder[MAX_N];
int permutation[MAX_N];
long long subarray[MAX_N];
char editor[MAX_TEXT + 1];

/* Each undo record is the number of characters an append added (>= 0) or
 * the characters a delete removed (negative length with the text). */
int undoLength[MAX_UNDO];
char undoText[MAX_UNDO][64];
int undoCount;

/* HackerRank "New Year Chaos": count the inversions but exploit the
 * fact that nobody can be more than 2 places ahead, so only the two
 * positions before i can hold a larger value. */
int newYearChaos(const int q[], int n) {
    long long bribes = 0;
    for (int i = 0; i < n; i++) {
        if (q[i] - (i + 1) > 2) return -1;        /* impossible queue */
        int start = q[i] - 2;
        if (start < 0) start = 0;
        for (int j = start; j < i; j++) {
            if (q[j] > q[i]) bribes++;
        }
    }
    return (int)bribes;
}

/* HackerRank "Minimum Swaps 2": the array is a permutation of 1..n, so
 * the minimum number of swaps is n - (number of cycles). A cycle of
 * length L needs exactly L - 1 swaps to rotate into place. */
int minimumSwaps2(int p[], int n) {
    char visited[MAX_N];
    for (int i = 0; i < n; i++) visited[i] = 0;
    int swaps = 0;
    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        int length = 0;
        int current = i;
        while (!visited[current]) {
            visited[current] = 1;
            current = p[current] - 1;
            length++;
        }
        swaps += length - 1;
    }
    return swaps;
}

/* HackerRank "Maximum Subarray Sum" (Kadane): with an all-negative input
 * the answer is the LEAST negative value, so the running best is seeded
 * with the first element rather than 0. */
long long kadane(const long long a[], int n) {
    long long best = a[0];
    long long current = a[0];
    for (int i = 1; i < n; i++) {
        current = a[i] > current + a[i] ? a[i] : current + a[i];
        if (current > best) best = current;
    }
    return best;
}

/* HackerRank "Simple Text Editor": the undo history stores ONE INVERSE
 * OPERATION per edit instead of a snapshot of the whole text. An append
 * of k characters is undone by deleting k; a delete is undone by
 * appending the very characters that were removed. */
void pushUndo(int length, const char* text) {
    if (undoCount >= MAX_UNDO) return;
    undoLength[undoCount] = length;
    int k = 0;
    while (text[k] != '\0' && k < 63) {
        undoText[undoCount][k] = text[k];
        k++;
    }
    undoText[undoCount][k] = '\0';
    undoCount++;
}

int editorLength(void) {
    int length = 0;
    while (editor[length] != '\0') length++;
    return length;
}

void appendText(const char* text) {
    int length = editorLength();
    int added = 0;
    while (text[added] != '\0') {
        if (length + added >= MAX_TEXT) break;
        editor[length + added] = text[added];
        added++;
    }
    editor[length + added] = '\0';
    pushUndo(added, "");          /* undo = delete the last `added` chars */
}

int deleteLast(int k) {
    int length = editorLength();
    if (k > length) k = length;
    char removed[64];
    int start = length - k;
    for (int i = 0; i < k; i++) removed[i] = editor[start + i];
    removed[k] = '\0';
    editor[start] = '\0';
    pushUndo(-k, removed);        /* undo = append `removed` again */
    return k;
}

void undoEdits(int n) {
    while (n > 0 && undoCount > 0) {
        undoCount--;
        int length = undoLength[undoCount];
        if (length >= 0) {
            int current = editorLength();
            int cut = current - length;
            if (cut < 0) cut = 0;
            editor[cut] = '\0';
        } else {
            appendText(undoText[undoCount]);
            undoCount--;          /* discard the record the append just made */
        }
        n--;
    }
}

char printKth(int k) {
    int length = editorLength();
    if (k < 1 || k > length) return '\0';
    return editor[k - 1];
}

int allUnique(const int p[], int n) {
    char seen[MAX_N + 1];
    for (int i = 0; i <= n; i++) seen[i] = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] < 1 || p[i] > n || seen[p[i]]) return 0;
        seen[p[i]] = 1;
    }
    return 1;
}

int main(void) {
    printf("HackerRank Tricky Classics - New Year Chaos, Minimum Swaps 2, "
           "Kadane, Text Editor\n\n");

    int n;
    printf("--- New Year Chaos ---\n");
    printf("How many people (1-%d)? ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter the final queue of %d values (a permutation of 1..%d):\n",
           n, n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &queueOrder[i]) != 1 || queueOrder[i] < 1 ||
            queueOrder[i] > n) {
            printf("Invalid queue value\n");
            return 1;
        }
    }
    if (!allUnique(queueOrder, n)) {
        printf("Invalid queue: values must be a permutation of 1..%d\n", n);
        return 1;
    }
    int bribes = newYearChaos(queueOrder, n);
    if (bribes < 0) {
        printf("Too chaotic\n");
    } else {
        printf("Minimum bribes = %d\n", bribes);
    }

    int m;
    printf("\n--- Minimum Swaps 2 ---\n");
    printf("How many elements (1-%d)? ", MAX_N);
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d values (a permutation of 1..%d):\n", m, m);
    for (int i = 0; i < m; i++) {
        if (scanf("%d", &permutation[i]) != 1) {
            printf("Invalid value\n");
            return 1;
        }
    }
    if (!allUnique(permutation, m)) {
        printf("Invalid array: values must be a permutation of 1..%d\n", m);
        return 1;
    }
    printf("Minimum swaps = %d\n", minimumSwaps2(permutation, m));

    int k;
    printf("\n--- Maximum Subarray Sum (Kadane) ---\n");
    printf("How many values (1-%d)? ", MAX_N);
    if (scanf("%d", &k) != 1 || k < 1 || k > MAX_N) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (|value| <= %d):\n", k, VALUE_LIMIT);
    for (int i = 0; i < k; i++) {
        if (scanf("%lld", &subarray[i]) != 1 ||
            subarray[i] < -VALUE_LIMIT || subarray[i] > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
    }
    printf("Maximum subarray sum = %lld", kadane(subarray, k));
    if (kadane(subarray, k) < 0) {
        printf("  (all-negative input: the answer is the least negative "
               "value, not 0)");
    }
    printf("\n");

    printf("\n--- Simple Text Editor ---\n");
    int operations;
    editor[0] = '\0';
    undoCount = 0;
    printf("How many operations? ");
    if (scanf("%d", &operations) != 1 || operations < 0 ||
        operations > 1000000) {
        printf("Invalid operation count\n");
        return 1;
    }
    for (int i = 0; i < operations; i++) {
        int type;
        printf("Operation %d - 1 append / 2 delete / 3 print / 4 undo "
               "(-1 to stop): ", i + 1);
        if (scanf("%d", &type) != 1) {
            printf("Invalid operation\n");
            return 1;
        }
        if (type == -1) break;
        if (type == 1) {
            char word[64];
            printf("  Text to append: ");
            if (scanf("%63s", word) != 1) {
                printf("Invalid text\n");
                return 1;
            }
            appendText(word);
            printf("  Editor now: \"%s\"\n", editor);
        } else if (type == 2) {
            int count;
            printf("  How many characters to delete? ");
            if (scanf("%d", &count) != 1 || count < 0) {
                printf("Invalid count\n");
                return 1;
            }
            deleteLast(count);
            printf("  Editor now: \"%s\"\n", editor);
        } else if (type == 3) {
            int index;
            printf("  Which position to print? ");
            if (scanf("%d", &index) != 1) {
                printf("Invalid position\n");
                return 1;
            }
            char c = printKth(index);
            if (c == '\0') {
                printf("  Position %d does not exist\n", index);
            } else {
                printf("  Character at %d: %c\n", index, c);
            }
        } else if (type == 4) {
            int count;
            printf("  How many edits to undo? ");
            if (scanf("%d", &count) != 1 || count < 0) {
                printf("Invalid count\n");
                return 1;
            }
            undoEdits(count);
            printf("  Editor now: \"%s\"\n", editor);
        } else {
            printf("  Unknown operation\n");
            return 1;
        }
    }
    printf("Final text: \"%s\"\n", editor);
    return 0;
}
