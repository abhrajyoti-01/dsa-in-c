/*
BST Problems Ladder (Basic -> Hard)
-----------------------------------
Real life example:
Dictionaries, autocomplete indexes and database range scans are
BST thinking: compare, go left or right, halving the candidate set
until the answer region is found.

DIAGRAM (validate BST with min/max ranges; LCA by split):
  50(30(20,40),70(60,80))
  node 30 must lie in (-inf, 50); node 70 in (50, +inf)
  LCA(20, 40): at 50 both smaller -> left; at 30: 20 < 30 < 40
  -> paths split here -> LCA = 30.

LADDER (easy -> hard inside this file):
  1. Insert + in-order (shows sorted output - THE BST fact)
  2. Search with path trace
  3. Min and Max (leftmost / rightmost walk)
  4. k-th smallest (in-order counter)
  5. Lowest Common Ancestor (BST property version)
  6. Validate BST (range-check, not just parent check)
  7. Sorted array -> balanced BST (recursion on midpoints)

BEGINNER EXPLANATION:
BST rule: left subtree < node < right subtree (all the way down).
That single invariant makes EVERYTHING easy:
- search: go left if smaller, right if bigger - halving each step.
- min = leftmost node, max = rightmost.
- in-order traversal prints the keys SORTED.
- LCA of a,b: first node where a and b split left/right.

SUPER SIMPLE EXAMPLE (LCA):
Tree: 50(30(20,40),70(60,80)). LCA(20, 40)?
Start 50: both 20,40 < 50 -> go left. Node 30: 20 < 30 < 40 ->
SPLIT -> LCA = 30. Two comparisons - no searching needed.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Validate BST: checking only node < right child FAILS on
  50(30(20, 60)) - 60 is right of 30 but also > 50. Correct check:
  every node within a (minAllowed, maxAllowed) RANGE tightened
  down the recursion. The #1 BST interview trap.
- Duplicates: this file REJECTS equal keys on insert (state your
  duplicate policy; a count-field is the industrial choice).
- k-th smallest: in-order with a countdown - visit left, decrement
  counter, check zero, visit right. No extra array needed.
- Sorted array to BST: pick MIDDLE as root, recurse on halves ->
  height = log n automatically (balanced by construction).

DRY RUN (insert 50, 30, 70, 20, 40):
50 -> root. 30 < 50 -> left. 70 > 50 -> right.
20 < 50, < 30 -> left of 30. 40: < 50, > 30 -> right of 30.
In-order: 20 30 40 50 70 - sorted, as promised.

TIPS:
- All BST ops are O(h): O(log n) if balanced, O(n) if skewed -
  that degeneration is WHY AVL/red-black-tree exist (avl_tree.c, red-black-tree.c).
- The range-validation trick generalizes: "count values in
  [a,b]" and "trim BST to range" are the same walk.
- Sorted-array-to-BST is the standard "make a balanced BST from
  sorted data" answer - middle-element recursion.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_ARR 100

typedef struct BNode {
    int key;
    struct BNode* left;
    struct BNode* right;
} BNode;

BNode* newBNode(int key) {
    BNode* node = (BNode*)malloc(sizeof(BNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
 * bstInsert - Duplicates rejected.
 */
BNode* bstInsert(BNode* root, int key) {
    if (root == NULL) {
        return newBNode(key);
    }
    if (key < root->key) {
        root->left = bstInsert(root->left, key);
    } else if (key > root->key) {
        root->right = bstInsert(root->right, key);
    }
    return root;
}

void inorderPrint(BNode* root) {
    if (root == NULL) {
        return;
    }
    inorderPrint(root->left);
    printf("%d ", root->key);
    inorderPrint(root->right);
}

/*
 * bstSearch - Traces the comparison path.
 */
int bstSearch(BNode* root, int key) {
    while (root != NULL) {
        printf("  at %d: ", root->key);
        if (key == root->key) {
            printf("FOUND\n");
            return 1;
        }
        if (key < root->key) {
            printf("%d < %d -> left\n", key, root->key);
            root = root->left;
        } else {
            printf("%d > %d -> right\n", key, root->key);
            root = root->right;
        }
    }
    printf("  reached NULL -> NOT FOUND\n");
    return 0;
}

int bstMin(BNode* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root->key;
}

int bstMax(BNode* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root->key;
}

/*
 * kthSmallest - In-order walk with a countdown.
 */
int kthSmallest(BNode* root, int* k) {
    if (root == NULL) {
        return -1;
    }
    int left = kthSmallest(root->left, k);
    if (*k == 0) {
        return left;
    }
    *k = *k - 1;
    if (*k == 0) {
        return root->key;
    }
    return kthSmallest(root->right, k);
}

/*
 * findLCA - Uses the BST ordering property.
 */
int findLCA(BNode* root, int a, int b) {
    while (root != NULL) {
        if (a < root->key && b < root->key) {
            root = root->left;
        } else if (a > root->key && b > root->key) {
            root = root->right;
        } else {
            return root->key;
        }
    }
    return -1;
}

/*
 * isValidBST - Range check down the recursion.
 */
int isValidBST(BNode* root, long long minAllowed, long long maxAllowed) {
    if (root == NULL) {
        return 1;
    }
    if (root->key <= minAllowed || root->key >= maxAllowed) {
        return 0;
    }
    return isValidBST(root->left, minAllowed, root->key) &&
           isValidBST(root->right, root->key, maxAllowed);
}

/*
 * sortedArrayToBST - Middle-element recursion (balanced by design).
 */
BNode* sortedArrayToBST(const int arr[], int lo, int hi) {
    if (lo > hi) {
        return NULL;
    }
    int mid = lo + (hi - lo) / 2;
    BNode* node = newBNode(arr[mid]);
    node->left = sortedArrayToBST(arr, lo, mid - 1);
    node->right = sortedArrayToBST(arr, mid + 1, hi);
    return node;
}

void freeBTree(BNode* root) {
    if (root == NULL) {
        return;
    }
    freeBTree(root->left);
    freeBTree(root->right);
    free(root);
}

int main() {
    BNode* root = NULL;
    int n;
    int key;

    printf("BST Problems Ladder (easy -> hard)\n\n");

    printf("How many keys to insert (1-%d)? ", MAX_ARR);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ARR) {
        printf("Invalid count\n");
        return 1;
    }

    printf("Enter %d distinct keys: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &key);
        root = bstInsert(root, key);
    }

    printf("\nIn-order (sorted!): ");
    inorderPrint(root);
    printf("\n");
    printf("Min = %d, Max = %d\n", bstMin(root), bstMax(root));

    printf("\nSearch for key: ");
    scanf("%d", &key);
    bstSearch(root, key);

    int k;
    printf("k for k-th smallest (1..%d): ", n);
    scanf("%d", &k);
    if (k >= 1 && k <= n) {
        int kk = k;
        printf("%d-th smallest = %d\n", k, kthSmallest(root, &kk));
    }

    int a;
    int b;
    printf("LCA of a and b: ");
    scanf("%d %d", &a, &b);
    printf("LCA(%d, %d) = %d\n", a, b, findLCA(root, a, b));

    printf("Is valid BST: %s\n",
           isValidBST(root, -1000000000LL, 1000000000LL) ? "YES" : "NO");

    /* STEP: sorted array -> balanced BST demo */
    int arr[MAX_ARR];
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    BNode* balanced = sortedArrayToBST(arr, 0, n - 1);
    printf("\nBalanced BST from 1..%d, in-order: ", n);
    inorderPrint(balanced);
    printf("\n");

    freeBTree(root);
    freeBTree(balanced);
    return 0;
}
