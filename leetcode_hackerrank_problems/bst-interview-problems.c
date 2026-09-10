/*
BST Interview Problems - Validate, Kth Smallest, Floor/Ceil, Range Count
(Intermediate -> Advanced)
-----------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 98   - Validate Binary Search Tree
  LeetCode 230  - Kth Smallest Element in a BST
  LeetCode 700  - Search in a Binary Search Tree
  LeetCode 701  - Insert into a Binary Search Tree
  LeetCode 938  - Range Sum of BST
  LeetCode 450  - Delete Node in a BST (implemented here)
  HackerRank "Binary Search Tree: Insertion / Lowest Common Ancestor"

PROBLEM DEFINITION:
Build a BST by inserting values, then exercise the six operations
every interview asks for. The insertion ORDER decides the shape, so a
sorted input degenerates into a list - the reason balanced trees exist
(see 07-trees-and-heaps/avl-tree.c).

REAL-LIFE EXAMPLE:
An ordered price index: insert quotes as they arrive, find the kth
cheapest product, and answer "what is the highest price not exceeding
my budget?" (the floor) or "the cheapest price at or above it?" (the
ceil).

DIAGRAM (insertions 8 3 10 1 6 14 4 7 13):
          8
        /   \
       3     10
      / \      \
     1   6      14
        / \    /
       4   7  13
  in-order : 1 3 4 6 7 8 10 13 14     (always sorted!)
  kth(3)   : 4        floor(5) = 4    ceil(5) = 6
  range sum on [4..10] = 4+6+7+8+10 = 35

PSEUDO CODE:
  Insert(node, value): go left if value < node, right if value > node,
      create a leaf at the first null; duplicates are refused
  Search(node, value): same walk, return the node or null
  Validate(node, low, high): value must satisfy low < value < high,
      then recurse with tightened bounds
  KthSmallest(node): in-order counter; the kth visited node is the answer
  Floor(node, value): keep the best candidate while walking;
      if value < node go left, else record node and go right
  Ceil(node, value): mirrored logic
  RangeSum(node, low, high): prune whole subtrees outside the range
  Delete(node, value): find it, then splice: 0/1 child -> replace with
      the child; 2 children -> replace the value with the in-order
      SUCCESSOR (leftmost of the right subtree) and delete that node

SAMPLE INPUT & DRY RUN:
  9
  8 3 10 1 6 14 4 7 13
  3
  3            <- how many floor/ceil queries follow
  4 5 3        <- the queried values
  4 10         <- range sum [4..10]
  8            <- value to delete
  In-order is 1 3 4 6 7 8 10 13 14; validation passes; the 3rd smallest
  is 4; floor(5) = 4 and ceil(5) = 6; the range sum over [4,10] is 35.
  Deleting 8 (a two-child node) swaps in its in-order successor 10 and
  then removes the old 10 node, leaving 1 3 4 6 7 10 13 14.

SPECIAL CASES:
  - Validation must pass BOUNDS down, not just compare parent/child:
     5
    / \        the classic counterexample: 6 > 5 is fine locally, but
   3   8       6 sits in 5's right subtree while 4 is under 3's right...
       /       only the (low, high) window rejects the bad tree.
      6
  - Duplicate values: refused here (a set semantics choice). Say so;
    multi-set variants keep a count per node.
  - k out of [1, size] is rejected; the in-order counter aborts early.
  - Floor/ceil of an absent value still returns a valid bracket, or an
    explicit "none" when the tree holds nothing on that side.
  - Deleting a leaf, a one-child node and a two-child node are all
    exercised above: deleting 8 (two children) replaces its value with
    its in-order successor 10, then removes the old 10 node.
  - n <= 1000, values within +/-10^9.

COMPLEXITY:
  All operations O(h) time where h is the tree height: O(log n) when
  balanced, O(n) for a sorted insertion order. Space O(h) recursion.

TIPS:
  "In-order of a BST is sorted" answers half of all BST questions.
  Any question about a RANGE ("how many values between a and b?") is
  answered by pruning with the BST property - never by visiting every
  node.
*/

#include <stdio.h>

#define MAX_EMPLOYEES 1000
#define VALUE_LIMIT 1000000000LL

