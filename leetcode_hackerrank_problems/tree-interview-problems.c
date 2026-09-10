/*
Tree Interview Problems - Depth, Balance, Diameter, Invert, LCA, Paths
(Intermediate -> Advanced)
-----------------------------------------------------------------------
PLATFORM PROBLEMS:
  LeetCode 104 - Maximum Depth of Binary Tree
  LeetCode 110 - Balanced Binary Tree
  LeetCode 543 - Diameter of Binary Tree (computed WITHOUT height nodes)
  LeetCode 226 - Invert Binary Tree
  LeetCode 112 - Path Sum
  LeetCode 236 - Lowest Common Ancestor of a Binary Tree
  LeetCode 102 - Binary Tree Level Order Traversal
  HackerRank "Tree: Height of a Binary Tree" (same traversal family)

PROBLEM DEFINITION:
Six recursion drills on one binary tree, built from a level-order
token list with 'N' marking a missing child. Each problem teaches a
different recursion shape: return one number, carry a running best,
mutate in place, or combine two subtree answers.

REAL-LIFE EXAMPLE:
A folder tree: its depth (nesting level) drives UI indentation, its
diameter (longest chain of nested folders) matters for path-length
limits, and the lowest common ancestor of two files is their shared
parent folder for a "copy relative path" feature.

DIAGRAM (tree built from: 3 9 20 N N 15 7):
        3
       / \
      9  20
         / \
        15  7
  in-order      : 9 3 15 20 7        (recursive)
  level-order   : 3 9 20 15 7        (queue)
  depth        : 3                    (nodes on the deepest path)
  balanced     : yes
  diameter     : 4                    (9-3-20-15 or 9-3-20-7, nodes)
  invert       : 3 20 9 7 15 -> 20 7 15 9
  path sum 38  : 3 + 20 + 15 = 38     -> yes
  LCA(15, 7)   : 20

PSEUDO CODE:
  MaxDepth(node) = node == null ? 0 : 1 + max(MaxDepth(l), MaxDepth(r))
  Balanced(node): returns -1 for imbalance, else the exact height;
      balanced iff abs(height(l) - height(r)) <= 1 at every node
  Diameter(node): post-order; at each node
      best = max(best, height(l) + height(r) + 1)   // nodes on path
  Invert(node): swap children; recurse (post-order also works)
  PathSum(node, remaining): null -> false; leaf -> remaining == value;
      else recurse with remaining - value
  LCA(root, p, q): if root is null or equals p or q return root;
      recurse left and right; if both non-null -> root is the LCA

SAMPLE INPUT & DRY RUN:
  7                       <- how many level-order tokens
  3 9 20 N N 15 7         <- tokens, LeetCode style ('N' = missing)
  1                       <- how many path-sum targets
  38                      <- target 1
  1                       <- how many LCA queries
  9 20                    <- the node-value pair to query
  The trace in the diagram above; the program prints all six results
  for the sample so every recursion can be checked by hand.
  A root-to-leaf path 3 -> 20 -> 15 sums to 38, 9 and 20 meet at 3, and
  the diameter is the 4-node path 9 -> 3 -> 20 -> 15 (or ...-> 7).

SPECIAL CASES:
  - Empty tree (n = 0): depth 0, diameter 0, balanced yes, no LCA.
  - Single node: depth 1, diameter 1, path sum equals that value.
  - A missing child is 'N'. Parsing follows the standard LeetCode
    serialize format: one queue of PARENTS, each parent consuming the
    next two tokens (or 'N'). Children listed after a missing slot
    therefore belong to the next present parent, exactly as
    [1, N, 2, 3] builds 1 -> right 2 -> left 3.
  - Depth and diameter here COUNT NODES; the README and the printed
    labels state this convention explicitly (edges = nodes - 1).
  - Balanced needs the -1 sentinel: comparing heights bottom-up avoids
    the O(n^2) "compute height at every node" trap.
  - Duplicate values are REJECTED: with duplicates, "which node did you
    mean?" is ambiguous for path sums and LCA, so the unique-value
    invariant is enforced at input time (the classic LCA pitfall).
  - Sentinel choice: -1 is NOT a reserved "missing" marker here. The
    input token 'N' means missing, and -1 is a perfectly legal node
    value (the earlier draft wrongly rejected -1).
  - Every malloc is checked; an allocation failure frees the partial
    tree and reports "Out of memory" instead of dereferencing NULL.
  - Tokens <= 200, values within +/-10^9.

COMPLEXITY:
  Every routine is O(n) time; recursion depth is O(h), so a completely
  skewed tree uses O(n) stack (the reason production code prefers an
  explicit stack for adversarial input).

TIPS:
  Post-order recursion (children first) is the shape for anything that
  COMBINES subtree results - height, diameter, balancedness. Pre-order
  is for anything that CARRIES state down - path sums and LCA checks.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 200

typedef struct TreeNode {
    long long value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct {
    TreeNode* nodes[MAX_TOKENS];
} Tree;

void freeTree(TreeNode* node);

TreeNode* newNode(long long value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) return NULL;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Builds the tree from a level-order array using the standard LeetCode
 * serialization: `present[i] == 0` marks a missing child ('N' in the
 * input) and one queue of PARENTS consumes two tokens each. The value
 * itself is unconstrained, so -1 is a legal node value. Returns NULL on
 * allocation failure with *ok set to 0; the partial tree is freed. */
