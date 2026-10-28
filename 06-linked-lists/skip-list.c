/*
Skip List - A Sorted Linked List with Express Lanes (Advanced)
-------------------------------------------------------------
PROBLEM DEFINITION:
Keep a sorted set of integers with search, insert and delete in
expected O(log n) time, using nothing but linked lists. Each node is
promoted to higher "express" levels by coin flips, so a search rides
the sparse upper lists first and drops down only near the target.

REAL-LIFE EXAMPLE:
A metro line with local and express trains: the express stops at only
some stations, so you ride it as far as possible, then change to the
local line for the last few stops. Redis sorted sets and the LevelDB /
RocksDB memtables are skip lists for exactly this reason.

DIAGRAM (the sample's list after its 8 inserts, fixed seed):
  Level 2: head ----------------> 9 -------> 19 -------------> NIL
  Level 1: head ------> 6 ------> 9 -> 12 -> 19 -------------> NIL
  Level 0: head -> 3 -> 6 -> 7 -> 9 -> 12 -> 19 -> 21 -> 25 -> NIL
  Search 19: level 2 head->9 (next is 19, not < 19: drop down),
             level 1 9->12 (next is 19: drop), level 0 step onto 19.

PSEUDO CODE:
  RandomLevel(): level = 1; while coin() is heads and level < MAX: level++
  Search(x):
      node = head
      for lv = top down to 0:
          while node.next[lv] != NIL and node.next[lv].key < x:
              node = node.next[lv]
      node = node.next[0];  return node != NIL and node.key == x
  Insert(x): same walk, remembering update[lv] = last node before x;
      if x already present: stop
      h = RandomLevel(); splice the new node after update[lv], lv < h
  Delete(x): same walk; unlink the node from every level it is on;
      lower top while the top level is empty

SAMPLE INPUT & DRY RUN:
  12
  i 3
  i 6
  i 7
  i 9
  i 12
  i 19
  i 21
  i 25
  s 19
  s 8
  d 9
  p
  Levels drawn by the seeded coin: 3->1 6->2 7->1 9->3 12->2 19->3
  21->1 25->1, giving the diagram above.
  s 19: 3 forward moves (head->9, 9->12, 12->19)   -> found
  s 8:  2 forward moves (head->6 on level 1, 6->7 on level 0);
        the next key is 9, so 8 is not present
  d 9:  unlinked from levels 2, 1 and 0; 19 keeps level 2 alive
  p:    Level 2: 19
        Level 1: 6 12 19
        Level 0: 3 6 7 12 19 21 25

SPECIAL CASES:
  - Duplicate inserts are ignored (set semantics); deleting a missing
    value is reported.
  - Searching an empty list returns "not found" immediately.
  - The coin is a fixed-seed xorshift generator (not rand()), so every
    compiler prints the same levels; change SEED to see other shapes.
  - Keys are -10^9..10^9, at most 1000 operations, MAX_LEVEL 16.

COMPLEXITY:
  Expected O(log n) per search/insert/delete; expected O(n) space
  (each node has 2 pointers on average with p = 1/2).
  Worst case O(n) if every coin flip is unlucky - vanishingly rare.

TIPS:
  A skip list gives balanced-BST performance with no rotations, which
  is why it is popular in concurrent code. Compare treap (randomized
  tree) and avl-tree / red-black-tree (deterministic balance).
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEVEL 16
#define MAX_OPERATIONS 1000
#define KEY_LIMIT 1000000000
#define SEED 273u

typedef struct Node {
    int key;
    struct Node *next[MAX_LEVEL];
} Node;

Node head;
int topLevel = 1;
unsigned int rngState = SEED;

unsigned int nextRandom(void) {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

int randomLevel(void) {
    int level = 1;
    while (level < MAX_LEVEL && (nextRandom() & 1u)) level++;
    return level;
}

/* Walks down from the top level; update[lv] = last node with key < x. */
Node *findPredecessors(int x, Node *update[], int *moves) {
    Node *node = &head;
    for (int lv = topLevel - 1; lv >= 0; lv--) {
        while (node->next[lv] != NULL && node->next[lv]->key < x) {
            node = node->next[lv];
            if (moves != NULL) (*moves)++;
        }
        update[lv] = node;
    }
    return node->next[0];
}