typedef struct BSTNode {
    long long value;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

BSTNode pool[MAX_EMPLOYEES];
int poolUsed;

void printInOrder(const BSTNode* node) {
    if (node == NULL) return;
    printInOrder(node->left);
    printf(" %lld", node->value);
    printInOrder(node->right);
}

int countNodes(const BSTNode* node) {
    if (node == NULL) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

BSTNode* createNode(long long value) {
    BSTNode* node = &pool[poolUsed++];
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSTNode* insertNode(BSTNode* node, long long value, int* duplicate) {
    if (node == NULL) return createNode(value);
    if (value < node->value) {
        node->left = insertNode(node->left, value, duplicate);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value, duplicate);
    } else {
        *duplicate = 1;
    }
    return node;
}

const BSTNode* searchNode(const BSTNode* node, long long value) {
    while (node != NULL) {
        if (value == node->value) return node;
        node = value < node->value ? node->left : node->right;
    }
    return NULL;
}

int validateBst(const BSTNode* node, long long low, long long high,
                int hasLow, int hasHigh) {
    if (node == NULL) return 1;
    if (hasLow && node->value <= low) return 0;
    if (hasHigh && node->value >= high) return 0;
    return validateBst(node->left, low, node->value, hasLow, 1) &&
           validateBst(node->right, node->value, high, 1, hasHigh);
}

int inOrderValues(const BSTNode* node, long long out[], int index) {
    if (node == NULL) return index;
    index = inOrderValues(node->left, out, index);
    out[index++] = node->value;
    return inOrderValues(node->right, out, index);
}

void kthSmallest(const BSTNode* node, int k, int* visited, long long* answer) {
    if (node == NULL || *visited >= k) return;
    kthSmallest(node->left, k, visited, answer);
    (*visited)++;
    if (*visited == k) {
        *answer = node->value;
        return;
    }
    if (*visited < k) kthSmallest(node->right, k, visited, answer);
}

int floorValue(const BSTNode* node, long long value, long long* answer) {
    int found = 0;
    while (node != NULL) {
        if (node->value == value) {
            *answer = node->value;
            return 1;
        }
        if (node->value < value) {
            *answer = node->value;
            found = 1;
            node = node->right;
        } else {
            node = node->left;
        }
    }
    return found;
}

int ceilValue(const BSTNode* node, long long value, long long* answer) {
    int found = 0;
    while (node != NULL) {
        if (node->value == value) {
            *answer = node->value;
            return 1;
        }
        if (node->value > value) {
            *answer = node->value;
            found = 1;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return found;
}

long long rangeSum(const BSTNode* node, long long low, long long high) {
    if (node == NULL) return 0;
    if (node->value < low) return rangeSum(node->right, low, high);
    if (node->value > high) return rangeSum(node->left, low, high);
    return node->value + rangeSum(node->left, low, high) +
           rangeSum(node->right, low, high);
}

BSTNode* deleteNode(BSTNode* node, long long value, int* removed) {
    if (node == NULL) return NULL;
    if (value < node->value) {
        node->left = deleteNode(node->left, value, removed);
    } else if (value > node->value) {
        node->right = deleteNode(node->right, value, removed);
    } else {
        *removed = 1;
        if (node->left == NULL) return node->right;
        if (node->right == NULL) return node->left;
        const BSTNode* successor = node->right;
        while (successor->left != NULL) successor = successor->left;
        node->value = successor->value;
        int ignored = 0;
        node->right = deleteNode(node->right, successor->value, &ignored);
    }
    return node;
}

int main(void) {
    int n;

    printf("BST Interview Problems\n\n");
    printf("How many values to insert (1-%d)? ", MAX_EMPLOYEES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_EMPLOYEES) {
        printf("Invalid count\n");
        return 1;
    }
    printf("Enter %d integers (within +/-10^9):\n", n);
    BSTNode* root = NULL;
    for (int i = 0; i < n; i++) {
        long long value;
        int duplicate = 0;
        if (scanf("%lld", &value) != 1 ||
            value < -VALUE_LIMIT || value > VALUE_LIMIT) {
            printf("Invalid value\n");
            return 1;
        }
        root = insertNode(root, value, &duplicate);
        if (duplicate) printf("Duplicate %lld ignored\n", value);
    }

    long long sorted[MAX_EMPLOYEES];
    int actual = inOrderValues(root, sorted, 0);
    printf("In-order (sorted):");
    for (int i = 0; i < actual; i++) printf(" %lld", sorted[i]);
    printf("\n");
    printf("Validation with (low, high) windows: %s\n",
           validateBst(root, 0, 0, 0, 0) ? "valid BST" : "NOT a BST");
    printf("Tree holds %d distinct values (%d nodes)\n", actual,
           countNodes(root));

    int k;
    printf("\nWhich rank k to find (1..%d)? ", actual);
    if (scanf("%d", &k) != 1 || k < 1 || k > actual) {
        printf("Invalid rank\n");
        return 1;
    }
    int visited = 0;
    long long answer = 0;
    kthSmallest(root, k, &visited, &answer);
    printf("%d-th smallest = %lld\n", k, answer);

    printf("\nHow many floor/ceil queries (0-%d)? ", MAX_EMPLOYEES);
    int tests;
    if (scanf("%d", &tests) != 1 || tests < 0 || tests > MAX_EMPLOYEES) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < tests; i++) {
        long long value;
        printf("Query %d value: ", i + 1);
        if (scanf("%lld", &value) != 1) {
            printf("Invalid value\n");
            return 1;
        }
        long long floorResult = 0;
        long long ceilResult = 0;
        if (floorValue(root, value, &floorResult)) {
            printf("floor(%lld) = %lld; ", value, floorResult);
        } else {
            printf("floor(%lld) = none; ", value);
        }
        if (ceilValue(root, value, &ceilResult)) {
            printf("ceil(%lld) = %lld\n", value, ceilResult);
        } else {
            printf("ceil(%lld) = none\n", value);
        }
        printf("   search(%lld) = %s\n", value,
               searchNode(root, value) != NULL ? "found" : "absent");
    }

    long long low;
    long long high;
    printf("\nRange sum. Enter low high (low <= high): ");
    if (scanf("%lld %lld", &low, &high) != 2 || low > high) {
        printf("Invalid range\n");
        return 1;
    }
    printf("Range sum over [%lld..%lld] = %lld\n", low, high,
           rangeSum(root, low, high));

    long long toDelete;
    printf("\nValue to delete: ");
    if (scanf("%lld", &toDelete) != 1) {
        printf("Invalid value\n");
        return 1;
    }
    int removed = 0;
    root = deleteNode(root, toDelete, &removed);
    printf("Deleted %lld: %s\n", toDelete, removed ? "yes" : "not found");
    printf("In-order after delete:");
    printInOrder(root);
    printf("\nValidation after delete: %s\n",
           validateBst(root, 0, 0, 0, 0) ? "valid BST" : "NOT a BST");
    printf("Nodes remaining = %d\n", countNodes(root));
    poolUsed = 0;
    return 0;
}
