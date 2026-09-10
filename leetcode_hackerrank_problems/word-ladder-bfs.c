/*
Word Ladder - BFS on an Implicit Graph (Advanced)
-------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 127 - Word Ladder (shortest transformation sequence)
  LeetCode 126 - Word Ladder II (all shortest paths - the follow-up)
  LeetCode 433 - Minimum Genetic Mutation (identical model on DNA)
  HackerRank "BFS: Shortest Reach" applied to words

PROBLEM DEFINITION:
Transform a start word into a target word one letter at a time, every
intermediate word coming from a dictionary. Words are vertices; two
words are adjacent when they differ in exactly one position. BFS finds
the fewest transformations.

REAL-LIFE EXAMPLE:
Spell-check suggestions, DNA point-mutation chains and keyboard
autocorrect ladders all model "minimum single-step edits" this way.

DIAGRAM (start "hit", target "cog", dictionary hot dot dog lot log cog):
  hit --hot-- dot --dog-- cog        length 5 words
         \             /
          --lot-- log --             length 5 words as well
  BFS layers: {hit} -> {hot} -> {dot, lot} -> {dog, log} -> {cog}
  shortest sequence length = 5 (words), i.e. 4 transformations.

PSEUDO CODE:
  Put all dictionary words into a set; keep the target separate
  BFS from start with distance 1 (the start word counts as one step)
  For each popped word, try ALL 26 letters in EVERY position:
      candidate = word with that letter
      if candidate is the target: return distance + 1
      if candidate is in the set: remove it (mark visited), push with
          distance + 1
  Exhausted queue -> impossible

SAMPLE INPUT & DRY RUN:
  hit
  cog
  6
  hot dot dog lot log cog
  hit -> hot -> dot -> dog -> cog = 5 words. The alternative route
  through lot/log also needs 5 words; BFS reports 5 (the minimum).

SPECIAL CASES:
  - start == target: answer 1 (one word, no transformations).
  - target absent from the dictionary: impossible (BFS drains).
  - A word is REMOVED from the set when enqueued, which both marks it
    visited and prevents a second visit at the same or a longer
    distance - the standard trick for implicit BFS on words.
  - Words shorter than the start word can never be produced (every
    candidate keeps the length); the program requires equal lengths.
  - Case matters: the program accepts lowercase only and says so.
  - Words are 1..10 letters, dictionary size 1..1000.

COMPLEXITY:
  O(N * L * 26 * L) time where N is the dictionary size and L the word
  length: every word expands L*26 candidates and each lookup scans the
  dictionary (a hash set would remove the final L factor). Space
  O(N * L).

TIPS:
  This is the "build the graph lazily" pattern: never materialise the
  O(N^2) adjacency, generate neighbours on the fly. The same trick
  turns Sudoku, 8-puzzle and Rubik's-cube states into BFS problems - if
  you can generate neighbours cheaply, you can BFS.
*/

#include <stdio.h>
#include <string.h>

#define MAX_WORDS 1000
#define MAX_LEN 10

char dictionary[MAX_WORDS][MAX_LEN + 1];
int wordCount;
char queueWords[MAX_WORDS][MAX_LEN + 1];
int queueDistance[MAX_WORDS];
unsigned char used[MAX_WORDS];

int findWord(const char* word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(dictionary[i], word) == 0) return i;
    }
    return -1;
}

int main(void) {
    char start[MAX_LEN + 2];
    char target[MAX_LEN + 2];
    int length;
    int front = 0;
    int rear = 0;
    int found = 0;

    printf("Word Ladder - BFS on One-Letter Changes\n\n");
    printf("Enter the start word (1..%d lowercase letters): ", MAX_LEN);
    if (scanf("%11s", start) != 1) {
        printf("Invalid start word\n");
        return 1;
    }
    printf("Enter the target word: ");
    if (scanf("%11s", target) != 1) {
        printf("Invalid target word\n");
        return 1;
    }
    length = (int)strlen(start);
    if (length < 1 || length > MAX_LEN ||
        (int)strlen(target) != length) {
        printf("Invalid words: both must have length 1..%d\n", MAX_LEN);
        return 1;
    }
    for (int i = 0; i < length; i++) {
        if (start[i] < 'a' || start[i] > 'z' || target[i] < 'a' ||
            target[i] > 'z') {
            printf("Invalid word: lowercase letters only\n");
            return 1;
        }
    }

    printf("How many dictionary words (1-%d)? ", MAX_WORDS);
    if (scanf("%d", &wordCount) != 1 || wordCount < 1 ||
        wordCount > MAX_WORDS) {
        printf("Invalid word count\n");
        return 1;
    }
    printf("Enter %d lowercase words of length %d:\n", wordCount, length);
    for (int i = 0; i < wordCount; i++) {
        if (scanf("%11s", dictionary[i]) != 1 ||
            (int)strlen(dictionary[i]) != length) {
            printf("Invalid word length\n");
            return 1;
        }
        for (int j = 0; j < length; j++) {
            if (dictionary[i][j] < 'a' || dictionary[i][j] > 'z') {
                printf("Invalid word: lowercase letters only\n");
                return 1;
            }
        }
        if (findWord(dictionary[i]) != i) {
            printf("Duplicate dictionary word %s ignored\n", dictionary[i]);
            wordCount--;
            i--;
        } else {
            used[i] = 0;
        }
    }

    if (strcmp(start, target) == 0) {
        printf("\nStart equals target -> shortest sequence length = 1\n");
        return 0;
    }
    if (findWord(target) == -1) {
        printf("\nTarget word is not in the dictionary -> impossible\n");
        return 0;
    }

    strcpy(queueWords[rear], start);
    queueDistance[rear] = 1;
    rear++;
    while (front < rear && !found) {
        char current[MAX_LEN + 1];
        strcpy(current, queueWords[front]);
        int distance = queueDistance[front];
        front++;
        for (int position = 0; position < length && !found; position++) {
            char original = current[position];
            for (char letter = 'a'; letter <= 'z' && !found; letter++) {
                if (letter == original) continue;
                current[position] = letter;
                if (strcmp(current, target) == 0) {
                    printf("\nShortest sequence length = %d words",
                           distance + 1);
                    printf(" (%d transformations)\n", distance);
                    found = 1;
                    break;
                }
                int index = findWord(current);
                if (index != -1 && !used[index]) {
                    used[index] = 1;
                    strcpy(queueWords[rear], current);
                    queueDistance[rear] = distance + 1;
                    rear++;
                }
            }
            current[position] = original;
        }
    }
    if (!found) {
        printf("\nNo transformation sequence exists (impossible)\n");
    }
    return 0;
}