TreeNode* buildComplete(const long long values[], const int present[], int n,
                        int* ok) {
    *ok = 1;
    if (n == 0 || !present[0]) return NULL;
    TreeNode* root = newNode(values[0]);
    if (root == NULL) {
        *ok = 0;
        return NULL;
    }
    Tree queue;
    int front = 0;
    int rear = 0;
    queue.nodes[rear++] = root;
    int i = 1;
    while (i < n && front < rear) {
        TreeNode* parent = queue.nodes[front++];
        if (present[i]) {
            parent->left = newNode(values[i]);
            if (parent->left == NULL) {
                *ok = 0;
                freeTree(root);
                return NULL;
            }
            queue.nodes[rear++] = parent->left;
        }
        i++;
        if (i < n && present[i]) {
            parent->right = newNode(values[i]);
            if (parent->right == NULL) {
                *ok = 0;
                freeTree(root);
                return NULL;
            }
            queue.nodes[rear++] = parent->right;
        }
        i++;
    }
    return root;
}

void freeTree(TreeNode* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

void inorder(const TreeNode* node) {
    if (node == NULL) return;
    inorder(node->left);
    printf(" %lld", node->value);
    inorder(node->right);
}

int maxDepth(const TreeNode* node) {
    if (node == NULL) return 0;
    int left = maxDepth(node->left);
    int right = maxDepth(node->right);
    return 1 + (left > right ? left : right);
}

int checkBalanced(const TreeNode* node, int* balanced) {
    if (node == NULL) return 0;
    int left = checkBalanced(node->left, balanced);
    if (*balanced == 0) return 0;
    int right = checkBalanced(node->right, balanced);
    if (*balanced == 0) return 0;
    int difference = left - right;
    if (difference > 1 || difference < -1) {
        *balanced = 0;
        return 0;
    }
    return 1 + (left > right ? left : right);
}

int diameterNodes(const TreeNode* node, int* best) {
    if (node == NULL) return 0;
    int left = diameterNodes(node->left, best);
    int right = diameterNodes(node->right, best);
    int through = left + right + 1;
    if (through > *best) *best = through;
    return 1 + (left > right ? left : right);
}

void invertTree(TreeNode* node) {
    if (node == NULL) return;
    TreeNode* temp = node->left;
    node->left = node->right;
    node->right = temp;
    invertTree(node->left);
    invertTree(node->right);
}

void levelOrder(const TreeNode* root) {
    if (root == NULL) return;
    const TreeNode* queue[MAX_TOKENS];
    int front = 0;
    int rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        const TreeNode* node = queue[front++];
        printf(" %lld", node->value);
        if (node->left != NULL) queue[rear++] = node->left;
        if (node->right != NULL) queue[rear++] = node->right;
    }
}

int hasPathSum(const TreeNode* node, long long remaining) {
    if (node == NULL) return 0;
    if (node->left == NULL && node->right == NULL) {
        return node->value == remaining;
    }
    return hasPathSum(node->left, remaining - node->value) ||
           hasPathSum(node->right, remaining - node->value);
}

const TreeNode* lowestCommonAncestor(const TreeNode* root,
                                     const TreeNode* first,
                                     const TreeNode* second) {
    if (root == NULL || root == first || root == second) return root;
    const TreeNode* left = lowestCommonAncestor(root->left, first, second);
    const TreeNode* right = lowestCommonAncestor(root->right, first, second);
    if (left != NULL && right != NULL) return root;
    return left != NULL ? left : right;
}

