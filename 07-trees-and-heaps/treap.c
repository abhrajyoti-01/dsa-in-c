/*
Treap - A Randomized Balanced BST with Split and Merge (Advanced)
-----------------------------------------------------------------
PROBLEM DEFINITION:
Maintain a set of integers supporting insert, delete, "k-th smallest"
and "how many values are less than x", all in expected O(log n). Each
node stores a key (kept in BST order) and a random priority (kept in
max-heap order). Random priorities give the tree the shape of a
randomly built BST, so the expected height is O(log n) whatever order
the keys arrive in - even sorted input.

REAL-LIFE EXAMPLE:
An online leaderboard: players join and leave all the time, and the
site must answer "who is ranked 10th?" and "how many players scored
below you?" instantly. Subtree sizes inside a treap answer both.

DIAGRAM (illustration, key/priority: BST by key, max-heap by priority):
              50/97
             /     \
         20/81     70/88
         /   \         \
     10/23  30/64     80/40
  split(T, 40) -> L = keys < 40 {10,20,30},  R = keys >= 40 {50,70,80}
  merge(L, R): every key in L is smaller than every key in R, and the
  new root is whichever of the two roots has the higher priority.

PSEUDO CODE:
  Split(t, x): returns (keys < x, keys >= x)
      if t is empty: return (empty, empty)
      if t.key < x: (a, b) = Split(t.right, x); t.right = a; return (t, b)
      else:         (a, b) = Split(t.left, x);  t.left = b;  return (a, t)
  Merge(a, b):    (all keys of a < all keys of b)
      if a or b is empty: return the other one
      if a.priority > b.priority: a.right = Merge(a.right, b); return a
      else:                       b.left = Merge(a, b.left);   return b
  Insert(x): (l, r) = Split(root, x); (mid, r) = Split(r, x + 1)
             if mid is empty: mid = NewNode(x, random priority)
             root = Merge(Merge(l, mid), r)
  Delete(x): (l, r) = Split(root, x); (mid, r) = Split(r, x + 1)
             free mid; root = Merge(l, r)
  Kth(k): at each node compare k with size(left) + 1 and go left/right
  CountLess(x): walk down; when going right add size(left) + 1

SAMPLE INPUT & DRY RUN:
  13
  i 50
  i 20
  i 70
  i 10
  i 30
  i 80
  i 60
  i 40
  k 3
  c 55
  d 20
  k 3
  p
  k 3 on {10,20,30,40,50,60,70,80}: at each node compare k with
      size(left) + 1 and go left, stop, or go right -> 30
  c 55: keys 10 20 30 40 50 are smaller -> 5
  d 20: split out [20, 21) and merge the two outer parts back
  k 3 on {10,30,40,50,60,70,80} -> 40
  p: in order 10 30 40 50 60 70 80; with the fixed seed the tree is
          80                     root 60, right child 80 (left: 70),
                70               left child 40 (right: 50, left: 10,
    60                           whose right child is 30): height 4
                50
          40
                      30
                10

SPECIAL CASES:
  - Inserting an existing key or deleting a missing key is reported
    and leaves the tree unchanged (set semantics).
  - k must be 1..size; k-th of an empty treap is reported as invalid.
  - Sorted insertions (1, 2, 3, ...) still give expected O(log n)
    height - the case that ruins a plain BST.
  - Priorities come from a fixed-seed xorshift generator, so every
    compiler builds the same tree shape.
  - Keys -10^9..10^9, at most 10000 commands.

COMPLEXITY:
  Expected O(log n) per operation, O(n) space.
  Recursion depth equals the height, expected about 2-3 * log2(n).

TIPS:
  Split/merge treaps replace rotations with two short recursive
  functions, and they extend to "implicit treaps" (index-keyed arrays
  with O(log n) insert-anywhere and reverse-a-range). Compare skip-list
  (the same randomness idea on linked lists) and avl-tree.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_COMMANDS 10000
#define KEY_LIMIT 1000000000
#define SEED 12345u

typedef struct Node {
    int key;
    unsigned int priority;
    int size;
    struct Node *left;
    struct Node *right;
} Node;

unsigned int rngState = SEED;

unsigned int nextRandom(void) {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

int size(Node *t) {
    return t == NULL ? 0 : t->size;
}

void update(Node *t) {
    if (t != NULL) t->size = 1 + size(t->left) + size(t->right);
}

/* Splits t into *left (keys < x) and *right (keys >= x). */
void split(Node *t, int x, Node **left, Node **right) {
    if (t == NULL) {
        *left = NULL;
        *right = NULL;
    } else if (t->key < x) {
        split(t->right, x, &t->right, right);
        *left = t;
    } else {
        split(t->left, x, left, &t->left);
        *right = t;
    }
    update(t);
}

