/*
Tree DP - House Robber III, Binary Tree Maximum Path Sum,
Binary Tree Cameras (Advanced -> Master)
----------------------------------------
PLATFORM PROBLEMS:
  LeetCode 337 - House Robber III (rob a tree; no two adjacent houses)
  LeetCode 124 - Binary Tree Maximum Path Sum (any-to-any path)
  LeetCode 968 - Binary Tree Cameras (minimum cameras covering all nodes)
  related: HackerRank "Tree: Height / Path Sum" family - the same
           bottom-up traversal; solved in tree-interview-problems.c.

PROBLEM DEFINITION:
Three problems where the answer at a node depends on what its children
report, so every node returns a SMALL TUPLE instead of a single number.
Robber III returns (best if this node is robbed, best if it is not),
Max Path Sum returns the best DOWNWARD gain while updating a global
"bend here" best, and Cameras returns (state, cameras) with three states:
uncovered, covered, has a camera.

REAL-LIFE EXAMPLE:
Incident response on a service dependency tree: covering every service
with the fewest monitoring agents where each agent also covers its
neighbours (cameras), choosing which machines to compromise without
alerting an adjacent pair (robber), and finding the most profitable
chain of services along any route (path sum).

DIAGRAM (House Robber III on 3 / \ 2 3 \ \ 3 1):
          3
         / \
        2   3
         \   \
          3   1
  return for each node = (withThis, withoutThis)
  leaf 3 -> (3, 0)        leaf 1 -> (1, 0)
  node 2 -> with  = 2 + 0        = 2
            without = max(3,0)   = 3        -> (2, 3)
  node 3 -> with  = 3 + 0        = 3
            without = max(1,0)   = 1        -> (3, 1)
  root 3 -> with  = 3 + 3 + 1                  = 7
            without = max(2,3) + max(3,1)      = 6   -> (7, 6)
  answer = max(7, 6) = 7  (rob the root, the 2 and the 3's right child)

PSEUDO CODE:
  RobberIII(node) -> (with, without):
      (lw, lwo) = left;  (rw, rwo) = right
      with    = node.value + lwo + rwo
      without = max(lw, lwo) + max(rw, rwo)
  MaxPathSum(node) -> best downward gain:
      leftGain  = max(0, MaxPathSum(left))
      rightGain = max(0, MaxPathSum(right))
      global = max(global, node.value + leftGain + rightGain)
      return node.value + max(leftGain, rightGain)
  Cameras(node) -> (state, count):
      0 = uncovered, 1 = covered, 2 = has a camera
      any child uncovered -> return (2, count+1)   (must place one here)
      any child has camera -> return (1, count)
      else                 -> return (0, count)
      a NULL child is COVERED (state 1)
      after the recursion: if the root is uncovered, add one camera

SAMPLE INPUT & DRY RUN:
  7                          <- tree 1: how many tokens
  3 2 3 N 3 N 1              <- tree 1 tokens, level-order ('N' = absent)
  7                          <- tree 2: how many tokens (a SECOND tree)
  0 0 0 0 0 N N              <- tree 2 tokens
  Both trees use the same format; the token count line is read again
  before the second tree, so the two trees never share state.
  Tree 1:      Tree 2 (the 968 tree):
        3            0
       / \          / \
      2   3        0   0
       \   \      / \
        3   1    0   0
  House Robber III: 7 (the diagram above; rob the root, the 2 and the
  3's right child).
  Binary Tree Maximum Path Sum: 12 - the path 3 -> 2 -> 3 -> 3 -> 1,
  i.e. the left subtree's deepest 3, the node 2, the root 3, the right
  child 3 and its child 1 (left gain 5, right gain 4, so 3 + 5 + 4).
  Extra hand-check for 124: -10 9 20 N N 15 7 gives 42 (15-20-7).
  Binary Tree Cameras on tree 2: 2 - one camera on each of the root's
  two children covers all five nodes.
  Max path sum of -10 / \ 9 20 / \ 15 7: best is 15 + 20 + 7 = 42.
  Cameras for 0 / \ 0 (N) 0 / \ 0 0 / \ N N N 0: the parent of the two
  leaves must hold a camera, so the answer is 3.

SPECIAL CASES:
  - Empty tree (token "N") is rejected: there is nothing to answer.
  - A single node is legal in all three: the robber answer is
    max(value, 0) because skipping a house is always allowed, the path
    sum is its value (paths may not be empty), and the camera answer
    is 1.
  - Max Path Sum is about ANY node-to-node path: for the sample above
    the best path is 15 + 20 + 7 = 42, not 9 + 20 (+15 or +7), because
    a path may only bend once. That is exactly why the function returns
    the best DOWNWARD gain but updates the global with the bend.
  - Negative path sums are allowed to be skipped: gains are clamped to
    0 with max(0, gain) so a subtree never drags the answer down, while
    the "return" value still reflects the actual downward chain.
  - The cameras DP is not a greedy: a leaf's parent is not always
    chosen, and the "uncovered child forces a camera here" rule must be
    checked BEFORE the covered rule, or the count comes out too small.
  - The final answer needs one more camera when the root ends uncovered.
  - Nodes <= 1000, |value| <= 10^6, level-order tokens with 'N' for a
    missing child (same serialisation as tree-interview-problems.c).

COMPLEXITY:
  O(n) time and O(height) stack for each part; every node returns at
  most two numbers, so the extra state is O(1) per frame.

TIPS:
  The pattern to learn is "return a tuple, not a number". Write the
  tuple's MEANING as a comment first (with/without, state/count,
  gain/value) and the recursions usually write themselves. Compare with
  tree-interview-problems.c, where every function returns one number -
  the hard versions are the ones that need two.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1000
#define NODE_LIMIT 1000000

typedef struct TreeNode {
    long long value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

void freeTree(TreeNode* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

TreeNode* newNode(long long value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) return NULL;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Same serialisation as tree-interview-problems.c: one queue of PARENTS,
 * each consuming two tokens ('N' marks a missing child). */