const TreeNode* findNode(const TreeNode* node, long long value) {
    if (node == NULL) return NULL;
    if (node->value == value) return node;
    const TreeNode* found = findNode(node->left, value);
    return found != NULL ? found : findNode(node->right, value);
}

int main(void) {
    int tokenCount;
    long long values[MAX_TOKENS];
    int present[MAX_TOKENS];

    printf("Tree Interview Problems\n\n");
    printf("How many level-order tokens (0-%d)? ", MAX_TOKENS);
    if (scanf("%d", &tokenCount) != 1 || tokenCount < 0 ||
        tokenCount > MAX_TOKENS) {
        printf("Invalid token count\n");
        return 1;
    }
    printf("Enter tokens for the level-order array, LeetCode style\n");
    printf("('N' = missing child; a parent consumes the next two tokens):\n");
    for (int i = 0; i < tokenCount; i++) {
        char token[32];
        if (scanf("%31s", token) != 1) {
            printf("Invalid token\n");
            return 1;
        }
        if (strcmp(token, "N") == 0 || strcmp(token, "n") == 0) {
            present[i] = 0;
            values[i] = 0;
            continue;
        }
        long long parsed;
        if (sscanf(token, "%lld", &parsed) != 1 ||
            parsed < -1000000000LL || parsed > 1000000000LL) {
            printf("Invalid token value (use N for a missing child)\n");
            return 1;
        }
        /* Duplicate-free values keep findNode/LCA unambiguous: -1 is a
         * legal value now, so the missing-child marker is the separate
         * present[] flag instead of a reserved sentinel number. */
        for (int j = 0; j < i; j++) {
            if (present[j] && values[j] == parsed) {
                printf("Invalid input: duplicate node value %lld makes "
                       "path-sum and LCA queries ambiguous\n", parsed);
                return 1;
            }
        }
        present[i] = 1;
        values[i] = parsed;
    }

    int buildOk = 1;
    TreeNode* root = buildComplete(values, present, tokenCount, &buildOk);
    if (!buildOk) {
        printf("Out of memory while building the tree\n");
        return 1;
    }
    printf("\nIn-order traversal:");
    inorder(root);
    printf("\n");
    printf("Level-order traversal:");
    levelOrder(root);
    printf("\n");
    printf("Maximum depth (nodes) = %d\n", maxDepth(root));
    int balanced = 1;
    checkBalanced(root, &balanced);
    printf("Balanced = %s\n", balanced ? "yes" : "no");
    int best = 0;
    diameterNodes(root, &best);
    printf("Diameter (nodes on the longest path) = %d\n", best);

    printf("\nInvert (mirror) the tree in place.\n");
    invertTree(root);
    printf("Level-order after invert:");
    levelOrder(root);
    printf("\nIn-order after invert (reverse of before):");
    inorder(root);
    printf("\n");
    invertTree(root);

    printf("\nHow many path-sum targets (0-%d)? ", MAX_TOKENS);
    int tests;
    if (scanf("%d", &tests) != 1 || tests < 0 || tests > MAX_TOKENS) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < tests; i++) {
        long long target;
        printf("Target %d (root-to-leaf sum): ", i + 1);
        if (scanf("%lld", &target) != 1 ||
            target < -1000000000000000000LL ||
            target > 1000000000000000000LL) {
            printf("Invalid target\n");
            return 1;
        }
        printf("Root-to-leaf path with sum %lld: %s\n", target,
               hasPathSum(root, target) ? "yes" : "no");
    }

    printf("\nLowest common ancestor. How many node pairs (0-%d)? ",
           MAX_TOKENS);
    if (scanf("%d", &tests) != 1 || tests < 0 || tests > MAX_TOKENS) {
        printf("Invalid query count\n");
        return 1;
    }
    for (int i = 0; i < tests; i++) {
        long long firstValue;
        long long secondValue;
        printf("Pair %d (two node values): ", i + 1);
        if (scanf("%lld %lld", &firstValue, &secondValue) != 2) {
            printf("Invalid pair\n");
            return 1;
        }
        const TreeNode* first = findNode(root, firstValue);
        const TreeNode* second = findNode(root, secondValue);
        if (first == NULL || second == NULL) {
            printf("LCA(%lld, %lld): node not found\n", firstValue,
                   secondValue);
            continue;
        }
        const TreeNode* ancestor = lowestCommonAncestor(root, first, second);
        printf("LCA(%lld, %lld) = %lld\n", firstValue, secondValue,
               ancestor->value);
    }
    freeTree(root);
    return 0;
}