int search(int x, int *moves) {
    Node *update[MAX_LEVEL];
    *moves = 0;
    Node *candidate = findPredecessors(x, update, moves);
    if (candidate != NULL && candidate->key == x) {
        (*moves)++;
        return 1;
    }
    return 0;
}

int insert(int x, int *levelOut) {
    Node *update[MAX_LEVEL];
    Node *candidate = findPredecessors(x, update, NULL);
    if (candidate != NULL && candidate->key == x) return 0;

    int level = randomLevel();
    for (int lv = topLevel; lv < level; lv++) update[lv] = &head;
    if (level > topLevel) topLevel = level;

    Node *node = malloc(sizeof(Node));
    if (node == NULL) return -1;
    node->key = x;
    for (int lv = 0; lv < MAX_LEVEL; lv++) node->next[lv] = NULL;
    for (int lv = 0; lv < level; lv++) {
        node->next[lv] = update[lv]->next[lv];
        update[lv]->next[lv] = node;
    }
    *levelOut = level;
    return 1;
}

int erase(int x) {
    Node *update[MAX_LEVEL];
    Node *target = findPredecessors(x, update, NULL);
    if (target == NULL || target->key != x) return 0;
    for (int lv = 0; lv < topLevel; lv++) {
        if (update[lv]->next[lv] != target) break;
        update[lv]->next[lv] = target->next[lv];
    }
    free(target);
    while (topLevel > 1 && head.next[topLevel - 1] == NULL) topLevel--;
    return 1;
}

void printLevels(void) {
    for (int lv = topLevel - 1; lv >= 0; lv--) {
        printf("  Level %d:", lv);
        for (Node *node = head.next[lv]; node != NULL; node = node->next[lv]) {
            printf(" %d", node->key);
        }
        printf("\n");
    }
}

void freeAll(void) {
    Node *node = head.next[0];
    while (node != NULL) {
        Node *following = node->next[0];
        free(node);
        node = following;
    }
}

int main(void) {
    int q;

    printf("Skip List - Sorted Set with Express Lanes\n\n");
    printf("Commands: i x (insert), d x (delete), s x (search), p (print)\n");
    printf("How many commands (1-%d)? ", MAX_OPERATIONS);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_OPERATIONS) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        char command;
        int x = 0;
        int moves;
        int level;
        if (scanf(" %c", &command) != 1) {
            printf("Invalid command\n");
            freeAll();
            return 1;
        }
        if (command != 'p' && (scanf("%d", &x) != 1 || x < -KEY_LIMIT ||
                               x > KEY_LIMIT)) {
            printf("Invalid value\n");
            freeAll();
            return 1;
        }
        switch (command) {
        case 'i': {
            int result = insert(x, &level);
            if (result < 0) {
                printf("Out of memory\n");
                freeAll();
                return 1;
            }
            if (result == 0) printf("insert %d: already present\n", x);
            else printf("insert %d: level %d\n", x, level);
            break;
        }
        case 'd':
            printf("delete %d: %s\n", x, erase(x) ? "removed" : "not found");
            break;
        case 's':
            if (search(x, &moves)) {
                printf("search %d: found after %d forward move%s\n", x, moves,
                       moves == 1 ? "" : "s");
            } else {
                printf("search %d: not found (%d forward move%s)\n", x, moves,
                       moves == 1 ? "" : "s");
            }
            break;
        case 'p':
            printf("skip list (%d level%s):\n", topLevel,
                   topLevel == 1 ? "" : "s");
            printLevels();
            break;
        default:
            printf("Unknown command '%c'\n", command);
            freeAll();
            return 1;
        }
    }
    freeAll();
    return 0;
}
