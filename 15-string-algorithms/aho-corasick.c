/*
Aho-Corasick - Matching Many Patterns in One Pass (Master)
---------------------------------------------------------
PROBLEM DEFINITION:
Find every occurrence of every pattern from a dictionary inside a text,
reading the text only ONCE. Running KMP separately for k patterns costs
O(k * n); Aho-Corasick builds a trie of all patterns, adds "failure
links" (KMP's prefix function generalised to a trie), and then scans
the text in O(n + number of matches).

REAL-LIFE EXAMPLE:
Antivirus engines match thousands of malware signatures against every
file; intrusion detection (Snort), spam filters and the Unix tool
fgrep do the same with keyword lists. One pass over the data is the
only affordable option.

DIAGRAM (patterns he, she, his, hers):
  root -h-> h -e-> he* -r-> her -s-> hers*
     |       \
     |        -i-> hi -s-> his*
     -s-> s -h-> sh -e-> she*
  failure links (longest proper suffix that is also a trie path):
     sh -> h,   she -> he,   his -> s,   hers -> s,  others -> root
  output link: she -> he, so reaching "she" also reports "he".

PSEUDO CODE:
  Build trie of all patterns; mark the node where each pattern ends
  BFS from the root, level by level:
      for each child v = child(u, c):
          fail[v] = (u is root) ? root : child(fail[u], c)
          output[v] = fail[v] if fail[v] ends a pattern else output[fail[v]]
      for each missing child c of u:
          child(u, c) = (u is root) ? root : child(fail[u], c)
  Search(text):
      state = root
      for i, ch in text:
          state = child(state, ch)
          report patterns ending at state, then follow output links

SAMPLE INPUT & DRY RUN:
  4
  he
  she
  his
  hers
  ahishers
  a -> root; h -> h; i -> hi; s -> his        : "his"  at 1..3
  h -> (his has no h child; fail s -> sh)    sh
  e -> she                                    : "she"  at 3..5
       output link she -> he                  : "he"   at 4..5
  r -> (she has no r child; fail he -> her)  her
  s -> hers                                   : "hers" at 4..7
  Counts: he 1, she 1, his 1, hers 1

SPECIAL CASES:
  - A pattern inside another ("he" in "she", "hers") is reported via
    output links - forgetting them is the classic bug.
  - Overlapping matches are all reported.
  - Duplicate patterns are both counted at every occurrence.
  - Patterns and text use lowercase a-z; up to 100 patterns of length
    1..100, text length 1..10000. At most 100 matches are listed, but
    all are counted.

COMPLEXITY:
  Build O(total pattern length * 26); search O(n + matches).
  Memory O(total pattern length * 26) for the completed goto table.

TIPS:
  With one pattern, the failure links ARE the KMP prefix function.
  Completing missing transitions during the BFS turns the trie into a
  DFA, so the search loop has no inner "while (fail)" loop. Compare
  kmp-string-matching and trie.
*/

#include <stdio.h>
#include <string.h>

#define MAX_PATTERNS 100
#define PATTERN_LENGTH 100
#define MAX_TEXT 10000
#define MAX_NODES (MAX_PATTERNS * PATTERN_LENGTH + 1)
#define ALPHABET 26
#define MAX_LISTED 100
#define TRACE_LIMIT 30

int child[MAX_NODES][ALPHABET];
int fail[MAX_NODES];
int outputLink[MAX_NODES];
int firstPattern[MAX_NODES];
int parentNode[MAX_NODES];
char edgeChar[MAX_NODES];
int nodeCount = 1;
int nextPattern[MAX_PATTERNS];
int patternLength[MAX_PATTERNS];
char patterns[MAX_PATTERNS][PATTERN_LENGTH + 1];
long long matchCount[MAX_PATTERNS];
int queue[MAX_NODES];

int isLowercaseWord(const char *s) {
    for (; *s; s++) {
        if (*s < 'a' || *s > 'z') return 0;
    }
    return 1;
}

void insertPattern(int id) {
    int node = 0;
    for (const char *p = patterns[id]; *p; p++) {
        int c = *p - 'a';
        if (child[node][c] == -1) {
            child[node][c] = nodeCount;
            parentNode[nodeCount] = node;
            edgeChar[nodeCount] = *p;
            nodeCount++;
        }
        node = child[node][c];
    }
    nextPattern[id] = firstPattern[node];
    firstPattern[node] = id;
}

