/*
Tree Problems Ladder 2 - Symmetry, Balance, Paths (Basic -> Hard)
------------------------------------------------------------------
Real life example:
Symmetry checks validate mirrored UI/component trees, balance
checks decide when to rebalance, and path sums drive budget or
dependency rollups in project tools.

DIAGRAM (symmetry and diameter):
        1          symmetric? mirror-compare the two subtrees:
       / \         (2.left vs 2.right) and (2.right vs 2.left)
      2   2
     /     \      diameter = height(left) + height(right) through
    3       3     every node; take the maximum.

LADDER (easy -> hard inside this file):
  1. Are two trees IDENTICAL?        (parallel recursion)
  2. Is the tree SYMMETRIC (mirror of itself)?
  3. Is the tree HEIGHT-BALANCED?    (AVL condition check)
  4. Root-to-leaf paths with a given sum
  5. Left boundary view

BEGINNER EXPLANATION:
- Identical: recurse BOTH trees at once - left==left, right==right.
- Symmetric: recurse one tree against its own MIRROR - compare
  left subtree with right subtree.
- Balanced: height function returns -1 as "already unbalanced"
  (early exit) - the elegant one-pass solution.
- Root-to-leaf sum: carry the remaining target down; subtract as
  you go; at a leaf, check if it equals what's left.

SUPER SIMPLE EXAMPLE (symmetric, 1(2,2)):
node 1: compare left 2 with right 2:
  left.left = NULL vs right.right = NULL (match)
  left.right = NULL vs right.left = NULL (match)
  -> symmetric. Values equal at every mirrored pair.

WHAT IF THE RESULT LOOKS DIFFERENT? (special cases):
- Empty trees: two empty trees are identical AND symmetric AND
  balanced - all "vacuously true".
- Symmetric check is NOT "identical to mirror": comparing left vs
  right directly is the same thing but cheaper (no mirror build).
- Balance check on a skewed chain: -1 propagates up instantly -
  O(n) not O(n log n) (the naive height-everywhere version).
- Path sum with negative values: the target can go negative
  mid-path; only the LEAF check matters (no pruning early unless
  all values are positive - state your assumption).
- Single node path-sum: node itself equals target -> true.

DRY RUN (balanced check on 1(2(3),NULL)):
height(3) = 1. height(2) = max(1, 0) + 1 = 2 -> balanced there.
height(1): left = 2, right = 0 -> diff 2 > 1 -> NOT balanced.
The -1 shortcut would have stopped at the imbalance - trace it.

TIPS:
- "Recurse two trees in lockstep" is the template for identical/
  symmetric/subtree-of questions - one pattern, three problems.
- The -1-as-error height trick is the canonical O(n) balance
  check; it appears in avl_tree.c's balance factor logic.
- Root-to-leaf path enumeration feeds directly into path-sum
  variants and the "serialize all paths" problems.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

typedef struct PNode {
    int data;
    struct PNode* left;
    struct PNode* right;
} PNode;

PNode* newPNode(int v) {
    PNode* n = (PNode*)malloc(sizeof(PNode));
    n->data = v;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/*
 * buildTreeManual - Level-order build (0 = absent child).
 */
PNode* buildTreeManual(void) {
    int v;
    printf("Root value (0 = empty tree): ");
    scanf("%d", &v);
    if (v == 0) {
        return NULL;
    }
    PNode* root = newPNode(v);
    PNode* queue[200];
    int front = 0;
    int rear = 0;
    queue[rear] = root;
    rear = rear + 1;

    while (front < rear) {
        PNode* curr = queue[front];
        front = front + 1;
        printf("Left child of %d (0 = none): ", curr->data);
        scanf("%d", &v);
        if (v != 0) {
            curr->left = newPNode(v);
            queue[rear] = curr->left;
            rear = rear + 1;
        }
        printf("Right child of %d (0 = none): ", curr->data);
        scanf("%d", &v);
        if (v != 0) {
            curr->right = newPNode(v);
            queue[rear] = curr->right;
            rear = rear + 1;
        }
    }
    return root;
}