TreeNode* buildComplete(const long long values[], const char present[], int n,
                        int* ok) {
    *ok = 1;
    if (n == 0 || !present[0]) return NULL;
    TreeNode* root = newNode(values[0]);
    if (root == NULL) {
        *ok = 0;
        return NULL;
    }
    TreeNode* queue[MAX_TOKENS];
    int front = 0;
    int rear = 0;
    queue[rear++] = root;
    int i = 1;
    while (i < n && front < rear) {
        TreeNode* parent = queue[front++];
        if (present[i]) {
            parent->left = newNode(values[i]);
            if (parent->left == NULL) {
                *ok = 0;
                freeTree(root);
                return NULL;
            }
            queue[rear++] = parent->left;
        }
        i++;
        if (i < n && present[i]) {
            parent->right = newNode(values[i]);
            if (parent->right == NULL) {
                *ok = 0;
                freeTree(root);
                return NULL;
            }
            queue[rear++] = parent->right;
        }
        i++;
    }
    return root;
}

/* LeetCode 337: return (rob this node, skip this node). If we rob the
 * node we must skip both children; if we skip it we take the better of
 * robbing or skipping each child. */
void robberIII(const TreeNode* node, long long* withNode, long long* without) {
    if (node == NULL) {
        *withNode = 0;
        *without = 0;
        return;
    }
    long long leftWith;
    long long leftWithout;
    long long rightWith;
    long long rightWithout;
    robberIII(node->left, &leftWith, &leftWithout);
    robberIII(node->right, &rightWith, &rightWithout);
    *withNode = node->value + leftWithout + rightWithout;
    long long bestLeft = leftWith > leftWithout ? leftWith : leftWithout;
    long long bestRight = rightWith > rightWithout ? rightWith : rightWithout;
    *without = bestLeft + bestRight;
}

/* LeetCode 124: return the best DOWNWARD gain while updating the global
 * best with the best path that BENDS at this node. Negative subtrees are
 * clamped to 0 so they never drag a path down. */
long long maxPathSum(const TreeNode* node, long long* best) {
    if (node == NULL) return 0;
    long long left = maxPathSum(node->left, best);
    if (left < 0) left = 0;
    long long right = maxPathSum(node->right, best);
    if (right < 0) right = 0;
    long long bend = node->value + left + right;
    if (bend > *best) *best = bend;
    long long downward = node->value + (left > right ? left : right);
    return downward;
}