void buildAutomaton(void) {
    int head = 0;
    int tail = 0;
    fail[0] = 0;
    outputLink[0] = -1;
    for (int c = 0; c < ALPHABET; c++) {
        int v = child[0][c];
        if (v == -1) {
            child[0][c] = 0;
        } else {
            fail[v] = 0;
            outputLink[v] = -1;
            queue[tail++] = v;
        }
    }
    while (head < tail) {
        int u = queue[head++];
        for (int c = 0; c < ALPHABET; c++) {
            int v = child[u][c];
            if (v == -1) {
                child[u][c] = child[fail[u]][c];
                continue;
            }
            fail[v] = child[fail[u]][c];
            outputLink[v] = firstPattern[fail[v]] != -1 ? fail[v]
                                                        : outputLink[fail[v]];
            queue[tail++] = v;
        }
    }
}

/* Writes the trie path spelling node into buffer. */
void nodeLabel(int node, char *buffer) {
    int length = 0;
    char reversed[PATTERN_LENGTH + 1];
    for (int v = node; v != 0; v = parentNode[v]) reversed[length++] = edgeChar[v];
    for (int i = 0; i < length; i++) buffer[i] = reversed[length - 1 - i];
    buffer[length] = '\0';
    if (length == 0) strcpy(buffer, "(root)");
}

int main(void) {
    static char text[MAX_TEXT + 2];
    char buffer[PATTERN_LENGTH + 8];
    int k;

    memset(child, -1, sizeof(child));
    memset(firstPattern, -1, sizeof(firstPattern));

    printf("Aho-Corasick - Multi-Pattern Matching\n\n");
    printf("How many patterns (1-%d)? ", MAX_PATTERNS);
    if (scanf("%d", &k) != 1 || k < 1 || k > MAX_PATTERNS) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < k; i++) {
        printf("Pattern %d (lowercase, 1-%d letters): ", i + 1, PATTERN_LENGTH);
        if (scanf("%101s", buffer) != 1 || strlen(buffer) > PATTERN_LENGTH ||
            !isLowercaseWord(buffer)) {
            printf("Invalid pattern\n");
            return 1;
        }
        strcpy(patterns[i], buffer);
        patternLength[i] = (int)strlen(buffer);
        insertPattern(i);
    }
    printf("Text (lowercase, 1-%d letters): ", MAX_TEXT);
    if (scanf("%10001s", text) != 1 || strlen(text) > MAX_TEXT ||
        !isLowercaseWord(text)) {
        printf("Invalid text\n");
        return 1;
    }
    buildAutomaton();

    if (nodeCount <= TRACE_LIMIT) {
        printf("\nTrie nodes with failure links:\n");
        for (int v = 1; v < nodeCount; v++) {
            char target[PATTERN_LENGTH + 8];
            nodeLabel(v, buffer);
            nodeLabel(fail[v], target);
            printf("  %-8s fail -> %-8s", buffer, target);
            if (outputLink[v] != -1) {
                nodeLabel(outputLink[v], target);
                printf(" output -> %s", target);
            }
            printf("\n");
        }
    }

    printf("\nMatches (0-based start..end):\n");
    long long total = 0;
    int state = 0;
    for (int i = 0; text[i]; i++) {
        state = child[state][text[i] - 'a'];
        for (int v = firstPattern[state] != -1 ? state : outputLink[state];
             v != -1; v = outputLink[v]) {
            for (int id = firstPattern[v]; id != -1; id = nextPattern[id]) {
                matchCount[id]++;
                if (total < MAX_LISTED) {
                    printf("  %d..%d: %s\n", i - patternLength[id] + 1, i,
                           patterns[id]);
                }
                total++;
            }
        }
    }
    if (total > MAX_LISTED) printf("  ... (%lld more)\n", total - MAX_LISTED);
    if (total == 0) printf("  none\n");

    printf("\nCounts:");
    for (int i = 0; i < k; i++) printf(" %s %lld%s", patterns[i], matchCount[i],
                                       i + 1 < k ? "," : "\n");
    return 0;
}