/*
 * identical - Parallel recursion on two trees.
 */
int identical(PNode* a, PNode* b) {
    if (a == NULL && b == NULL) {
        return 1;
    }
    if (a == NULL || b == NULL) {
        return 0;
    }
    return a->data == b->data &&
           identical(a->left, b->left) &&
           identical(a->right, b->right);
}

/*
 * symmetric - Compare a tree with its own mirror.
 */
int symmetric(PNode* l, PNode* r) {
    if (l == NULL && r == NULL) {
        return 1;
    }
    if (l == NULL || r == NULL) {
        return 0;
    }
    return l->data == r->data &&
           symmetric(l->left, r->right) &&
           symmetric(l->right, r->left);
}

/*
 * balancedHeight - Returns height, or -1 when unbalanced.
 */
int balancedHeight(PNode* root) {
    if (root == NULL) {
        return 0;
    }
    int lh = balancedHeight(root->left);
    if (lh == -1) {
        return -1;
    }
    int rh = balancedHeight(root->right);
    if (rh == -1) {
        return -1;
    }
    if (lh - rh > 1 || rh - lh > 1) {
        return -1;
    }
    return 1 + (lh > rh ? lh : rh);
}

/*
 * pathSum - Root-to-leaf paths matching target (prints each).
 */
void pathSum(PNode* root, int remaining, int path[], int depth) {
    if (root == NULL) {
        return;
    }
    path[depth] = root->data;
    depth = depth + 1;
    remaining = remaining - root->data;

    if (root->left == NULL && root->right == NULL) {
        if (remaining == 0) {
            printf("  ");
            for (int i = 0; i < depth; i++) {
                printf("%d ", path[i]);
            }
            printf("\n");
        }
        return;
    }
    pathSum(root->left, remaining, path, depth);
    pathSum(root->right, remaining, path, depth);
}

/*
 * leftBoundary - Root, then leftmost spine (excluding leaves).
 */
void leftBoundary(PNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->data);
    PNode* curr = root->left;
    while (curr != NULL) {
        if (curr->left != NULL || curr->right != NULL) {
            printf("%d ", curr->data);
        }
        if (curr->left != NULL) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
}

void freePTree(PNode* root) {
    if (root == NULL) {
        return;
    }
    freePTree(root->left);
    freePTree(root->right);
    free(root);
}

int main() {
    printf("Tree Problems Ladder 2 (easy -> hard)\n\n");

    printf("TREE 1:\n");
    PNode* root = buildTreeManual();

    if (root == NULL) {
        printf("Empty tree\n");
        return 0;
    }

    int choice;
    while (1) {
        printf("\n1. Symmetric (mirror of itself)?\n");
        printf("2. Height-balanced (AVL condition)?\n");
        printf("3. Root-to-leaf paths with sum S\n");
        printf("4. Left boundary view\n");
        printf("5. Identical check (build TREE 2 to compare)\n");
        printf("6. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("Symmetric: %s\n",
                   symmetric(root->left, root->right) ? "YES" : "NO");
        } else if (choice == 2) {
            printf("Height-balanced: %s\n",
                   balancedHeight(root) >= 0 ? "YES" : "NO");
        } else if (choice == 3) {
            int target;
            int path[100];
            printf("Target sum: ");
            scanf("%d", &target);
            printf("Paths:\n");
            pathSum(root, target, path, 0);
        } else if (choice == 4) {
            printf("Left boundary: ");
            leftBoundary(root);
            printf("\n");
        } else if (choice == 5) {
            printf("TREE 2:\n");
            PNode* root2 = buildTreeManual();
            printf("Identical: %s\n", identical(root, root2) ? "YES" : "NO");
            freePTree(root2);
        } else if (choice == 6) {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }

    freePTree(root);
    return 0;
}