/* LeetCode 968: states 0 = uncovered, 1 = covered (no camera),
 * 2 = has a camera. Kids are processed before the parent decides. */
long long placeCameras(const TreeNode* node, int* state) {
    if (node == NULL) {
        *state = 1;                 /* a null child is already covered */
        return 0;
    }
    int leftState;
    int rightState;
    long long count = placeCameras(node->left, &leftState) +
                      placeCameras(node->right, &rightState);
    if (leftState == 0 || rightState == 0) {
        *state = 2;                 /* an uncovered child forces one here */
        return count + 1;
    }
    if (leftState == 2 || rightState == 2) {
        *state = 1;                 /* a child camera covers this node */
        return count;
    }
    *state = 0;                     /* both children covered, none has one */
    return count;
}

/* Reads and validates one level-order token list ('N' marks a missing
 * child, as in tree-interview-problems.c). Duplicate values are allowed
 * (the sample trees contain repeats), they only make node labels
 * ambiguous - the DP never needs to name a specific node. */
int readTokens(long long values[], char present[], int* count) {
    int n;
    printf("How many tokens (1-%d)? ", MAX_TOKENS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_TOKENS) {
        printf("Invalid token count\n");
        return -1;
    }
    printf("Enter %d tokens, level-order, 'N' for a missing child:\n", n);
    for (int i = 0; i < n; i++) {
        char token[64];
        if (scanf("%63s", token) != 1) {
            printf("Invalid token\n");
            return -1;
        }
        if (token[0] == 'N' && token[1] == '\0') {
            present[i] = 0;
            values[i] = 0;
            continue;
        }
        char* end = NULL;
        long long v = strtoll(token, &end, 10);
        if (end == token || *end != '\0' || v < -NODE_LIMIT ||
            v > NODE_LIMIT) {
            printf("Invalid token\n");
            return -1;
        }
        present[i] = 1;
        values[i] = v;
    }
    *count = n;
    return 0;
}

int main(void) {
    long long values[MAX_TOKENS];
    char present[MAX_TOKENS];
    int count = 0;
    int ok = 0;

    printf("Tree DP - House Robber III, Maximum Path Sum, Binary Tree "
           "Cameras\n\n");
    printf("=== Trees 1 and 2 use the SAME token format ===\n");
    if (readTokens(values, present, &count) != 0) return 1;

    TreeNode* root = buildComplete(values, present, count, &ok);
    if (!ok) {
        printf("Out of memory while building the tree\n");
        return 1;
    }
    if (root == NULL) {
        printf("Empty tree - there is nothing to answer\n");
        return 1;
    }

    long long withRoot = 0;
    long long withoutRoot = 0;
    robberIII(root, &withRoot, &withoutRoot);
    long long robbery = withRoot > withoutRoot ? withRoot : withoutRoot;
    printf("\nHouse Robber III (LeetCode 337): %lld\n", robbery);
    printf("  (rob the root: %lld, skip the root: %lld)\n", withRoot,
           withoutRoot);

    long long bestPath = root->value;
    maxPathSum(root, &bestPath);
    printf("Binary Tree Maximum Path Sum (LeetCode 124): %lld\n", bestPath);

    freeTree(root);

    /* The camera sample is usually a different tree, so read a second one. */
    if (readTokens(values, present, &count) != 0) return 1;
    TreeNode* camerasTree = buildComplete(values, present, count, &ok);
    if (!ok) {
        printf("Out of memory while building the tree\n");
        return 1;
    }
    if (camerasTree == NULL) {
        printf("Empty tree - no cameras to place\n");
        return 1;
    }
    int rootState = 0;
    long long cameras = placeCameras(camerasTree, &rootState);
    if (rootState == 0) cameras++;            /* the root is its own parent */
    printf("\nBinary Tree Cameras (LeetCode 968): %lld\n", cameras);
    printf("  (states: 0 uncovered, 1 covered, 2 has a camera)\n");

    freeTree(camerasTree);
    return 0;
}