Node *merge(Node *a, Node *b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    if (a->priority > b->priority) {
        a->right = merge(a->right, b);
        update(a);
        return a;
    }
    b->left = merge(a, b->left);
    update(b);
    return b;
}

/* Returns 1 if inserted, 0 if the key was present, -1 if out of memory. */
int insert(Node **root, int x) {
    Node *left;
    Node *middle;
    Node *right;
    split(*root, x, &left, &right);
    split(right, x + 1, &middle, &right);
    int result = 0;
    if (middle == NULL) {
        middle = malloc(sizeof(Node));
        if (middle == NULL) {
            *root = merge(left, right);
            return -1;
        }
        middle->key = x;
        middle->priority = nextRandom();
        middle->size = 1;
        middle->left = NULL;
        middle->right = NULL;
        result = 1;
    }
    *root = merge(merge(left, middle), right);
    return result;
}

int erase(Node **root, int x) {
    Node *left;
    Node *middle;
    Node *right;
    split(*root, x, &left, &right);
    split(right, x + 1, &middle, &right);
    *root = merge(left, right);
    if (middle == NULL) return 0;
    free(middle);
    return 1;
}

int kth(Node *t, int k) {
    while (t != NULL) {
        int leftSize = size(t->left);
        if (k == leftSize + 1) return t->key;
        if (k <= leftSize) {
            t = t->left;
        } else {
            k -= leftSize + 1;
            t = t->right;
        }
    }
    return 0;
}

int countLess(Node *t, int x) {
    int count = 0;
    while (t != NULL) {
        if (t->key < x) {
            count += size(t->left) + 1;
            t = t->right;
        } else {
            t = t->left;
        }
    }
    return count;
}

int height(Node *t) {
    if (t == NULL) return 0;
    int a = height(t->left);
    int b = height(t->right);
    return 1 + (a > b ? a : b);
}

void printInOrder(Node *t) {
    if (t == NULL) return;
    printInOrder(t->left);
    printf(" %d", t->key);
    printInOrder(t->right);
}

/* Sideways drawing: right subtree on top, root at the left margin. */
void printSideways(Node *t, int depth) {
    if (t == NULL) return;
    printSideways(t->right, depth + 1);
    printf("    %*s%d\n", depth * 6, "", t->key);
    printSideways(t->left, depth + 1);
}

void freeTree(Node *t) {
    if (t == NULL) return;
    freeTree(t->left);
    freeTree(t->right);
    free(t);
}

int main(void) {
    int q;
    Node *root = NULL;

    printf("Treap - Randomized BST with Split and Merge\n\n");
    printf("Commands: i x (insert), d x (delete), k idx (k-th smallest),\n");
    printf("          c x (count < x), p (print)\n");
    printf("How many commands (1-%d)? ", MAX_COMMANDS);
    if (scanf("%d", &q) != 1 || q < 1 || q > MAX_COMMANDS) {
        printf("Invalid count\n");
        return 1;
    }
    for (int i = 0; i < q; i++) {
        char command;
        int x = 0;
        if (scanf(" %c", &command) != 1) {
            printf("Invalid command\n");
            freeTree(root);
            return 1;
        }
        if (command != 'p' && (scanf("%d", &x) != 1 || x < -KEY_LIMIT ||
                               x > KEY_LIMIT)) {
            printf("Invalid value\n");
            freeTree(root);
            return 1;
        }
        switch (command) {
        case 'i': {
            int result = insert(&root, x);
            if (result < 0) {
                printf("Out of memory\n");
                freeTree(root);
                return 1;
            }
            printf("insert %d: %s\n", x, result ? "ok" : "already present");
            break;
        }
        case 'd':
            printf("delete %d: %s\n", x,
                   erase(&root, x) ? "removed" : "not found");
            break;
        case 'k':
            if (x < 1 || x > size(root)) {
                printf("kth %d: invalid (treap holds %d keys)\n", x,
                       size(root));
            } else {
                printf("kth %d: %d\n", x, kth(root, x));
            }
            break;
        case 'c':
            printf("count < %d: %d\n", x, countLess(root, x));
            break;
        case 'p':
            printf("in order:");
            printInOrder(root);
            printf("\nsize %d, height %d; tree (rotated, root at left):\n",
                   size(root), height(root));
            printSideways(root, 0);
            break;
        default:
            printf("Unknown command '%c'\n", command);
            freeTree(root);
            return 1;
        }
    }
    freeTree(root);
    return 0;
}
